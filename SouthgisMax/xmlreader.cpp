#include "xmlreader.h"
#include "constants.h"

#include <QDebug>

XmlReader::XmlReader(QTreeWidget *treeWidget)
    :m_treeWidget(treeWidget)
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
            readCategory( 0 );
        else if (m_reader.name() == XML_TAG_FILE)
            readFile( 0 );
        else
            m_reader.skipCurrentElement();
    }
}

void XmlReader::readCategory(QTreeWidgetItem *parent)
{
    Q_ASSERT(m_reader.isStartElement() && m_reader.name() == XML_TAG_CATEGORY);

    QTreeWidgetItem *categroy = createChildItem(parent);
    categroy->setFlags(categroy->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable);
    categroy->setText(0, m_reader.attributes().value( XML_CATEGORY_ATTR_NAME ).toString());
    bool expand = (m_reader.attributes().value( XML_CATEGORY_ATTR_EXPAND ) != "no");
    m_treeWidget->setItemExpanded(categroy, expand);

    while (m_reader.readNextStartElement())
    {
        if ( m_reader.name() == XML_TAG_CATEGORY )
            readCategory(categroy);
        else if ( m_reader.name() == XML_TAG_FILE )
            readFile(categroy);
        else
            m_reader.skipCurrentElement();
    }
}

void XmlReader::readFile(QTreeWidgetItem *parent)
{
    Q_ASSERT(m_reader.isStartElement() && m_reader.name() == XML_TAG_FILE);

    QTreeWidgetItem *file = createChildItem(parent);
    file->setFlags(parent->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
    file->setFlags(parent->flags() & ~Qt::ItemIsDropEnabled );
    file->setText(0, m_reader.attributes().value( XML_FILE_ATTR_TITLE ).toString());

    while (m_reader.readNextStartElement())
    {
        if ( m_reader.name() == XML_TAG_PROP )
            readProp(file);
        else
            m_reader.skipCurrentElement();
    }
}

void XmlReader::readProp(QTreeWidgetItem *item)
{
    Q_ASSERT(m_reader.isStartElement() && m_reader.name() == XML_TAG_PROP);

    QString tooltip = m_reader.readElementText();
    item->setText(1, tooltip);
    item->setToolTip(0, tooltip);
}

QString XmlReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(m_reader.errorString())
            .arg(m_reader.lineNumber())
            .arg(m_reader.columnNumber());
}

void XmlReader::skipUnknownElement()
{
    m_reader.readNext();
    while (!m_reader.atEnd())
    {
        if (m_reader.isEndElement())
        {
            m_reader.readNext();
            break;
        }

        if (m_reader.isStartElement())
        {
            skipUnknownElement();
        }
        else
        {
            m_reader.readNext();
        }
    }
}

QTreeWidgetItem *XmlReader::createChildItem(QTreeWidgetItem *parent)
{
    QTreeWidgetItem *childItem;
    if (parent)
    {
        childItem = new QTreeWidgetItem(parent);
    }
    else
    {
        childItem = new QTreeWidgetItem(m_treeWidget);
    }
    QStringList list;
    list.append(m_reader.name().toString());
    list.append(m_reader.attributes().value( XML_ARCHIVE_NAME ).toString());
    childItem->setData(0, Qt::UserRole, list);

    return childItem;
}
