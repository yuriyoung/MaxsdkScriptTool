#ifndef SCRIPT_THREAD_H
#define SCRIPT_THREAD_H

#include <QThread>
#include <QTreeWidgetItem>

class ScriptThread : public QThread
{
    Q_OBJECT
public:
    explicit ScriptThread(QObject *parent = 0);
    void completedInit();

signals:
    void addParent(QTreeWidgetItem *item);
    void addChild(QTreeWidgetItem *item);
    void runScript(const QString &data);

public slots:
    void scriptDoubleClicked(QTreeWidgetItem *item, int column);

protected:
    void run();

private:
    void listArchiveFiles();
    QString parseFile(QTreeWidgetItem *item);

    bool            m_initialized;
    bool            m_runScript;
};

#endif // SCRIPT_THREAD_H
