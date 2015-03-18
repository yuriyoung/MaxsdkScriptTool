#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QStringList>
#include <QRect>
#include <QColor>

/*
 * 系统偏好设置
 */
class Preferences
{
public:
    Preferences();
    ~Preferences();

    void reset();
    void load();
    void save();

    void        appendToolboxIcon(const QString &icon);
    QStringList toolboxIcons();
    void        appendToolboxTitle(const QString &title);
    QStringList toolboxTitles();
    void        appendToolboxCategory(const QString &category);
    QStringList toolboxCategorys();
    QList<QString>  themeFiles();

    int themeNumber();

public:
    QString m_tbRenameIcon;
    QString m_tbMaterialIcon;
    QString m_tbObjectEditIcon;
    QString m_tbImportExportIcon;
    QString m_tbSceneSheckIcon;
    QString m_tbOtherIcon;
    QString m_tbCustomIcon;

    QStringList m_toolboxCategorys;
    QStringList m_customCategorys;

    QStringList m_toolboxTitles;
    QStringList m_customTitles;

    QStringList m_toolboxIcons;
    QStringList m_customIcons;

    QString m_scriptButtonIcon;

    QString m_lastOpenFilePath;
    int     m_dockLocation;
    int     m_lastActivePage;

    int     m_themeNo;
};

#endif // PREFERENCES_H
