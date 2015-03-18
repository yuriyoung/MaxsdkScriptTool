#ifndef TREEVIEW_WIDGET_H
#define TREEVIEW_WIDGET_H

#include <QTreeWidget>
#include "xmlreader.h"
#include "xmlwriter.h"

class QAction;
class QMenu;
class QContextMenuEvent;

class TreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit TreeWidget(QWidget *parent = 0);
    ~TreeWidget();

    bool loadFile(const QString &xmlFile);
    bool saveFile(const QString &xmlFile);

signals:
    
private slots:
    void addCategory();
    void deleteCategory();
    void addFile();
    void deleteFile();

    void runScript(QTreeWidgetItem *item, int column);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    void listArchiveFiles();
    QString parseFile(QTreeWidgetItem *item);

    XmlWriter       *m_writer;
    XmlReader       *m_reader;
};

#endif // TREEVIEW_WIDGET_H
