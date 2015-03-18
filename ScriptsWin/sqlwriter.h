#ifndef SQLWRITER_H
#define SQLWRITER_H

#include <QSqlDatabase>
#include <QStringList>
#include <QThread>
#include <QTimer>

class SqlWriter : public QObject
{
    Q_OBJECT
public:
    SqlWriter(QSqlDatabase &db, QObject *parent = 0);
    ~SqlWriter();

    bool        storeFile(const QString &filename, const QString &category);
    bool        storeFile(const QString &filename, const QString &category, const QString &version);
    bool        storeFiles(const QStringList &files, const QStringList &category);
    bool        insertUsage(const QString &usage, const QString &category, const QString &filename);
    bool        removeFile(const QString &filename, const QString &category);
    bool        removeCategory(const QString &category);
    bool        close();

    bool        updateFile(const QString &filename, const QString &category, const QString &newCategory);

public slots:
    void writeToFile(const QString &filename, const QString &category, const QString &version, const QString &remark);

private slots:
    void finished();

private:
    bool        createConnect();
    bool        createTable();

    QSqlDatabase        m_db;
    QThread             m_thread;
    QTimer              m_timer;
};

#endif // SQLWRITER_H
