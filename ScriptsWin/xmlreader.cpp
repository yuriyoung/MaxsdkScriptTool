#include "xmlreader.h"
#include "constants.h"

#include <QDebug>

XmlReader::XmlReader(QToolBox *toolbox)
    :m_toolbox(toolbox)
{
}

bool XmlReader::read(QIODevice *device)
{
    m_reader.setDevice(device);

    if (m_reader.readNextStartElement())
    {
        if (m_reader.name() == XML_START_TAG &&
                m_reader.attributes().value( XML_START_VERSION ) == XML_START_VERSION_VALUE)
            readXml();
        else
            m_reader.raiseError(QObject::tr("The file is not an xml version %1 file.").arg(XML_START_VERSION_VALUE));
    }

    return !m_reader.error();
}

void XmlReader::readXml()
{
    Q_ASSERT(m_reader.isStartElement() && m_reader.name() == XML_START_TAG);

    while (m_reader.readNextStartElement())
    {
        if (m_reader.name() == XML_TAG_CATEGORY)
            readCategoryPage( );
//        else if (m_reader.name() == XML_TAG_FILE)
//            readFileItem();
        else
            m_reader.skipCurrentElement();
    }
}

void XmlReader::readCategoryPage()
{
    Q_ASSERT(m_reader.isStartElement() && m_reader.name() == XML_TAG_CATEGORY);

    // TODO: 这里只做添加ToolBox页
    QString categoryTitle = m_reader.attributes().value( XML_CATEGORY_ATTR_NAME ).toString();
    QString cateoryIcon = m_reader.attributes().value( XML_CATEGORY_ATTR_ICON ).toString();

    TableWidget *tablePage = new TableWidget(m_toolbox);
    tablePage->setObjectName(categoryTitle);
    m_toolbox->addItem(tablePage, QIcon(cateoryIcon), categoryTitle);

    while (m_reader.readNextStartElement())
    {
        if ( m_reader.name() == XML_TAG_CATEGORY )
            readCategoryPage();
        else if ( m_reader.name() == XML_TAG_FILE )
            readFileItem(tablePage);
        else
            m_reader.skipCurrentElement();
    }
}

void XmlReader::readFileItem(TableWidget *tablePage)
{
    Q_ASSERT(m_reader.isStartElement() && m_reader.name() == XML_TAG_FILE);

    QToolButton* toolbtn = createItemBtn(tablePage);

    while (m_reader.readNextStartElement())
    {
        if ( m_reader.name() == XML_TAG_PROP )
            readProp(toolbtn);
        else
            m_reader.skipCurrentElement();
    }
}

void XmlReader::readProp(QToolButton* toolbtn)
{
    Q_ASSERT(m_reader.isStartElement() && m_reader.name() == XML_TAG_PROP);

    QString tooltip = m_reader.readElementText();
    QStringList list = tooltip.split("|");
    QString tooltips;
    for(int i = 0; i < list.count(); ++i)
    {
        tooltips.append( list.at(i) );
        if( i < list.count()-1 )
            tooltips.append( "\n" );
    }
    toolbtn->setToolTip( tooltips );
}

QString XmlReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(m_reader.errorString())
            .arg(m_reader.lineNumber())
            .arg(m_reader.columnNumber());
}

// 建立Button的同时,将属性数据设置在tablewidgetitem上.
QToolButton* XmlReader::createItemBtn(TableWidget *tablePage)
{
    QString title = m_reader.attributes().value( XML_FILE_TITLE ).toString();
    QString icon = m_reader.attributes().value( XML_FILE_ICON ).toString();
    QString archive = m_reader.attributes().value( XML_FILE_ARCHIVE ).toString();

    QToolButton* toolbtn = tablePage->addButton( title, icon, archive );

    return toolbtn;
}
