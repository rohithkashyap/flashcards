#ifndef TCADDSECTIONWIDGET_H
#define TCADDSECTIONWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class tcAddSectionWidget;
}

class tcAddSectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit tcAddSectionWidget(QWidget *parent = 0);
    ~tcAddSectionWidget();

    void setDatabase(QSqlDatabase adDatabase);

signals:
    void closed();

private slots:
    void slot_btnAdd_clicked();
    void slot_btnClose_clicked();

    void closeEvent(QCloseEvent*);

private:
    Ui::tcAddSectionWidget *ui;


    QSqlDatabase mdDatabase;
};

#endif // TCADDSECTIONWIDGET_H
