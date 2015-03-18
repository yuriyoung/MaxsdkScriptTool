#include "sqlreader.h"
#include "constants.h"
#include "plugin.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QMessageBox>
#include <QFile>

#include <QDebug>

SqlReader::SqlReader(QSqlDatabase &db, QObject *parent)
    : m_db(db), QObject(parent)
{
    if( createConnect() )
    {
        createTable();
    }
}

SqlReader::~SqlReader()
{
    close();
}

/*
 * 连接数据库表
 */
bool SqlReader::createConnect()
{
    if(QSqlDatabase::contains("SQLREADER"))
        m_db = QSqlDatabase::database("SQLREADER");
    else
    {
        m_db = QSqlDatabase::addDatabase("SQLITECIPHER", "SQLREADER");
        m_db.setDatabaseName( Plugin::dataPath() + "/" + SQL_BD_NAME );
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

/*
 * 创建数据库表
 */
bool SqlReader::createTable()
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
            qDebug()<< "Create table error: " << query.lastError();
            return false;
        }
    }

    return true;
}

/*
 * 读取数据数库的文件
 * 参数1:指定的文件名,参数2:指定文件存在类型,即所在的表
 */
QByteArray SqlReader::readFile(const QString &filename, const QString &category)
{
    QSqlQuery query(m_db);

    QString select = QString("SELECT %1 FROM %2 WHERE %3=? AND %4=?")\
            .arg(SQL_FIELD_CONTENT)\
            .arg(SQL_TABLE_FILES)\
            .arg(SQL_FIELD_CATEGORY)\
            .arg(SQL_FIELD_NAME);
    query.prepare( select );
    query.addBindValue( category );
    query.addBindValue( filename );
    query.exec();
    query.next();
    QByteArray tdata = query.value(0).toByteArray();
    QByteArray data = qUncompress(tdata);

    return data;
}

/*
 * 读取数据库的文件
 * 参数1:指定的文件名列表,参数2:指定文件存在类型,即所在的表
 */
QList<QByteArray> SqlReader::readFiles(const QStringList &files, const QString &category)
{
    QList<QByteArray> dataList;
    foreach( QString filename, files )
    {
        QByteArray data = readFile(filename, category);
        dataList.append( data );
    }

    return dataList;
}

/*
 * 读取数据库对应文件的图标
 * 参数1:指定的文件名列表,参数2:指定文件存在类型,即所在的表
 */
QString SqlReader::selectIcon(const QString &filename, const QString &category)
{
    QSqlQuery query(m_db);
    QString select = QString("SELECT %1 FROM %2 WHERE %3=?")\
            .arg(SQL_FIELD_ICON)\
            .arg(SQL_TABLE_FILES)\
            .arg(SQL_FIELD_NAME);
    query.prepare( select );
    query.addBindValue( category );
    query.addBindValue( filename );
    query.exec();
    query.next();
    QString icon = query.value(0).toString();

    return icon;
}

/*
 * 读取数据库对应文件的图标
 * 参数:指定文件存在类型,即所在的表
 */
QStringList SqlReader::selectIcons(const QString &category)
{
    QStringList list;
    QSqlQuery query(m_db);
    QString select = QString("SELECT %1 FROM %2 WHERE %3!=''")\
            .arg(SQL_FIELD_ICON)\
            .arg(SQL_TABLE_FILES)\
            .arg(SQL_FIELD_NAME);
    query.prepare( select );
    query.exec();
    while( query.next() )
    {
        QString icon = query.value(0).toString();
        list.append(icon);
    }

    return list;
}

QString SqlReader::selectUsage(const QString &filename, const QString &category)
{
    QString usag;
    QSqlQuery query(m_db);
    QString select = QString("SELECT %1 FROM %2 WHERE %3=? AND %4=?")\
            .arg(SQL_FIELD_USAGE)\
            .arg(SQL_TABLE_FILES)\
            .arg(SQL_FIELD_CATEGORY)\
            .arg(SQL_FIELD_NAME);
    query.prepare( select );
    query.addBindValue( category );
    query.addBindValue( filename );
    query.exec();
    query.next();
    usag = query.value(0).toString();

    return usag;
}

QString SqlReader::selectDate(const QString &filename, const QString &category)
{
    QString date;
    QSqlQuery query(m_db);
    QString select = QString("SELECT %1 FROM %2 WHERE %3=? AND %4=?")\
            .arg(SQL_FIELD_DATE)\
            .arg(SQL_TABLE_FILES)\
            .arg(SQL_FIELD_CATEGORY)\
            .arg(SQL_FIELD_NAME);
    query.prepare( select );
    query.addBindValue( category );
    query.addBindValue( filename );
    query.exec();
    query.next();
    date = query.value(0).toString();

    return date;
}

QString SqlReader::selectSize(const QString &filename, const QString &category)
{
    QString size;
    QSqlQuery query(m_db);
    QString select = QString("SELECT %1 FROM %2 WHERE %3=? AND %4=?")\
            .arg(SQL_FIELD_SIZE)\
            .arg(SQL_TABLE_FILES)\
            .arg(SQL_FIELD_CATEGORY)\
            .arg(SQL_FIELD_NAME);
    query.prepare( select );
    query.addBindValue( category );
    query.addBindValue( filename );
    query.exec();
    query.next();
    size = query.value(0).toString();

    return size;
}

QString SqlReader::selectType(const QString &filename, const QString &category)
{
    QString type;
    QSqlQuery query(m_db);
    QString select = QString("SELECT %1 FROM %2 WHERE %3=? AND %4=?")\
            .arg(SQL_FIELD_TYPE)\
            .arg(SQL_TABLE_FILES)\
            .arg(SQL_FIELD_CATEGORY)\
            .arg(SQL_FIELD_NAME);
    query.prepare( select );
    query.addBindValue( category );
    query.addBindValue( filename );
    query.exec();
    query.next();
    type = query.value(0).toString();

    return type;
}

QString SqlReader::selectVersion(const QString &filename, const QString &category)
{
    QString version = "1.0";
    QSqlQuery query(m_db);
    QString select = QString("SELECT %1 FROM %2 WHERE %3=? AND %4=?")\
            .arg(SQL_FIELD_VERSION)\
            .arg(SQL_TABLE_FILES)\
            .arg(SQL_FIELD_CATEGORY)\
            .arg(SQL_FIELD_NAME);
    query.prepare( select );
    query.addBindValue( category );
    query.addBindValue( filename );
    query.exec();
    query.next();
    version = query.value(0).toString();

    return version;
}

bool SqlReader::close()
{
    m_db.close();
    return true;
}

/*
 * 读取数据库的文件
 * 参数:指定文件存在类型,即所在的表
 */
QStringList SqlReader::selectFilenames(const QString &category)
{
    QStringList list;
    QSqlQuery query(m_db);
    QString select = QString("SELECT %1 FROM %2 WHERE %3=?")\
            .arg(SQL_FIELD_NAME)\
            .arg(SQL_TABLE_FILES)\
            .arg(SQL_FIELD_CATEGORY);
    query.prepare( select );
    query.addBindValue( category );
    query.exec();
    while( query.next() )
    {
        QString filename = query.value(0).toString();
        list.append(filename);
    }

    return list;
}


