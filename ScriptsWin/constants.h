#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <tchar.h>
#include <Windows.h>
#include <Max.h>
#include <QString>
#include <QStringList>

// ²å¼þ´°¿ÚÃèÊö
#define IDC_WIN_TITEL                   "SouthgisMax for 3dsmax 2.0"
#define COMPANY                         "SouthGis"
#define PROGRAM                         "SouthgisMax"
static const QString CODE              = QString("yori");
// xml
#define XML_DTD                         "<!DOCTYPE xyori>"
#define XML_START_TAG                   "xyori"
#define XML_START_VERSION               "version"
#define XML_START_VERSION_VALUE         "1.0"

#define XML_TAG_CATEGORY                "category"
#define XML_CATEGORY_ATTR_NAME          "name"
#define XML_CATEGORY_ATTR_ICON          "icon"

#define XML_TAG_FILE                    "file"
#define XML_FILE_TITLE                  "title"
#define XML_FILE_ARCHIVE                "archive"
#define XML_FILE_ICON                   "img"
#define XML_TAG_PROP                    "propertise"

//file
#define XML_FILENAME                    "script.xml"

//database
#define SQL_BD_NAME                     "script"
static const char PassWord[] = {'p', 's', 'w', '=', 'y', 'o', 'r', 'i'};
#define SQL_TABLE_FILES                 "files"

// field name
#define SQL_FIELD_CATEGORY              "category"
#define SQL_FIELD_ICON                  "icon"
#define SQL_FIELD_NAME                  "name"
#define SQL_FIELD_CONTENT               "content"
#define SQL_FIELD_DATE                  "date"
#define SQL_FIELD_SIZE                  "size"
#define SQL_FIELD_TYPE                  "type"
#define SQL_FIELD_VERSION               "version"
#define SQL_FIELD_USAGE                 "usage"

#endif
