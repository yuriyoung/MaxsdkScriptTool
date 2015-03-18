#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <tchar.h>
#include <Windows.h>
#include <Max.h>
#include <QString>

// 插件窗口描述
#define IDC_WIN_TITEL                   "SouthgisMax for 3dsmax 2.0"
#define PROGRAM                         "SouthgisMax"
// library descriptions
#define IDS_LIBDESCRIPTION				"Southgis Tools(www.southgis.com)"
#define IDS_CATEGORY                    "Digital City Tools"
#define IDS_CLASS_NAME                  "YMenu"
#define IDS_PARAMS                      "Macai"
static const QString CODE              = QString("yori");

// mani menu and sub menu ids
#define IDC_YMENU_SEPARATOR             10000
#define IDC_YMENU_PLUGIN				10001
#define IDC_YMENU_CREATER				10002
#define IDC_YMENU_MODIFIER				10003
#define IDC_YMENU_EXPORTER				10004
#define IDC_YMENU_IMPORTER				10006
#define IDC_YMENU_TOOLBAR				10007
#define IDC_YMENU_SETTING               10008
#define IDC_YMENU_HELPER				10009
#define IDC_YMENU_HELP					10010
#define IDC_YMENU_REG					10011
#define IDC_YMENU_ABOUT					10012

#define MENU_TITLE_MAIN					"SouthgisMax"
#define MENU_TITLE_PLUGIN				"插件集"
#define MENU_TITLE_CREATER				"建模助手"
#define MENU_TITLE_MODIFIER				"资源管理"
#define MENU_TITLE_EXPORTER				"导出"
#define MENU_TITLE_IMPORTER				"导入"
#define MENU_TITLE_TOOLBAR				"工具栏"
#define MENU_TITLE_SETTING				"设置"
#define MENU_TITLE_HELPER				"帮助"
#define MENU_TITLE_HELP					"帮助"
#define MENU_TITLE_REG                  "注册"
#define MENU_TITLE_ABOUT				"关于"

#endif
