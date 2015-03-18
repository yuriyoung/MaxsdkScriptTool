#ifndef XMLREADER_H
#define XMLREADER_H

#include <QTreeWidget>
#include <QXmlStreamReader>

class XmlReader
{
public:
    explicit XmlReader(QTreeWidget *treeWidget = 0);
    
    bool read(QIODevice *device);
    QString errorString() const;

signals:
    
public slots:
    
private:
     void readXml();
     void readCategory(QTreeWidgetItem *parent);
     void readFile(QTreeWidgetItem *parent);
     void readProp(QTreeWidgetItem *item);

     void skipUnknownElement();

     QTreeWidgetItem *createChildItem(QTreeWidgetItem *parent);

    QXmlStreamReader m_reader;
    QTreeWidget     *m_treeWidget;
};

#endif // XMLREADER_H
