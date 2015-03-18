#ifndef SCRIPTS_WIDGET_H
#define SCRIPTS_WIDGET_H

#include <QWidget>
#include "xmlreader.h"
#include "xmlwriter.h"
#include "sqlwriter.h"
#include "sqlreader.h"

#include <QSqlDatabase>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QDialog>

class QMouseEvent;
class TableWidget;

namespace Ui {
class ScriptsWidget;
}

class AddCategoryDlg : public QDialog
{
    Q_OBJECT
public:
    explicit AddCategoryDlg(QWidget* parent = 0);

private slots:
    void addCategoryBox();

private:
    QLineEdit*      m_categoryTitleEdt;
    QPushButton*    m_addBtn;
};

class ScriptsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ScriptsWidget(QWidget *parent = 0);
    ~ScriptsWidget();

    QToolBox *toolbox() const;

signals:
    void storeToDatabase(const QString &filename, const QString &category, const QString &version, const QString &remark);

public slots:
    void addCategory();
    void delCategory();
    void addScript(QToolButton *btn, const QString &filename);
    void moveScript(int pageId, const QString &title, const QString &category);
    void removeScript(const QString &filename, const QString &category);
    void runScript(const QString &filename, const QString &category);
    void showProp(const QString &filename, const QString &category);
    void showAddScriptDlg();
    void bacthAddButton(const QList<QTreeWidgetItem *> &items);

    void dock(bool dock);
    void dockLeft();
    void dockRight();

    void currecntPageChanged(int);

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    void initStyleSheet();
    void initToolbox();
    void createPageButtons(TableWidget *tableWidget, const QString &category);
    QString btnTooltip(const QString &title, const QString &category, const QString &remark);

    void createDockMenu();

    Ui::ScriptsWidget *ui;

    QSqlDatabase     m_db;
    SqlWriter       *m_sqlWriter;
    SqlReader       *m_sqlReader;
};

#endif // SCRIPTS_WIDGET_H
