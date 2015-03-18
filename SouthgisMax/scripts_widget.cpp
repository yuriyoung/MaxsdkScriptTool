#include "scripts_widget.h"
#include "ui_scripts_widget.h"
#include "constants.h"
#include "plugin.h"

#include <QHeaderView>
#include <QDir>

ScriptsWidget::ScriptsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScriptsWidget)
{
    ui->setupUi(this);

    QStringList labels;
    labels << tr("Title") << tr("tooltips");
    ui->scriptsTreeWidget->header()->setResizeMode(QHeaderView::Stretch);
    ui->scriptsTreeWidget->setHeaderLabels(labels);
    ui->scriptsTreeWidget->hideColumn(1);

    QString data_path = Plugin::dataPath();
    m_xmlfile = data_path + QDir::separator() + XML_FILENAME;
    bool ok = ui->scriptsTreeWidget->loadFile(m_xmlfile);
    if( !ok )
    {
    }
}

ScriptsWidget::~ScriptsWidget()
{
    ui->scriptsTreeWidget->saveFile(m_xmlfile);
    delete ui;
}
