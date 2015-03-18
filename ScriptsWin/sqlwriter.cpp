#include "sqlwriter.h"
#include "constants.h"
#include "plugin.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

#include <QDebug>

SqlWriter::SqlWriter(QSqlDatabase &db, QObject *parent)
    : m_db(db), QObject(parent)
{
    moveToThread(&m_thread);
    m_thread.start();

    if( createConnect() )
        createTable();

    m_timer.setInterval(10000); // 10s
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(finished()));
}

SqlWriter::~SqlWriter()
{
    if(m_thread.isRunning())
    {
        m_thread.quit();
        m_thread.wait();
    }
    close();
}

bool SqlWriter::createConnect()
{
    if(QSqlDatabase::contains("SQLWRITER"))
        m_db = QSqlDatabase::database("SQLWRITER");
    else
    {
        m_db = QSqlDatabase::addDatabase("SQLITECIPHER", "SQLWRITER");
        m_db.setDatabaseName(Plugin::dataPath() + "/" + SQL_BD_NAME);
        m_db.setPassword(QString(PassWord));
    }

    if( !m_db.isOpen() )
    {
        if ( !m_db.open() )
        {
            qDebug()<< QString(" can't open database >>>>>> %1").arg(SQL_BD_NAME);
            qDebug() << "error code: " << m_db.lastError();
            QMessageBox::warning(0, IDC_WIN_TITEL, QObject::tr("Read data error:\nError: %1").arg(m_db.lastError().databaseText()));
            return false;
        }
    }
    return true;
}

bool SqlWriter::createTable()
{
    QStringList tableList = m_db.tables();
    QSqlQuery query(m_db);
    if( !tableList.contains( SQL_TABLE_FILES ) )
    {
        QString createTable = QString("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                      "%2 VARCHAR(128), %3 VARCHAR(128), %4 BLOB,"
                                      "%5 VARCHAR(24), %6 VARCHAR(64), %7 VARCHAR(24),"
                                      "%8 VARCHAR(24), %9 VARCHAR(256) )")\
                .arg(SQL_TABLE_FILES)   // 表名 1
                .arg(SQL_FIELD_CATEGORY)// 类别名 2
                .arg(SQL_FIELD_NAME)    // 文件名 3
                .arg(SQL_FIELD_CONTENT) // 文件内容 4
                .arg(SQL_FIELD_DATE)    // 最后修改日期 5
                .arg(SQL_FIELD_SIZE)    // 文件大小 6
                .arg(SQL_FIELD_TYPE)    // 文件类型 7
                .arg(SQL_FIELD_VERSION) // 文件类型 8
                .arg(SQL_FIELD_USAGE);  // 文件用法 9
        if(!query.exec(createTable))
        {
            QMessageBox::warning(0, QObject::tr("Create table error"), QObject::tr("Error: ")+ query.lastError().text(), QMessageBox::Ok);
            qDebug() << "Create table error: " << query.lastError();
            return false;
        }
    }

    return true;
}

bool SqlWriter::storeFile(const QString &filename, const QString &category)
{
    QSqlQuery query(m_db);

    QFile file(filename);
    if( !file.open(QIODevice::ReadOnly) )
        return false;
    qint64 _size = file.size();
    QString sizeStr = Plugin::formatSize(_size);
    QByteArray &tdata = file.readAll();
    QByteArray data = qCompress(tdata, 9);

    QFileInfo fi(filename);
    QString time = fi.lastModified().toString("yyyy/MM.dd hh:mm:ss");

    QString insert = QString("INSERT INTO %1 (id, %2, %3, %4, %5, %6, %7, %8) "
                             "VALUES (NULL, ?, ?, ?, ?, ?, ?, ?)")\
            .arg(SQL_TABLE_FILES)      //1
            .arg(SQL_FIELD_CATEGORY)   //2
            .arg(SQL_FIELD_NAME)       //3
            .arg(SQL_FIELD_CONTENT)    //4
            .arg(SQL_FIELD_DATE)       //5
            .arg(SQL_FIELD_SIZE)       //6
            .arg(SQL_FIELD_TYPE)       //7
            .arg(SQL_FIELD_VERSION);   //8
    query.prepare( insert );
    query.addBindValue( category );
    query.addBindValue( fi.completeBaseName() );
    query.addBindValue( data );
    query.addBindValue( time );
    query.addBindValue( sizeStr );
    query.addBindValue( fi.suffix() );
    query.addBindValue( QString("1.0") );
    if( !query.exec() )
    {
        qDebug() << query.lastError();
//        QMessageBox::information(0, IDC_WIN_TITEL, query.lastError().text() + "\n" + filename, 0);
        return false;
    }

    return true;
}

bool SqlWriter::storeFile(const QString &filename, const QString &category, const QString &version)
{
    QSqlQuery query(m_db);

    QFile file(filename);
    if( !file.open(QIODevice::ReadOnly) )
        return false;
    qint64 _size = file.size();
    QString sizeStr = Plugin::formatSize(_size);
    QByteArray &tdata = file.readAll();
    QByteArray data = qCompress(tdata, 9);

    QFileInfo fi(filename);
    QString time = fi.lastModified().toString("yyyy/MM.dd hh:mm:ss");

    QString insert = QString("INSERT INTO %1 (id, %2, %3, %4, %5, %6, %7, %8) "
                             "VALUES (NULL, ?, ?, ?, ?, ?, ?, ?)")\
            .arg(SQL_TABLE_FILES)      //1
            .arg(SQL_FIELD_CATEGORY)   //2
            .arg(SQL_FIELD_NAME)       //3
            .arg(SQL_FIELD_CONTENT)    //4
            .arg(SQL_FIELD_DATE)       //5
            .arg(SQL_FIELD_SIZE)       //6
            .arg(SQL_FIELD_TYPE)       //7
            .arg(SQL_FIELD_VERSION);   //8
    query.prepare( insert );
    query.addBindValue( category );
    query.addBindValue( fi.completeBaseName() );
    query.addBindValue( data );
    query.addBindValue( time );
    query.addBindValue( sizeStr );
    query.addBindValue( fi.suffix() );
    query.addBindValue( version );
    if( !query.exec() )
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

bool SqlWriter::storeFiles(const QStringList &files, const QStringList &category)
{
    int i = 0;
    foreach( QString filename, files )
    {
        storeFile(filename, category.at(i));
        i+=1;
    }

    return true;
}

bool SqlWriter::insertUsage(const QString &setUsage, const QString &category, const QString &filename)
{
    QString str = QString("UPDATE %1 SET %2=? WHERE %3=? AND %4=?").arg(SQL_TABLE_FILES).arg(SQL_FIELD_USAGE)
            .arg(SQL_FIELD_CATEGORY).arg(SQL_FIELD_NAME);
    QSqlQuery query(m_db);
    query.prepare(str);
    query.addBindValue(setUsage, QSql::In);
    query.addBindValue(category, QSql::Out);
    query.addBindValue(filename, QSql::Out);
    if( ! query.exec() )
        return false;

    return true;
}

bool SqlWriter::removeFile(const QString &filename, const QString &category)
{
    QString str = QString("DELETE FROM %1 WHERE %2=? AND %3=?")
                        .arg(SQL_TABLE_FILES).arg(SQL_FIELD_CATEGORY).arg(SQL_FIELD_NAME);
    QSqlQuery query(m_db);
    query.prepare(str);
    query.addBindValue(category, QSql::Out);
    query.addBindValue(filename, QSql::Out);
    if( ! query.exec() )
        return false;

    return true;
}

bool SqlWriter::removeCategory(const QString &category)
{
    QString str = QString("DELETE FROM %1 WHERE %2=?")
                        .arg(SQL_TABLE_FILES).arg(SQL_FIELD_CATEGORY);
    QSqlQuery query(m_db);
    query.prepare(str);
    query.addBindValue(category, QSql::Out);
    if( ! query.exec() )
        return false;

    return true;
}

bool SqlWriter::close()
{
    m_db.close();
    return true;
}

bool SqlWriter::updateFile(const QString &filename, const QString &category , const QString &newCategory)
{
    QString str = QString("UPDATE %1 SET %2=? WHERE %3=? AND %4=?").arg(SQL_TABLE_FILES).arg(SQL_FIELD_CATEGORY)
            .arg(SQL_FIELD_CATEGORY).arg(SQL_FIELD_NAME);
    QSqlQuery query(m_db);
    query.prepare(str);
    query.addBindValue(newCategory, QSql::In);
    query.addBindValue(category, QSql::Out);
    query.addBindValue(filename, QSql::Out);
    if( ! query.exec() )
        return false;

    return true;
}

void SqlWriter::finished()
{
    if(m_thread.isFinished() /*|| m_thread.isRunning()*/)
    {
        m_thread.quit();
        m_thread.wait();
    }
}

void SqlWriter::writeToFile(const QString &filename, const QString &category, const QString &version, const QString &remark)
{
    if( !m_thread.isRunning() )
    {
        m_thread.start();
//        m_timer.start();
    }
    storeFile(filename, category, version);
    QFileInfo f( filename );
    QString basename = f.completeBaseName();
    insertUsage(remark, category, basename);
}
