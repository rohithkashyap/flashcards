#ifndef TCSECTIONDATAWIDGET_H
#define TCSECTIONDATAWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class tcSectionDataWidget;
}

class tcSectionDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit tcSectionDataWidget(QWidget *parent = 0);
    ~tcSectionDataWidget();

    void setDatabase(QSqlDatabase adDatabase, QString asSection);

signals:
    void closed();

private slots:
    void slot_btnDeleteSelectedCard_clicked();
    void closeEvent(QCloseEvent*);

private:
    Ui::tcSectionDataWidget *ui;

    void addSectionDataToTable();

    QSqlDatabase mdDatabase;
    QString msSectionName;
};

#endif // TCSECTIONDATAWIDGET_H
