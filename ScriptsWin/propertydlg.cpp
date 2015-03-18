#include "propertydlg.h"
#include "ui_propertydlg.h"
#include "preferences.h"
#include "plugin.h"

PropertyDlg::PropertyDlg(SqlWriter *sqlwriter, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PropertyDlg),
    m_sqlWriter(sqlwriter)
{

    ui->setupUi(this);
    m_usageEdt = new QTextEdit();
    m_usageEdt->setStyleSheet("background-color: rgb(255, 255, 255);"
                           "color: rgb(0, 0, 0);");

    m_isChanged = false;

    connect( m_usageEdt, SIGNAL(textChanged()), this, SLOT(modifyUsage()) );
    connect( ui->applyBtn, SIGNAL(clicked()), this, SLOT(applyUsage()) );
}

PropertyDlg::~PropertyDlg()
{
    delete ui;
}

void PropertyDlg::setFilename(const QString &filename)
{
    QTableWidgetItem *item = new QTableWidgetItem(filename);
    ui->propertyTb->setItem(0, 1, item);
    m_filename = filename;
}

void PropertyDlg::setCategory(const QString &category)
{
    QString type = category;
    QStringList titles = Plugin::g_preferences->toolboxTitles();
    QStringList categorys = Plugin::g_preferences->toolboxCategorys();
    int index = categorys.indexOf( category );
    if( index != -1 )
        type = titles.at(index);

    QTableWidgetItem *item = new QTableWidgetItem(type);
    ui->propertyTb->setItem(1, 1, item);
    m_category = category;
}

/**
 * @从数据库中查询使用方法字段值设置到属性对话框的文本编辑框中,并保证应用按钮不可用状态
 * @brief PropertyDlg::setUsage
 * @param usage
 */
void PropertyDlg::setUsage(const QString &usage)
{
    m_usageEdt->setText(usage);
    ui->propertyTb->setCellWidget(6, 1, m_usageEdt);
    ui->applyBtn->setEnabled(false);
    m_isChanged = false;
}

/**
 * @修改脚本的使用方法属性后,设置当前的应用按钮可用
 * @brief PropertyDlg::modifyUsage
 */
void PropertyDlg::modifyUsage()
{
    ui->applyBtn->setEnabled(true);
    m_isChanged = true;
}

/**
 * @在修改脚本的使用方法属性后,需要更新一下该按钮的提示信息.
 * @brief PropertyDlg::applyUsage
 */
void PropertyDlg::applyUsage()
{
    if( ! m_isChanged ) return;

    m_usage = m_usageEdt->toPlainText();
    m_sqlWriter->insertUsage( m_usage, m_category, m_filename );
    ui->applyBtn->setEnabled(false);

    QString type = m_category;
    QStringList titles = Plugin::g_preferences->toolboxTitles();
    QStringList categorys = Plugin::g_preferences->toolboxCategorys();
    int index = categorys.indexOf( m_category );
    if( index != -1 )
        type = titles.at(index);

    QString tooltip = tr("Type: %1").arg(type) + "\n" + tr("Name: %1").arg(m_filename)
            + "\n" + tr("Remark: %1").arg( m_usage );
    emit updateTooltip(tooltip);
}

void PropertyDlg::setDate(const QString &date)
{
    QTableWidgetItem *item = new QTableWidgetItem(date);
    ui->propertyTb->setItem(4, 1, item);
}

void PropertyDlg::setVersion(const QString &version)
{
    QTableWidgetItem *item = new QTableWidgetItem(version);
    ui->propertyTb->setItem(5, 1, item);
}

void PropertyDlg::setSize(const QString &size)
{
    QTableWidgetItem *item = new QTableWidgetItem(size);
    ui->propertyTb->setItem(3, 1, item);
}

void PropertyDlg::setType(const QString &type)
{
    QTableWidgetItem *item = new QTableWidgetItem(type);
    ui->propertyTb->setItem(2, 1, item);
}
