#include "xmlwriter.h"
#include "constants.h"
#include "table_widget.h"
#include <QToolBox>

#include <QDebug>

XmlWriter::XmlWriter(QToolBox *toolbox) :
    m_toolbox(toolbox)
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

    for(int i = 0; i < m_toolbox->count(); ++i)
        writeItem( i );

    m_writer.writeEndDocument();

    return true;
}

void XmlWriter::writeItem( int index )
{
    int iconIdx = index;
    if( iconIdx >= PageIcons.count() )
        iconIdx = (int)index % PageIcons.count();

    m_writer.writeStartElement(XML_TAG_CATEGORY);
    m_writer.writeAttribute( XML_CATEGORY_ATTR_NAME, m_toolbox->itemText( index ) );
    m_writer.writeAttribute( XML_CATEGORY_ATTR_ICON, PageIcons.at(iconIdx) );


    QWidget *widget = m_toolbox->widget( index );
    TableWidget *tableWidget = qobject_cast<TableWidget*>(widget);
    if( !tableWidget )
        return;
    int count = tableWidget->rowCount();
    for(int j = 0; j < count; ++j)
    {
        QToolButton* toolbtn = tableWidget->itemButton(j);
        QTableWidgetItem *item = tableWidget->item( j, 0 );

        m_writer.writeStartElement(XML_TAG_FILE);
        m_writer.writeAttribute( XML_FILE_TITLE, item->data(Qt::UserRole).toStringList().at(0) );
        m_writer.writeAttribute( XML_FILE_ICON, item->data(Qt::UserRole).toStringList().at(1) );
        m_writer.writeAttribute( XML_FILE_ARCHIVE, item->data(Qt::UserRole).toStringList().at(2) );

        QString tooltips = toolbtn->toolTip();
        tooltips.replace( QString("\n"), QString("|") );
        m_writer.writeTextElement( XML_TAG_PROP, tooltips );
        m_writer.writeEndElement();
    }
    m_writer.writeEndElement();
}
