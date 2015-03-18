#include "scripts_widget.h"
#include "ui_scriptswidget.h"
#include "table_widget.h"
#include "constants.h"
#include "plugin.h"
#include "archivereader.h"
#include "scriptgup.h"
#include "preferences.h"
#include "propertydlg.h"
#include "addscriptdlg.h"
#include "dockablewindow.h"
#include "qwin/qwinwidget.h"

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QIcon>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>

#include <QDebug>

AddCategoryDlg::AddCategoryDlg(QWidget *parent):
    QDialog(parent)
{
    this->setWindowFlags( this->windowFlags() &~Qt::WindowContextHelpButtonHint);
    QLabel* lb = new QLabel(QDialog::tr("Input a name for your category, please."));
    m_categoryTitleEdt = new QLineEdit;
    m_addBtn = new QPushButton( QDialog::tr("Ok"));
    QGridLayout* grid = new QGridLayout(this);
    grid->addWidget(lb, 0, 0, 1, 3);
    grid->addWidget(m_categoryTitleEdt, 1, 0, 1, 2);
    grid->addWidget(m_addBtn, 1, 2, 1, 1);
    this->setWindowTitle( tr("Add custom category"));

    connect(m_addBtn, SIGNAL(clicked()), this, SLOT(addCategoryBox()) );
}

void AddCategoryDlg::addCategoryBox()
{
    QString title = m_categoryTitleEdt->text();
    if( title.isEmpty())
    {
        QMessageBox::information(this, tr("Add category"),
                                 tr("Category title must be Specified."), 0);
        return;
    }
    QToolBox* toolbox = Plugin::g_scriptsWidget->toolbox();
    TableWidget *tablePage = new TableWidget(toolbox);


    Plugin::g_preferences->appendToolboxCategory(title);
    Plugin::g_preferences->appendToolboxTitle(title);
    Plugin::g_preferences->appendToolboxIcon(Plugin::g_preferences->m_tbCustomIcon);
    int idx = toolbox->addItem(tablePage, QIcon( Plugin::g_preferences->m_tbCustomIcon ),title );
    toolbox->setCurrentIndex(idx);

    QObject::connect( tablePage, SIGNAL(addScript(QToolButton*, QString)),
             Plugin::g_scriptsWidget, SLOT(addScript(QToolButton*, QString)) );

    QObject::connect( tablePage, SIGNAL(addCategory()),
             Plugin::g_scriptsWidget, SLOT(addCategory()) );
    QObject::connect( tablePage, SIGNAL(delCategory()),
             Plugin::g_scriptsWidget, SLOT(delCategory()) );

    QObject::connect( tablePage, SIGNAL(moveScript(int,QString,QString)),
             Plugin::g_scriptsWidget, SLOT(moveScript(int,QString,QString)) );
    QObject::connect( tablePage, SIGNAL(runScript(QString,QString)),
             Plugin::g_scriptsWidget, SLOT(runScript(QString,QString)) );
    QObject::connect( tablePage, SIGNAL(removeScript(QString, QString)),
             Plugin::g_scriptsWidget, SLOT(removeScript(QString, QString)) );
    QObject::connect( tablePage, SIGNAL(showProp(QString, QString)),
             Plugin::g_scriptsWidget, SLOT(showProp(QString, QString)) );

    this->accept();
}


ScriptsWidget::ScriptsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScriptsWidget)
{
    ui->setupUi(this);
    ui->blackLb->installEventFilter(this);
    ui->grayLb->installEventFilter(this);
    ui->blueLb->installEventFilter(this);
    ui->greenLb->installEventFilter(this);
    ui->pinkLb->installEventFilter(this);
    ui->redLb->installEventFilter(this);

    ui->toolBox->widget(0)->hide();
    ui->toolBox->setItemEnabled(0, false);
    ui->toolBox->removeItem(0);


    //    initStyleSheet();
    createDockMenu();

    m_sqlWriter = new SqlWriter(m_db);
    m_sqlReader = new SqlReader(m_db);

    initToolbox();
    ui->toolBox->setCurrentIndex(Plugin::g_preferences->m_lastActivePage);

    connect( ui->addScriptBtn, SIGNAL(clicked()), this, SLOT(showAddScriptDlg()) );
    connect( ui->dockBtn, SIGNAL(clicked(bool)), this, SLOT(dock(bool)) );
    QObject::connect( this, SIGNAL(storeToDatabase(QString,QString,QString,QString)),
                      m_sqlWriter, SLOT(writeToFile(QString,QString,QString,QString)) );

    connect(ui->toolBox, SIGNAL(currentChanged(int)),
            this, SLOT(currecntPageChanged(int)) );
}

ScriptsWidget::~ScriptsWidget()
{
    m_sqlWriter->deleteLater();
    m_sqlReader->deleteLater();
    delete ui;
}

QToolBox* ScriptsWidget::toolbox() const
{
    return ui->toolBox;
}

// 添加一个分类
void ScriptsWidget::addCategory()
{
    AddCategoryDlg* addCateDlg = new AddCategoryDlg(this);

    addCateDlg->exec();
}

// 删除一个分类,直接删除,并且从数据库中删除该分类下的脚本
void ScriptsWidget::delCategory()
{
    int idx = ui->toolBox->currentIndex();
    QString title = ui->toolBox->itemText(idx);

    QMessageBox msgBox;
    msgBox.setWindowTitle(IDC_WIN_TITEL);
    msgBox.setText( tr("Are you sure delete the category?") );
    msgBox.setInformativeText( tr("Click 'Yes' will delete [%1] category and all scripts.").arg(title));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret)
    {
    case QMessageBox::Yes:
    {
        QString category = Plugin::g_preferences->m_toolboxCategorys.at(idx);
        m_sqlWriter->removeCategory(category);
        ui->toolBox->removeItem(idx);
        Plugin::g_preferences->m_customCategorys.removeOne(category);
        Plugin::g_preferences->m_customTitles.removeOne(category);
        Plugin::g_preferences->m_customIcons.removeOne(Plugin::g_preferences->m_tbCustomIcon);

        // idx 不会等于0
        ui->toolBox->setCurrentIndex(idx-1);
    }
        break;
    case QMessageBox::No:
        break;
    default:
        // should never be reached
        break;
    }


}

void ScriptsWidget::initStyleSheet()
{
    QFile qss(":/default.qss");
    if( !qss.open(QFile::ReadOnly) )
    {
        QMessageBox::information(this, IDC_WIN_TITEL,
                                QObject::tr("Set theme failed: Theme file"
                                "is missing or read error: %1").arg(qss.fileName()), QMessageBox::Ok);
        return;
    }
    QByteArray buff = qss.readAll();
    this->setStyleSheet(buff);
    ui->toolBox->setStyleSheet(buff);

    qss.close();
}

/**
 * @初始化toolbox脚本工具箱,即添加每个脚本分类页同时添加按钮(从数据库中读取)
 * @brief ScriptsWidget::initToolbox
 */
void ScriptsWidget::initToolbox()
{
    QStringList icons = Plugin::g_preferences->toolboxIcons();
    QStringList titles = Plugin::g_preferences->toolboxTitles();
    QStringList categorys = Plugin::g_preferences->toolboxCategorys();

    for( int i = 0; i < titles.count(); ++i )
    {
        TableWidget *tablePage = new TableWidget(ui->toolBox);
        createPageButtons( tablePage, categorys.at(i));
        ui->toolBox->addItem(tablePage, QIcon( icons.at(i) ), titles.at(i) );

        QObject::connect( tablePage, SIGNAL(addScript(QToolButton*, QString)),
                 this, SLOT(addScript(QToolButton*, QString)) );

        QObject::connect( tablePage, SIGNAL(addCategory()),
                 this, SLOT(addCategory()) );
        QObject::connect( tablePage, SIGNAL(delCategory()),
                 this, SLOT(delCategory()) );

        QObject::connect( tablePage, SIGNAL(moveScript(int,QString,QString)),
                 this, SLOT(moveScript(int,QString,QString)) );
        QObject::connect( tablePage, SIGNAL(runScript(QString,QString)),
                 this, SLOT(runScript(QString,QString)) );
        QObject::connect( tablePage, SIGNAL(removeScript(QString, QString)),
                 this, SLOT(removeScript(QString, QString)) );
        QObject::connect( tablePage, SIGNAL(showProp(QString, QString)),
                 this, SLOT(showProp(QString, QString)) );
    }
}

/**
 * @建立运行脚本文件的按钮
 * @函数名: ScriptsWidget::createPageButtons
 * @参数1: tableWidget, 显示按钮的列表界面
 * @参数2: category, 所在分类
 */
void ScriptsWidget::createPageButtons(TableWidget *tableWidget, const QString &category)
{
    QStringList nameList = m_sqlReader->selectFilenames(category);
    foreach( QString title, nameList )
    {
        QToolButton *btn = tableWidget->addButton( title, Plugin::g_preferences->m_scriptButtonIcon, category );
        QString remark = m_sqlReader->selectUsage(title, category);
        QString tooltip = btnTooltip(title, category, remark);
        btn->setToolTip( tooltip );
    }
}

QString ScriptsWidget::btnTooltip(const QString &title, const QString &category, const QString &remark )
{
    QString type = category;
    QStringList titles = Plugin::g_preferences->toolboxTitles();
    QStringList categorys = Plugin::g_preferences->toolboxCategorys();
    int index = categorys.indexOf( category );
    if( index != -1 )
        type = titles.at(index);
    QString tooltip = tr("Type: %1").arg(type) + "\n" + tr("Name: %1").arg(title)
            + "\n" + tr("Remark: %1").arg( remark );
    return tooltip;
}

/**
 * @添加新脚本文件(mse,ms)
 * @函数名: ScriptsWidget::addScript
 * @参数: filename,添加的脚本文件名
 */
void ScriptsWidget::addScript(QToolButton *btn, const QString &filename)
{
    int idx = ui->toolBox->currentIndex();
    QString category =  Plugin::g_preferences->toolboxCategorys().at(idx);
    QString remark = QString(tr("Descriptive information about the script file.\n"
                                "e.g.:how to use or what to work for..."));
    bool ok = m_sqlWriter->storeFile( filename, category );
    QFileInfo f( filename );
    QString fname = f.completeBaseName();
    m_sqlWriter->insertUsage(remark, category, fname);
    if( !ok )
        QMessageBox::information(this, IDC_WIN_TITEL, tr("read data failed.The script '%1' is invalid.").arg(fname), 0);

    btn->setToolTip( btnTooltip(fname, category, remark) );
}

void ScriptsWidget::moveScript(int pageId, const QString &title, const QString &category)
{
    QWidget* widget = ui->toolBox->widget(pageId);
    if( widget )
    {
        TableWidget* table = static_cast<TableWidget*>(widget);
        if( table )
        {
            if( table->hasButton( title ) )
            {
                QMessageBox::information(this, IDC_WIN_TITEL, tr("The script file already exists:%1").arg(title), 0);
                return;
            }
            table->setRowCount(table->rowCount()+1);
            QToolButton* btn = new QToolButton(table);
            btn->setIcon( QIcon(Plugin::g_preferences->m_scriptButtonIcon));
            btn->setIconSize( QSize(24,24) );
            btn->setText( title );
            btn->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );

            QString newCategory =  Plugin::g_preferences->toolboxCategorys().at(pageId);

            //new item with a btn
            QTableWidgetItem* item = new QTableWidgetItem( title );
            QStringList datalist;
            datalist << title << Plugin::g_preferences->m_scriptButtonIcon << newCategory;
            item->setData( Qt::UserRole, datalist );
            table->setItem( table->rowCount() - 1, 0, item );
            table->setCellWidget( table->rowCount()-1, 0, btn );

            // set tooltips
            QString remark = m_sqlReader->selectUsage(title, category);
            QString tooltip = btnTooltip(title, newCategory, remark);
            btn->setToolTip( tooltip );

            connect( btn, SIGNAL(clicked()), table, SLOT( buttonClicked() ) );

            bool ok = m_sqlWriter->updateFile(title, category, newCategory);
            if( !ok )
            {
                QMessageBox::information(this, IDC_WIN_TITEL,
                                         tr("Move script file failed:%1").arg(title), 0);
                disconnect( btn, SIGNAL(clicked()), table, SLOT(buttonClicked()) );
                table->removeCellWidget( item->row(), 0 );
                table->removeRow( item->row() );
            }
        }
    }
    else
    {
        QMessageBox::information(this, IDC_WIN_TITEL, tr("Move script file failed."), 0);
    }
}

void ScriptsWidget::removeScript(const QString &filename, const QString &category)
{
    bool ok = m_sqlWriter->removeFile(filename, category);
    if( !ok )
        QMessageBox::information(0, IDC_WIN_TITEL, tr("Removed script file failed:%1").arg(filename), 0);
}

/**
 * @运行脚本文件
 * @函数名: ScriptsWidget::runScript
 * @参数1: filename, 当前运行的脚本文件名
 * @参数2: category, 当前脚本所在的分类
 * @参数用以数据字段从数据中查询脚本
 */
void ScriptsWidget::runScript(const QString &filename, const QString &category)
{
    if( filename.isEmpty() ) return;
    QByteArray data = m_sqlReader->readFile(filename, category);
    QString type = m_sqlReader->selectType(filename, category);
    if( data.isEmpty() )
    {
        QMessageBox::information(this, IDC_WIN_TITEL, tr("Run script failed:%1.\n Script is missing.").arg(filename), 0);
        return;
    }
    // 创建临时文件
    QString tempDir = QDir::tempPath();
    QString tempfile = tempDir + "/" + filename + "." + type;

    QFile file(tempfile);
    if( !file.open(QIODevice::WriteOnly) )
    {
        file.remove();
        return;
    }
    file.write(data);
    file.close();

    QString fname = file.fileName();
    ScriptGup gup;
    QString script = QString("FileIn \"%1\")").arg( fname );
    gup.ExecuteStringScript(script.toLocal8Bit().data());
//    bool ok = gup.ExecuteFileScript(fname.toLocal8Bit().data());
    file.remove();
//    if( !ok )
//    {
//        gup.ExecuteStringScript("actionMan.executeAction 0 \"40472\"");
//        QMessageBox::information(this, IDC_WIN_TITEL, tr("run script error:%1").arg(filename), 0);
//    }
}

/**
 * @显示脚本属性对话框
 * @函数名: ScriptsWidget::showProp
 * @参数1: filename,传入脚本文件名以便从数据库中查询对应数据
 * @参数2: category,传入脚本分类名以便从数据库中查询对应数据
 */
void ScriptsWidget::showProp(const QString &filename, const QString &category)
{
    TableWidget *table_page = qobject_cast<TableWidget*>(sender());
    PropertyDlg *dlg = new PropertyDlg(m_sqlWriter, this);
    dlg->setFilename(filename);
    dlg->setCategory(category);
    QString usage = m_sqlReader->selectUsage(filename, category);
    dlg->setUsage(usage);
    dlg->setDate(m_sqlReader->selectDate(filename,category));
    dlg->setSize(m_sqlReader->selectSize(filename,category));
    dlg->setType(m_sqlReader->selectType(filename,category));
    dlg->setVersion(m_sqlReader->selectVersion(filename,category));

    connect(dlg, SIGNAL(updateTooltip(QString)),
            table_page, SLOT(updateBtnTooltips(QString)));

    int ret = dlg->exec();
    if( ret == 1)
        dlg->applyUsage();
}

/**
 * @显示添加脚本文件对话框
 * @brief ScriptsWidget::showAddScriptDlg
 */
void ScriptsWidget::showAddScriptDlg()
{
    AddScriptDlg *addDlg = new AddScriptDlg(this);
    int idx = ui->toolBox->currentIndex();
    addDlg->setCurrentCategory(idx);

    connect(addDlg, SIGNAL(bacthAddButton(QList<QTreeWidgetItem*>)),
            this, SLOT(bacthAddButton(QList<QTreeWidgetItem*>)) );

    addDlg->exec();
}

/**
 * @批量添加脚本文件
 * @brief ScriptsWidget::bacthAddButton
 * @param item
 */
void ScriptsWidget::bacthAddButton(const QList<QTreeWidgetItem *> &items)
{
    int lastPage = 0;
    foreach (QTreeWidgetItem *item, items)
    {
        QStringList dataList = item->data(1, Qt::UserRole).toStringList();
        int     categoryIndex = dataList.at(0).toInt();
        QString filename = dataList.at(1);
        QString version   = dataList.at(5);
        QString remark   = dataList.at(6);

        QString category =  Plugin::g_preferences->toolboxCategorys().at(categoryIndex);
        QWidget *widget = ui->toolBox->widget( categoryIndex );
        TableWidget *tableWidget = qobject_cast<TableWidget*>(widget);
        if(! tableWidget )
        {
            QMessageBox::information(this, IDC_WIN_TITEL,
                                     tr("Add script failed, can not find tablewidget."), 0);
            return;
        }
        QFileInfo f( filename );
        QString basename = f.completeBaseName();
        QToolButton *btn = tableWidget->addButton(basename, \
                                                  Plugin::g_preferences->m_scriptButtonIcon, \
                                                  category);
        if( btn == NULL)
        {
            continue;
        }

        QString tooltip = btnTooltip(basename, category, remark);
        btn->setToolTip( tooltip );

        emit storeToDatabase(filename, category, version, remark);

        lastPage = categoryIndex;
    }

    ui->toolBox->setCurrentIndex(lastPage);
}

void ScriptsWidget::createDockMenu()
{
    QMenu *dockMenu = new QMenu(this);

    QAction* dockLeftAct = new QAction(this);
    dockMenu->addAction(dockLeftAct);
    dockLeftAct->setText(tr("Dock &Left"));
    dockLeftAct->setIcon(QIcon(":/images/dock_left.png"));
    connect(dockLeftAct, SIGNAL(triggered()), this, SLOT(dockLeft()));

    QAction* dockRightAct = new QAction(this);
    dockMenu->addAction(dockRightAct);
    dockRightAct->setText(tr("Dock &Right"));
    dockRightAct->setIcon(QIcon(":/images/dock_right.png"));
    connect(dockRightAct, SIGNAL(triggered()), this, SLOT(dockRight()));

    ui->dockBtn->setMenu(dockMenu);
}

void ScriptsWidget::dock(bool dock)
{
    if( dock )
    {
        Plugin::g_preferences->m_dockLocation = 2;
        Plugin::g_dockableWindow->Dock(DockableWindow::Left);
        ui->dockBtn->setChecked(true);
        ui->dockBtn->setIcon( QIcon(":/images/pushpin_left_azure.png") );
    }
    else
    {
        Plugin::g_preferences->m_dockLocation = 0;
        Plugin::g_dockableWindow->Float();
        ui->dockBtn->setChecked(false);
        ui->dockBtn->setIcon( QIcon(":/images/pushpin_azure.png") );
    }
}

void ScriptsWidget::dockLeft()
{
    Plugin::g_preferences->m_dockLocation = 2;
    Plugin::g_dockableWindow->Dock(DockableWindow::Left);
    ui->dockBtn->setChecked(true);
    ui->dockBtn->setIcon( QIcon(":/images/pushpin_left_azure.png") );
}

void ScriptsWidget::dockRight()
{
    Plugin::g_preferences->m_dockLocation = 3;
    Plugin::g_dockableWindow->Dock(DockableWindow::Right);
    ui->dockBtn->setChecked(true);
    ui->dockBtn->setIcon( QIcon(":/images/pushpin_right_azure.png") );
}

void ScriptsWidget::currecntPageChanged(int index)
{
    Plugin::g_preferences->m_lastActivePage = index;

}

bool ScriptsWidget::eventFilter(QObject *obj, QEvent *e)
{
    if( e->type() == QEvent::MouseButtonPress )
    {
        int num = 0;
         if( obj == ui->blackLb )
         {
             num = 0;
             Plugin::g_qWin->setStyleSheet("background-color: rgb(42, 42, 42);"
                                   "color: rgb(255, 255, 255);");
         }
         else if( obj == ui->grayLb )
         {
             num = 1;
             Plugin::g_qWin->setStyleSheet("background-color: rgb(197, 197, 197);"
                                   " color: rgb(0, 0, 0);");
         }
        else if( obj == ui->blueLb )
        {
            num = 2;
            Plugin::g_qWin->setStyleSheet("background-color: rgb(0, 85, 127);"
                                  "color: rgb(255, 255, 255);");
        }
         else if( obj == ui->greenLb )
         {
            num = 3;
            Plugin::g_qWin->setStyleSheet("background-color: rgb(0, 59, 0);"
                                  "color: rgb(255, 255, 255);");
         }
         else if( obj == ui->pinkLb )
         {
            num = 4;
            Plugin::g_qWin->setStyleSheet("background-color: rgb(180, 90, 147);"
                                  "color: rgb(255, 255, 255);");
         }
         else if( obj == ui->redLb )
         {
            num = 5;
            Plugin::g_qWin->setStyleSheet("background-color: rgb(170, 170, 127);"
                                  "color: rgb(255, 255, 255);");
         }
        Plugin::g_preferences->m_themeNo = num;
        Plugin::setTheme();
    }

    return QWidget::eventFilter(obj, e);
}

