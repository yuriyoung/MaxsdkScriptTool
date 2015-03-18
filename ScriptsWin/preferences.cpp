#include "preferences.h"
#include "plugin.h"

#include <QObject>
#include <QSettings>
#include <QString>
#include <QDir>

Preferences::Preferences()
{
    reset();
    load();
}

Preferences::~Preferences()
{
    save();
}

void Preferences::reset()
{
    m_tbRenameIcon          = ":/images/Rename.png";
    m_tbMaterialIcon        = ":/images/Material.png";
    m_tbObjectEditIcon      = ":/images/ObjectEdit.png";
    m_tbImportExportIcon    = ":/images/ImportExport.png";
    m_tbSceneSheckIcon      = ":/images/SceneCheck.png";
    m_tbOtherIcon           = ":/images/Other.png";
    m_tbCustomIcon          = ":/images/Custom.png";
    m_scriptButtonIcon      = ":/images/Faverit.png";

    m_toolboxCategorys << "Rename" << "Material" << "ObjectEdit"
        <<  "ImportExport" << "SceneCheck" << "Other";
    m_customCategorys.clear();

    m_toolboxTitles << QObject::tr("Rename") << QObject::tr("Material") << QObject::tr("ObjectEdit")
        <<  QObject::tr("ImportExport") << QObject::tr("SceneCheck") << QObject::tr("Other");
    m_customTitles.clear();

    m_toolboxIcons << m_tbRenameIcon << m_tbMaterialIcon << m_tbObjectEditIcon
        << m_tbImportExportIcon << m_tbSceneSheckIcon << m_tbOtherIcon;
    m_customIcons.clear();

    m_lastOpenFilePath      = QDir::homePath();;
    m_dockLocation          = 0;    // 0 float, 1 bottom(disabled), 2 left, 3 right
    m_lastActivePage        = 0;
    m_themeNo               = 1;
}

void Preferences::load()
{
    QSettings *set = Plugin::g_settings;

    set->beginGroup("InternalUse");
    m_lastOpenFilePath = set->value("LastOpenFilePath", m_lastOpenFilePath).toString();
    set->endGroup();

    set->beginGroup("Detail");
    m_dockLocation = set->value("DockLocation", m_dockLocation).toInt();
    m_lastActivePage = set->value("LastActivePage", m_lastActivePage).toInt();
    m_themeNo = set->value("Theme", m_themeNo).toInt();
    set->endGroup();

    set->beginGroup("Categorys");
    m_customCategorys = set->value("Category", m_customCategorys).toStringList();
    m_customTitles = m_customCategorys;
    m_customIcons = set->value("Icon", m_customIcons).toStringList();
    set->endGroup();
}

void Preferences::save()
{
    QSettings *set = Plugin::g_settings;

    set->beginGroup("InternalUse");
    set->setValue("LastOpenFilePath", m_lastOpenFilePath);
    set->endGroup();

    set->beginGroup("Detail");
    set->setValue("DockLocation", m_dockLocation);
    set->setValue("LastActivePage", m_lastActivePage);
    set->setValue("Theme", m_themeNo);
    set->endGroup();

    set->beginGroup("Categorys");
    set->setValue("Category", m_customCategorys);
    set->setValue("Icon", m_customIcons);
    set->endGroup();

}

void Preferences::appendToolboxIcon(const QString &icon)
{
    m_customIcons.append(icon);
}

QStringList Preferences::toolboxIcons()
{
    m_toolboxIcons += m_customIcons;
    return m_toolboxIcons;
}

void Preferences::appendToolboxTitle(const QString &title)
{
    m_customTitles.append(title);
}

QStringList Preferences::toolboxTitles()
{
    m_toolboxTitles += m_customTitles;
    return m_toolboxTitles;
}

void Preferences::appendToolboxCategory(const QString &category)
{
    m_customCategorys.append(category);
}

QStringList Preferences::toolboxCategorys()
{
    m_toolboxCategorys += m_customCategorys;

    return m_toolboxCategorys;
}

QList<QString> Preferences::themeFiles()
{
    QList<QString> list;
    list  << ":/qss/black.qss" << ":/qss/gray.qss" << ":/qss/blue.qss"
         << ":/qss/green.qss" << ":/qss/pink.qss" << ":/qss/yellow.qss" ;

    return list;
}

int Preferences::themeNumber()
{
    QString num = QString("%1").arg(m_themeNo);
    if( num.isEmpty() )
        m_themeNo = 1;

    return m_themeNo;
}

