#ifndef ADDSCRIPTDLG_H
#define ADDSCRIPTDLG_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class AddScriptDlg;
}

class AddScriptDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddScriptDlg(QWidget *parent = 0);
    ~AddScriptDlg();
    void setCurrentCategory(int index);

signals:
    void bacthAddButton(const QList<QTreeWidgetItem*> &items);

private slots:
    void addFile();
    void itemSelected(QTreeWidgetItem *, int);
    void categoryChanged( int index );
    void versionChanged();
    void remarkChanged();
    void applyModify();

    void okToAdd();

private:
    Ui::AddScriptDlg *ui;
    bool m_isChanged;
};

#endif // ADDSCRIPTDLG_H
