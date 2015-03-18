#include "script_thread.h"
#include "constants.h"
#include "plugin.h"
#include "archivereader.h"

#include <QDir>
#include <QMessageBox>
#include <QFile>

ScriptThread::ScriptThread(QObject *parent) :
    QThread(parent)
{
    m_initialized = false;
    m_runScript = false;
}

void ScriptThread::completedInit()
{
    m_initialized = true;
}

void ScriptThread::run()
{
    if( m_initialized == false)
        listArchiveFiles();
}

void ScriptThread::listArchiveFiles()
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
        emit addParent(parent);

        foreach( QString f, files )
        {
            QTreeWidgetItem *subItem = new QTreeWidgetItem(parent);
            subItem->setFlags(parent->flags() & ~Qt::ItemIsDropEnabled );
            subItem->setText(0, f);
            QStringList list;
            list.append(XML_TAG_FILE);
            list.append(archive.filePath());
            subItem->setData(0, Qt::UserRole, list);
            emit addChild(subItem);
        }
    }

    m_initialized = true;
}

QString ScriptThread::parseFile(QTreeWidgetItem *item)
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

void ScriptThread::scriptDoubleClicked(QTreeWidgetItem *item, int column)
{
    if( !item ) return;

    QString tagName = item->data(0, Qt::UserRole).toStringList().at(0);
    if ( tagName == XML_TAG_CATEGORY )
        return;

    QString data = parseFile(item);

    if( data.isEmpty() )
        return;

    emit runScript(data);
//    QMessageBox::information(0, item->text(0), data, 0);
//    int nSize = data.size();
//    char *pData = new char(nSize);
//    memcpy(pData, data.data(), nSize);
//    ScriptGup gup;
//    bool ok = gup.ExecuteStringScript(pData);
//    if( !ok )
//    {
//        gup.ExecuteStringScript("actionMan.executeAction 0 \"40472\"");
//    }
}
