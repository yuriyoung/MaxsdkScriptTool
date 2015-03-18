#include "xmlwriter.h"
#include "constants.h"

#include <QTreeWidget>

XmlWriter::XmlWriter(QTreeWidget *treeWidget) :
    m_treeWidget(treeWidget)
{
    m_writer.setAutoFormatting( true );
}

bool XmlWriter::write(QIODevice *device)
{
    m_writer.setDevice(device);

    m_writer.writeStartDocument();
    m_writer.writeDTD( XML_DTD );
    m_writer.writeStartElement( XML_START_TAG );
    m_writer.writeAttribute( XML_START_VERSION, XML_START_VERSION_VALUE);
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); ++i)
        writeItem(m_treeWidget->topLevelItem(i));

    m_writer.writeEndDocument();

    return true;
}

void XmlWriter::writeItem(QTreeWidgetItem *item)
{
    QString tagName = item->data(0, Qt::UserRole).toStringList().at(0);
    if ( tagName == XML_TAG_CATEGORY )
    {
        bool expanded = m_treeWidget->isItemExpanded(item);
        m_writer.writeStartElement(tagName);
        m_writer.writeAttribute( XML_CATEGORY_ATTR_NAME, item->text(0));
        m_writer.writeAttribute( XML_CATEGORY_ATTR_EXPAND, expanded ? "yes" : "no");
        for (int i = 0; i < item->childCount(); ++i)
            writeItem(item->child(i));
        m_writer.writeEndElement();
    }
    else if ( tagName == XML_TAG_FILE )
    {
        m_writer.writeStartElement(tagName);
        m_writer.writeAttribute( XML_FILE_ATTR_TITLE, item->text(0));
        m_writer.writeAttribute( XML_ARCHIVE_NAME, item->data(0, Qt::UserRole).toStringList().at(1) );
        m_writer.writeTextElement( XML_TAG_PROP, item->text(1));
        m_writer.writeEndElement();
    }
    else
    {
        m_writer.writeEmptyElement(tagName);
    }
}
