#ifndef PROPERTYDLG_H
#define PROPERTYDLG_H

#include <QDialog>
#include <QTextEdit>
#include "sqlwriter.h"

namespace Ui {
class PropertyDlg;
}

class PropertyDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit PropertyDlg(SqlWriter *sqlwriter, QWidget *parent = 0);
    ~PropertyDlg();
    
    void setFilename(const QString &filename);
    void setCategory(const QString &category);
    void setType(const QString &type);
    void setDate(const QString &date);
    void setVersion(const QString &version);
    void setSize( const QString &size);
    void setUsage(const QString &usage);

signals:
    void updateTooltip(const QString &tooltips);

public slots:
    void modifyUsage();
    void applyUsage();

private:
    Ui::PropertyDlg *ui;
    QString         m_filename;
    QString         m_category;
    QString         m_usage;

    QTextEdit       *m_usageEdt;
    SqlWriter       *m_sqlWriter;
    bool            m_isChanged;
};

#endif // PROPERTYDLG_H
