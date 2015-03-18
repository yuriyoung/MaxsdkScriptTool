#include "addscriptdlg.h"
#include "ui_addscriptdlg.h"
#include "plugin.h"
#include "preferences.h"
#include "constants.h"

#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>

AddScriptDlg::AddScriptDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddScriptDlg)
{
    ui->setupUi(this);
    m_isChanged = false;

    QHeaderView *header = ui->scriptsTreeWidget->header();
    header->resizeSection(0, 30);

    QStringList titles = Plugin::g_preferences->toolboxTitles();
    ui->categoryCombox->addItems(titles);

    ui->helpeLabel->setVisible(false);

    connect( ui->openBtn, SIGNAL(clicked()), this, SLOT(addFile()) );
    connect( ui->scriptsTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
             this, SLOT(itemSelected(QTreeWidgetItem *, int)) );

    connect( ui->categoryCombox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(categoryChanged(int)) );

    connect( ui->versionEdt, SIGNAL(textChanged()),
            this, SLOT(versionChanged()) );

    connect( ui->remarkEdt, SIGNAL(textChanged()),
            this, SLOT(remarkChanged()) );

    connect( ui->applyBtn, SIGNAL(clicked()), this, SLOT( applyModify()) );
    connect( ui->buttonBox, SIGNAL(accepted()), this, SLOT(okToAdd()) );
}

AddScriptDlg::~AddScriptDlg()
{
    delete ui;
}

void AddScriptDlg::setCurrentCategory(int index)
{
    ui->categoryCombox->setCurrentIndex( index );
}

/**
 * @添加脚本文件
 * @brief AddScriptDlg::addFile
 */
void AddScriptDlg::addFile()
{
    QStringList filenames = QFileDialog::getOpenFileNames(
                               this,
                               tr("Add 3dsmax maxscript"),
                               Plugin::g_preferences->m_lastOpenFilePath,
                               tr("maxscript files (*.ms *.mse);;All files(*.*)") );
    if ( filenames.isEmpty() )
    {
        return;
    }

    ui->scriptsTreeWidget->clear();
    QFileInfo fi( filenames.at(0) );
    ui->dirEdt->setText(fi.absolutePath());
    Plugin::g_preferences->m_lastOpenFilePath = fi.absoluteFilePath();

    int count = ui->scriptsTreeWidget->topLevelItemCount();
    foreach( QString name, filenames )
    {
        count++;
        QFileInfo fi(name);
        QString basename = fi.completeBaseName();
        QTreeWidgetItem* item = new QTreeWidgetItem( ui->scriptsTreeWidget );
        item->setText( 0, QString("%1").arg(count) );
        item->setText( 1, basename );

        QStringList dataList;
        dataList << QString("%1").arg( ui->categoryCombox->currentIndex() )\
                 << name
                 << Plugin::formatSize( fi.size() ) \
                 << fi.suffix() \
                 << fi.lastModified().toString("yyyy/MM/dd hh:mm:ss") \
                 << ui->versionEdt->text()\
                 << ui->remarkEdt->toPlainText(); // 6
        item->setData(1, Qt::UserRole, dataList);
        ui->scriptsTreeWidget->addTopLevelItem(item);
        ui->scriptsTreeWidget->setItemSelected(item, true);
    }
}

/**
 * @添加脚本文件对话框中文件列表项选中时,取其数据同步显示
 * @brief AddScriptDlg::itemSelected
 * @param item
 */
void AddScriptDlg::itemSelected(QTreeWidgetItem *item, int /*column*/)
{
    QStringList dataList = item->data(1, Qt::UserRole).toStringList();
    int     categoryIndex = dataList.at(0).toInt();
//    QString filename = dataList.at(1);
    QString size     = dataList.at(2);
    QString type     = dataList.at(3);
    QString date     = dataList.at(4);
    QString version  = dataList.at(5);
    QString remark   = dataList.at(6);

    ui->categoryCombox->setCurrentIndex(categoryIndex);
    ui->sizeEdt->setText(size);
    ui->typeEdt->setText(type);
    ui->dateEdt->setText(date);
    ui->versionEdt->setText(version);
    ui->remarkEdt->setPlainText(remark);

    if( !m_isChanged )
        ui->applyBtn->setEnabled(false);
}

/**
 * @脚本文件列表选中项的类别更改时,修改项的数据
 * @brief AddScriptDlg::categoryChanged
 * @param index
 */
void AddScriptDlg::categoryChanged(int index)
{
    if( ui->scriptsTreeWidget->topLevelItemCount() == 0)
        return;
    ui->applyBtn->setEnabled(true);
    QTreeWidgetItem* curItem = ui->scriptsTreeWidget->currentItem();
    if( !curItem )
        return;

    QList<QTreeWidgetItem*> items = ui->scriptsTreeWidget->selectedItems();
    foreach (QTreeWidgetItem* item, items)
    {
        QStringList dataList = item->data(1, Qt::UserRole).toStringList();
        dataList.replace(0, QString("%1").arg(index));
        item->setData(1, Qt::UserRole, dataList);
    }

    m_isChanged = true;
}

/**
 * @修改项的版本数据
 * @brief AddScriptDlg::versionChanged
 */
void AddScriptDlg::versionChanged()
{
    if( ui->scriptsTreeWidget->topLevelItemCount() == 0)
        return;
    ui->applyBtn->setEnabled(true);
    QString version = ui->versionEdt->text();

    QList<QTreeWidgetItem*> items = ui->scriptsTreeWidget->selectedItems();
    foreach (QTreeWidgetItem* item, items)
    {
        QStringList dataList = item->data(1, Qt::UserRole).toStringList();
        dataList.replace(5, version);
        item->setData(1, Qt::UserRole, dataList);
    }

    m_isChanged = true;
}

/**
 * @修改备注信息数据
 * @brief AddScriptDlg::remarkChanged
 */
void AddScriptDlg::remarkChanged()
{
    if( ui->scriptsTreeWidget->topLevelItemCount() == 0)
        return;

    ui->applyBtn->setEnabled(true);
    QString remark = ui->remarkEdt->toPlainText();

    QList<QTreeWidgetItem*> items = ui->scriptsTreeWidget->selectedItems();
    foreach (QTreeWidgetItem* item, items)
    {
        QStringList dataList = item->data(1, Qt::UserRole).toStringList();
        dataList.replace(6, remark);
        item->setData(1, Qt::UserRole, dataList);
    }
    m_isChanged = true;
}

/**
 * @应用所有的修改
 * @brief AddScriptDlg::applyModify
 */
void AddScriptDlg::applyModify()
{
    if( ui->scriptsTreeWidget->topLevelItemCount() == 0)
        return;

    ui->applyBtn->setEnabled(false);

    QList<QTreeWidgetItem*> items = ui->scriptsTreeWidget->selectedItems();
    foreach (QTreeWidgetItem* item, items)
    {
        QStringList dataList;
        dataList << QString("%1").arg( ui->categoryCombox->currentIndex() )\
                 << item->data(1, Qt::UserRole).toStringList().at(1)\
                 << ui->sizeEdt->text()\
                 << ui->typeEdt->text()\
                 << ui->dateEdt->text()\
                 << ui->versionEdt->text()\
                 << ui->remarkEdt->toPlainText();
        item->setData(1, Qt::UserRole, dataList);
    }

    m_isChanged = false;
}

/**
 * @批量添加按钮到脚本管理主界面, 这里添加脚本文件比较慢,
 *  对于大量同时添加时,变得卡,需要使用线程来添加脚本文件
 * @brief AddScriptDlg::okToAdd
 */
void AddScriptDlg::okToAdd()
{
    if( ui->scriptsTreeWidget->topLevelItemCount() == 0)
        return;

//    QMessageBox msgBox;
//    msgBox.setWindowTitle(IDC_WIN_TITEL);
//    msgBox.setText("Make sure each script file has been modified to the appropriate [category].");
//    msgBox.setInformativeText("Are you sure all files are assigned to the appropriate category?");
//    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//    msgBox.setDefaultButton(QMessageBox::Yes);
//    int ret = msgBox.exec();
//    switch (ret)
//    {
//    case QMessageBox::Yes:
        QList<QTreeWidgetItem*> itemList;
        int total =  ui->scriptsTreeWidget->topLevelItemCount();
        for(int i = 0; i < total; ++i)
        {
            QTreeWidgetItem* item = ui->scriptsTreeWidget->topLevelItem( i );
            if(item)
            {
                itemList.append(item);
            }
        }
        emit bacthAddButton(itemList);
//        break;
//    case QMessageBox::No:
//        break;
//    default:
//        // should never be reached
//        break;
//    }
}
