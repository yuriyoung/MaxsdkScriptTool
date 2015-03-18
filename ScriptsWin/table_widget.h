#ifndef TABLE_WIDGET_H
#define TABLE_WIDGET_H

#include <QTableWidget>
#include <QToolButton>

//class QSignalMapper;

class TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit TableWidget(QWidget *parent = 0);
    
    QList<QToolButton*> addButtons(const QStringList &titles, const QList<QString> &icons, const QString &category);
    QList<QToolButton*> addButtons(const QStringList &titles, const QString &category);
    QToolButton* addButton(const QString &title, const QString &icon, const QString &category);
    QToolButton* addButton(const QString &title, const QString &category);

    QList<QToolButton*> itemButtons();
    QToolButton* itemButton( int row );

    bool hasButton(const QString &title);

signals:
    void addCategory();
    void delCategory();
    void clickedScript(const QString &filename, const QString &category);
    void runScript(const QString &filename, const QString &category);
    void addScript(QToolButton *btn, const QString &filename );
    void moveScript( int pageId, const QString &filename, const QString &category );
    void removeScript(const QString &filename, const QString &category);
    void showProp(const QString &filename, const QString &category);

public slots:
    void buttonClicked();

private slots:
    void viewPropAct();
    void addScriptAct();
    void moveScriptAct(int id);
    void removeScriptAct();
    void updateBtnTooltips(const QString &tooltips);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    void initialize();
//    QSignalMapper*          m_moveActMapper;
};

#endif // TABLE_WIDGET_H
