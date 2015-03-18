#include "treewidget.h"
#include "constants.h"
#include "plugin.h"
#include "archivereader.h"
#include "scriptgup.h"
#include "main_menu.h"

#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

#include <QDebug>

TreeWidget::TreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAlternatingRowColors(true);

    m_reader = new XmlReader(this);
    m_writer = new XmlWriter(this);

    connect( this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
             this, SLOT(runScript(QTreeWidgetItem*, int)) );
}

TreeWidget::~TreeWidget()
{
}

void TreeWidget::contextMenuEvent(QContextMenuEvent */*event*/)
{
    QMenu* popMenu = new QMenu(this);

    QAction* addCategoryAct = popMenu->addAction(tr("Add &Category"));
    QAction* removeCategoryAct = popMenu->addAction(tr("&Remove Category"));
    popMenu->addSeparator();
    QAction* addFileAct = popMenu->addAction(tr("Add &File"));
    QAction* removeFileAct = popMenu->addAction(tr("R&emove File"));

    connect(addCategoryAct, SIGNAL( triggered() ), this, SLOT( addCategory() ));
    connect(removeCategoryAct, SIGNAL( triggered() ), this, SLOT( deleteCategory() ));
    connect(addFileAct, SIGNAL( triggered() ), this, SLOT( addFile() ));
    connect(removeFileAct, SIGNAL( triggered() ), this, SLOT( deleteFile() ));

//    int _x = mapFromGlobal( cursor().pos() ).x();
//    int _y = mapFromGlobal( cursor().pos() ).y() - this->header()->height();// 如果有表头,需要减去表头高度
//    QTreeWidgetItem *item = this->itemAt( QPoint(_x, _y) );
//    if( !item )
//    {
//        this->clearSelection();
//        this->setCurrentItem(0);
////        this->selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::Clear);
//        removeCategoryAct->setEnabled(false);
//        removeFileAct->setEnabled(false);
//    }

    popMenu->exec(cursor().pos());
}

bool TreeWidget::loadFile(const QString &xmlFile)
{
    if (xmlFile.isEmpty())
        return false;

    QFile file(xmlFile);
    if ( !file.open(QFile::ReadOnly | QFile::Text ))
    {
        QMessageBox::warning(this, tr("Simple XML"),
                             tr("Cannot load file %1:\n%2.")
                             .arg(xmlFile)
                             .arg(file.errorString()));
        return false;
    }

    if (!m_reader->read(&file))
    {
        QMessageBox::warning( this, tr("Simple XML"),
                            tr("Parse error in file %1:\n\n%2")
                            .arg(xmlFile).arg( m_reader->errorString() ) );
        return false;
    }

    file.close();

    return true;
}

bool TreeWidget::saveFile(const QString &xmlFile)
{
    if (xmlFile.isEmpty())
        return false;

    QFile file(xmlFile);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Simple XML"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(xmlFile)
                             .arg( file.errorString() ));
        return false;
    }

    if ( !m_writer->write(&file) )
    {
        QMessageBox::warning(this, tr("Simple XML"),
                            tr("Save error in file %1:\n\n%2")
                            .arg(xmlFile).arg(file.errorString()));
        return false;
    }

    return true;
}

void TreeWidget::addCategory()
{
    QTreeWidgetItem *selItem = this->currentItem();
    QTreeWidgetItem *newItem = NULL;
    if( !selItem )
    {
        newItem = new QTreeWidgetItem(this);
        newItem->setFlags(newItem->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
    }
    else
    {
        QString tagName = selItem->data(0, Qt::UserRole).toString();
        if ( tagName == XML_TAG_CATEGORY )
        {
            newItem = new QTreeWidgetItem(selItem);
            newItem->setFlags(selItem->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
        }
        else
        {
            newItem = new QTreeWidgetItem(selItem->parent());
            newItem->setFlags(selItem->parent()->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
        }
    }

    newItem->setText( 0, tr("[New CateGory]") );
    newItem->setData( 0, Qt::UserRole, XML_TAG_CATEGORY );
}

void TreeWidget::deleteCategory()
{
    QTreeWidgetItem *selItem = this->currentItem();
    if( !selItem ) return;
    QString tagName = selItem->data(0, Qt::UserRole).toString();
    if ( tagName == XML_TAG_CATEGORY )
    {
        delete selItem;
    }
    else
    {
        delete selItem;
    }
}

void TreeWidget::addFile()
{
    QTreeWidgetItem *selItem = this->currentItem();
    QTreeWidgetItem *newItem = NULL;
    if( !selItem )
    {
        newItem = new QTreeWidgetItem(this);
        newItem->setFlags(newItem->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
        newItem->setFlags(newItem->flags() & ~Qt::ItemIsDropEnabled );
    }
    else
    {
        QString tagName = selItem->data(0, Qt::UserRole).toString();
        if ( tagName == XML_TAG_CATEGORY )
        {
            newItem = new QTreeWidgetItem(selItem);
            newItem->setFlags(selItem->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
        }
        else
        {
            newItem = new QTreeWidgetItem(selItem->parent());
            newItem->setFlags(selItem->parent()->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
        }
        newItem->setFlags(newItem->flags() & ~Qt::ItemIsDropEnabled );
    }

    newItem->setText( 0, tr("[New File]") );
    newItem->setData( 0, Qt::UserRole, XML_TAG_FILE );
}

void TreeWidget::deleteFile()
{
    QTreeWidgetItem *selItem = this->currentItem();
    if( !selItem ) return;
    QString tagName = selItem->data(0, Qt::UserRole).toString();
    if ( tagName == XML_TAG_FILE )
    {
        delete selItem;
    }
}

void TreeWidget::runScript(QTreeWidgetItem *item, int /*column*/)
{
    if( !item ) return;

    QString tagName = item->data(0, Qt::UserRole).toStringList().at(0);
    if ( tagName == XML_TAG_CATEGORY )
        return;

    QString data = parseFile( item );

    if( data.isEmpty() )
        return;

    ScriptGup gup;
//    QString script = QString("FileIn \"%1\"").arg(data);
//    bool ok = gup.ExecuteStringScript(script.toLocal8Bit().data());
    bool ok = gup.ExecuteFileScript(data.toLocal8Bit().data());
    if( !ok )
    {
        gup.ExecuteStringScript("actionMan.executeAction 0 \"40472\"");
    }
    QFile temp(data);
    temp.remove();
}

void TreeWidget::listArchiveFiles()
{
    QString data_path = Plugin::dataPath();
    QDir dir( data_path );
    if( !dir.exists() ) return;

    QStringList filters;
    filters << "*.zip" << "*.yori" << "*.sth";
    dir.setNameFilters(filters);
    QFileInfoList archives = dir.entryInfoList(QDir::Files);

    ArchiveReader archReader;
    foreach( QFileInfo archive, archives)
    {
        QStringList files = archReader.listFiles( archive.filePath(), ARCHIVE_PASSWORD );

        QTreeWidgetItem *parent = new QTreeWidgetItem();
        parent->setFlags(parent->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
        QFileInfo file(archive);
        QString basename = file.baseName() + QString(" (%1)").arg(files.count());
        parent->setText( 0, basename );
        parent->setData( 0, Qt::UserRole, XML_TAG_CATEGORY );
        this->addTopLevelItem( parent );

        foreach( QString f, files )
        {
            QTreeWidgetItem *subItem = new QTreeWidgetItem(parent);
            subItem->setFlags(parent->flags() & ~Qt::ItemIsDropEnabled );
            subItem->setText(0, f);
            QStringList list;
            list.append(XML_TAG_FILE);
            list.append(archive.filePath());
            subItem->setData(0, Qt::UserRole, list);
            this->addTopLevelItem( subItem );
        }
    }
}

QString TreeWidget::parseFile(QTreeWidgetItem *item)
{
    QString data;
    QString data_path = Plugin::dataPath();
    QDir dir( data_path );
    if( !dir.exists() ) return data;

    QString text = item->text(0);
    QString tagName = item->data(0, Qt::UserRole).toStringList().at(0);

    if ( tagName == XML_TAG_FILE )
    {
        QString archive = item->data( 0, Qt::UserRole).toStringList().at(1);
        ArchiveReader reader;
        data = reader.readArchiveFile(text, archive, ARCHIVE_PASSWORD);
    }

    return data;
}
