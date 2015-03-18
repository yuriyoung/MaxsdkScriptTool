#ifndef XMLREADER_H
#define XMLREADER_H

#include "table_widget.h"
#include <QToolBox>
#include <QXmlStreamReader>

class XmlReader
{
public:
    explicit XmlReader( QToolBox *toolbox = 0);
    
    bool read(QIODevice *device);
    QString errorString() const;

signals:
    
public slots:
    
private:
     void readXml();
     void readCategoryPage();
     void readFileItem(TableWidget *tablePage);
     void readProp(QToolButton *toolbtn);

     QToolButton* createItemBtn(TableWidget *tablePage);

    QXmlStreamReader m_reader;
    QToolBox        *m_toolbox;
};

#endif // XMLREADER_H
