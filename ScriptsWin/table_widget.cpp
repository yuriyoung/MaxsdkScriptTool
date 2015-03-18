#include "table_widget.h"
#include "scriptgup.h"
#include "archivereader.h"
#include "scripts_widget.h"
#include "plugin.h"
#include "constants.h"
#include "sqlwriter.h"
#include "preferences.h"

#include <QHeaderView>
#include <QToolButton>
#include <QPushButton>
#include <QStringList>
#include <QGridLayout>
#include <QDir>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QToolBox>
#include <QSignalMapper>
#include <QContextMenuEvent>

#include <QDebug>

TableWidget::TableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    initialize();
}

void TableWidget::initialize()
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);// 禁止编辑
    this->verticalHeader()->setVisible(false); //隐藏列表头
    this->horizontalHeader()->setVisible(false); //隐藏行表头

    this->setDragEnabled(false);
//    this->setDragDropMode(QAbstractItemView::InternalMove);
//    this->setShowGrid(false);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setFocusPolicy( Qt::ClickFocus );
    this->setFrameShape(QFrame::NoFrame);
    this->setFrameShadow(QFrame::Sunken);
    this->setLineWidth(3);
    this->setCornerButtonEnabled(false);
    this->setAlternatingRowColors(false);
    this->horizontalHeader()->setDefaultSectionSize(180);
    this->horizontalHeader()->setStretchLastSection(true);
    this->verticalHeader()->setStretchLastSection(false);
    this->setObjectName(QString::fromUtf8("tableWidget"));
    this->setColumnCount(1);
}

/*
 * 添加一列有图标的按钮,按钮包含标题,图标和分类
 * 返回添加的按钮
 */
QList<QToolButton*> TableWidget::addButtons(const QStringList &titles, const QList<QString> &icons, const QString &category)
{
    QList<QToolButton*> btnList;
    int begin = rowCount();
    int total = begin + titles.count();
    this->setRowCount(total);
    for(int i = begin; i < total; ++i )
    {
        int j = i-begin;
        QString title = titles.at(j);
        QString icon = icons.at(j);
        QToolButton *toolbtn = new QToolButton(this);
        toolbtn->setIcon( QIcon(icon) );
        toolbtn->setIconSize(QSize(24, 24));
        toolbtn->setText( title );
        toolbtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        connect( toolbtn, SIGNAL(clicked()), this, SLOT(buttonClicked()) );

        // new item within a button
        QTableWidgetItem *item = new QTableWidgetItem;
        QStringList dataList;
        dataList << title << icon << category;
        item->setData( Qt::UserRole, dataList );
        this->setItem(i, 0, item);

        this->setCellWidget(i, 0, toolbtn);
        btnList.append(toolbtn);
    }

    return btnList;
}

/*
 * 添加一列没有图标的按钮,按钮包含标题和分类
 * 返回添加的按钮
 */
QList<QToolButton*> TableWidget::addButtons(const QStringList &titles, const QString &category)
{
    QList<QToolButton*> btnList;
    int begin = rowCount();
    int total = begin + titles.count();
    this->setRowCount(total);
    for(int i = begin; i < total; ++i )
    {
        int j = i-begin;
        QString title = titles.at(j);
        QToolButton *toolbtn = new QToolButton(this);
        toolbtn->setIconSize(QSize(24, 24));
        toolbtn->setText( title );
        toolbtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        connect( toolbtn, SIGNAL(clicked()), this, SLOT(buttonClicked()) );

        // new item within a button
        QTableWidgetItem *item = new QTableWidgetItem;
        QStringList dataList;
        dataList << title << category;
        item->setData( Qt::UserRole, dataList );
        this->setItem(i, 0, item);

        this->setCellWidget(i, 0, toolbtn);
        btnList.append(toolbtn);
    }

    return btnList;
}

/*
 * 添加一个有图标的按钮,按钮包含标题,图标和分类
 * 返回添加的按钮
 */
QToolButton* TableWidget::addButton(const QString &title, const QString &icon, const QString &category)
{
    if( hasButton( title ) )
    {
        return NULL;
    }
    this->setRowCount(rowCount()+1);

    QToolButton *toolbtn = new QToolButton(this);
    toolbtn->setIcon( QIcon(icon) );
    toolbtn->setIconSize(QSize(24, 24));
    toolbtn->setText( title );
    toolbtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // new item within a button
    QTableWidgetItem *item = new QTableWidgetItem;
    QStringList dataList;
    dataList << title << icon << category;
    item->setData( Qt::UserRole, dataList );
    this->setItem(rowCount() - 1, 0, item);

    this->setCellWidget( rowCount()-1, 0, toolbtn );

    connect( toolbtn, SIGNAL(clicked()), this, SLOT(buttonClicked()) );

    return toolbtn;
}

/*
 * 添加一个没有图标的按钮,按钮包含标题和分类
 * 返回添加的按钮
 */
QToolButton* TableWidget::addButton(const QString &title, const QString &category)
{
    this->setRowCount(rowCount()+1);

    QToolButton *toolbtn = new QToolButton(this);
    toolbtn->setIconSize(QSize(24, 24));
    toolbtn->setText( title );
    toolbtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // new item within a button
    QTableWidgetItem *item = new QTableWidgetItem;
    QStringList dataList;
    dataList << title << category;
    item->setData( Qt::UserRole, dataList );
    this->setItem(rowCount() - 1, 0, item);

    this->setCellWidget( rowCount()-1, 0, toolbtn );

    connect( toolbtn, SIGNAL(clicked()), this, SLOT(buttonClicked()) );

    return toolbtn;
}

QList<QToolButton *> TableWidget::itemButtons()
{
    QList<QToolButton *> btnList;
    this->rowCount();
    for(int i = 0; i < rowCount(); ++i)
    {
        QWidget *widget = this->cellWidget(i, 0);
        QToolButton* toolbtn = qobject_cast<QToolButton*>(widget);
        if( toolbtn )
            btnList.append(toolbtn);
    }

    return btnList;
}

QToolButton *TableWidget::itemButton(int row)
{
    QWidget *widget = this->cellWidget(row, 0);
    QToolButton* toolbtn = qobject_cast<QToolButton*>(widget);
    if( !toolbtn )
        return toolbtn=NULL;

    return toolbtn;
}

bool TableWidget::hasButton(const QString &title)
{
    QList<QTableWidgetItem*> list = this->findItems(title, Qt::MatchExactly);
    if( list.count() > 0)
        return true;

    return false;
}

void TableWidget::buttonClicked()
{
    QToolButton *btn = qobject_cast<QToolButton *>(sender());
    for(int row = 0; row < this->rowCount(); ++row)
    {
        QWidget *widget = this->cellWidget(row, 0);
        QToolButton *btn1 = qobject_cast<QToolButton*>(widget);
        if( btn == btn1)
        {
            this->clearSelection();
            this->item(row, 0)->setSelected(true);
            QString filename = item(row, 0)->data(Qt::UserRole).toStringList().at(0);
            QString category = item(row, 0)->data(Qt::UserRole).toStringList().at(2);
            emit runScript(filename, category);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////
/**
 * @添加一个脚本文件
 * @函数: TableWidget::addScript
 */
void TableWidget::addScriptAct()
{
    QString filename = QFileDialog::getOpenFileName(
           this,
           tr("Open 3dsmax maxscript"),
           Plugin::g_preferences->m_lastOpenFilePath,
           tr("maxscript files (*.ms *.mse);;All files(*.*)") );
    if (filename.isEmpty())
    {
        return;
    }


    int idx = Plugin::g_scriptsWidget->toolbox()->currentIndex();
    QString category =  Plugin::g_preferences->toolboxCategorys().at(idx);
    QFileInfo f( filename );
    Plugin::g_preferences->m_lastOpenFilePath = f.absoluteFilePath();

    if( hasButton(f.completeBaseName()) )
    {
        QMessageBox::information(this, IDC_WIN_TITEL, tr("The script file already exists:%1").arg(f.fileName()), 0);
        return;
    }
    QToolButton *btn = addButton( f.completeBaseName(), Plugin::g_preferences->m_scriptButtonIcon, category);
    if( btn == NULL )
    {
        QMessageBox::information(this, IDC_WIN_TITEL, tr("The script file already exists:%1").arg(f.fileName()), 0);
        return;
    }
    emit addScript(btn, filename);
}

void TableWidget::moveScriptAct(int id)
{
    QTableWidgetItem *curitem = this->currentItem();
    if( !curitem )
    {
        return;
    }

    int r = this->row(curitem);
    if( r == -1 )
        return;
    QToolButton *btn = itemButton(r);
    if( !btn )
        return;
    QString filename = item(r, 0)->data(Qt::UserRole).toStringList().at(0);
    QString category = item(r, 0)->data(Qt::UserRole).toStringList().at(2);

    emit moveScript(id, filename, category);

    this->removeRow(r);
}

/**
 * @删除一个脚本文件
 * @brief TableWidget::removeScript
 */
void TableWidget::removeScriptAct()
{
    QTableWidgetItem *curitem = this->currentItem();
    if( !curitem )
    {
        QMessageBox::information(this, IDC_WIN_TITEL, tr("Select a script please.\n(Button is selected by a green border. try to right-clicked tow times)"), 0);
        return;
    }
    int r = this->row(curitem);
    if( r == -1 )
        return;

    QToolButton *btn = itemButton(r);
    if( !btn )
        return;

    QString filename = item(r, 0)->data(Qt::UserRole).toStringList().at(0);
    QString category = item(r, 0)->data(Qt::UserRole).toStringList().at(2);

    QString type = category;
    QStringList titles = Plugin::g_preferences->toolboxTitles();
    QStringList categorys = Plugin::g_preferences->toolboxCategorys();
    int index = categorys.indexOf( category );
    if( index != -1 )
        type = titles.at(index);

    QMessageBox msgBox;
    //msgBox.setParent(this);//设置父窗口,这里就不继续执行下去了.不知道为何.
    msgBox.setWindowTitle(IDC_WIN_TITEL);
    msgBox.setText( tr("Are you sure delete the script?") );
    msgBox.setInformativeText( tr("Click 'Yes' will delete script [%1] from [%2].").arg(filename).arg(type));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret)
    {
    case QMessageBox::Yes:
        disconnect( btn, SIGNAL(clicked()), this, SLOT(buttonClicked()) );
        this->removeCellWidget(r, 0);

        emit removeScript(filename, category);

        this->removeRow(r);
        break;
    case QMessageBox::No:
        break;
    default:
        // should never be reached
        break;
    }
}

/**
 * @查看当前选中的脚本文件属性,并可修改当前脚本的使用方法
 * @brief TableWidget::viewProp
 */
void TableWidget::viewPropAct()
{
    int row = this->currentRow();
    QTableWidgetItem *curitem = this->currentItem();
    if( !curitem )
    {
        QMessageBox::information(this, IDC_WIN_TITEL, tr("Select a script please.\n(Button is selected by a green border)"), 0);
        return;
    }
    QString filename = item(row, 0)->data(Qt::UserRole).toStringList().at(0);
    QString category = item(row, 0)->data(Qt::UserRole).toStringList().at(2);

    emit showProp(filename, category);
}

/**
 * @修改脚本属性后,更新按钮的提示信息
 * @brief TableWidget::updateBtnTooltips
 * @param tooltips
 */
void TableWidget::updateBtnTooltips(const QString &tooltips)
{
    int row = this->currentRow();
    QToolButton* btn = itemButton(row);
    btn->setToolTip(tooltips);
}

/**
   @是否需要添加一个移动菜单功能, 重命名菜单功能?
 * @brief TableWidget::contextMenuEvent
 * @param event
 */
void TableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QSignalMapper* moveActMapper = new QSignalMapper(this);
    QMenu *popMenu = new QMenu(this);
    QAction *addAct = popMenu->addAction(tr("&Add"));
    addAct->setIcon(QIcon(":/images/script_add.png"));
    connect(addAct, SIGNAL( triggered() ), this, SLOT( addScriptAct() ));
    popMenu->addSeparator();

    // add category
    QAction *addCategoryAct = popMenu->addAction(tr("Add &Category"));
    addCategoryAct->setIcon(QIcon(":/images/category_add.png"));
    connect(addCategoryAct, SIGNAL( triggered() ), this, SIGNAL( addCategory() ));
    // delete category
    QAction *delCategoryAct = popMenu->addAction(tr("D&el Category"));
    delCategoryAct->setIcon(QIcon(":/images/category_del.png"));
    connect(delCategoryAct, SIGNAL( triggered() ), this, SIGNAL( delCategory() ));
    popMenu->addSeparator();

    // move script to
    QMenu* moveActMenu = popMenu->addMenu(QIcon(":/images/script_move.png"), tr("&Move To"));
    QList<QAction*> actions;
    int count = Plugin::g_preferences->toolboxCategorys().count();
    int idx = Plugin::g_scriptsWidget->toolbox()->currentIndex();
    for(int i = 0; i < count; ++i)
    {
        QString icon = Plugin::g_preferences->toolboxIcons().at(i);
        QString category =  Plugin::g_preferences->toolboxTitles().at(i);

        QAction* moveAct = new QAction(QIcon(icon), category, this);
        connect( moveAct, SIGNAL( triggered() ), moveActMapper, SLOT( map() ) );
        moveActMapper->setMapping(moveAct, i);
        actions.append(moveAct);
        if( i == idx )
            moveAct->setDisabled(true);
    }
    connect(moveActMapper, SIGNAL(mapped(int)), this, SLOT(moveScriptAct(int)));
    moveActMenu->addActions( actions );

    QAction *removeAct = popMenu->addAction(tr("&Remove"));
    removeAct->setIcon(QIcon(":/images/script_remove.png"));
    popMenu->addSeparator();

    QAction* propAct = popMenu->addAction(tr("&Propertise"));
    propAct->setIcon(QIcon(":/images/script_property.png"));

    int r = this->rowAt( mapFromGlobal(event->globalPos()).y() );
    if( r != -1 )
    {
        this->selectRow(r);
        connect(removeAct, SIGNAL( triggered() ), this, SLOT( removeScriptAct() ));
        connect(propAct, SIGNAL( triggered() ), this, SLOT( viewPropAct() ));
    }
    else
    {
        this->clearFocus();
        this->clearSelection();
        removeAct->setEnabled(false);
        propAct->setEnabled(false);
        moveActMenu->setEnabled(false);
    }

    // 当前操作所在的分类是否是自定义分类,如果不是,则删除分类菜单不可用
    int id = Plugin::g_scriptsWidget->toolbox()->currentIndex();
    QString category = Plugin::g_preferences->m_toolboxCategorys.at(id);
    bool ok = Plugin::g_preferences->m_customCategorys.contains(category);
    if(!ok)
    {
        delCategoryAct->setEnabled(false);
    }
    popMenu->exec(event->globalPos());
    event->accept();
}



















