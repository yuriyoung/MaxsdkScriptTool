#ifndef SQLREADER_H
#define SQLREADER_H

#include <QObject>
#include <QSqlDatabase>
#include <QFile>
#include <QStringList>

class SqlReader : public QObject
{
public:
    explicit SqlReader(QSqlDatabase &db, QObject* parent = 0);
    ~SqlReader();

//    void setSelectField(const QString &filename, const QString &category);

    QByteArray          readFile(const QString &filename, const QString &category);
    QList<QByteArray>   readFiles(const QStringList &files, const QString &category);

    QString             selectIcon(const QString &filename, const QString &category);
    QStringList         selectFilenames(const QString &category);
    QStringList         selectIcons(const QString &category);
    QString             selectUsage(const QString &filename, const QString &category);
    QString             selectDate(const QString &filename, const QString &category);
    QString             selectSize(const QString &filename, const QString &category);
    QString             selectType(const QString &filename, const QString &category);
    QString             selectVersion(const QString &filename, const QString &category);

    bool                close();

private:
    bool                createConnect();
    bool                createTable();
    QSqlDatabase        m_db;
};

#endif // SQLREADER_H
