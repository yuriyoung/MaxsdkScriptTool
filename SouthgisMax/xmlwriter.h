#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QXmlStreamWriter>

class QTreeWidget;
class QTreeWidgetItem;

class XmlWriter
{
public:
    explicit XmlWriter( QTreeWidget *treeWidget = 0 );
    
    bool write(QIODevice *device);

signals:
    
public slots:
    
private:
    void writeItem(QTreeWidgetItem *item);

    QXmlStreamWriter     m_writer;
    QTreeWidget         *m_treeWidget;
};

#endif // XMLWRITER_H
