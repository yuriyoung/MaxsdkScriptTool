#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QXmlStreamWriter>

class QToolBox;

class XmlWriter
{
public:
    explicit XmlWriter( QToolBox *toolbox = 0 );
    
    bool write(QIODevice *device);

signals:
    
public slots:
    
private:
    void writeItem(int index);

    QXmlStreamWriter     m_writer;
    QToolBox            *m_toolbox;
};

#endif // XMLWRITER_H
