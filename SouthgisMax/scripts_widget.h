#ifndef SCRIPTS_WIDGET_H
#define SCRIPTS_WIDGET_H

#include <QWidget>
#include "script_thread.h"

namespace Ui {
class ScriptsWidget;
}

class ScriptsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ScriptsWidget(QWidget *parent = 0);
    ~ScriptsWidget();
    
private slots:

private:
    Ui::ScriptsWidget *ui;
    QString         m_xmlfile;

//    ScriptThread    *m_scriptThread;
};

#endif // SCRIPTS_WIDGET_H
