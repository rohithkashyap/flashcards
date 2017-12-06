#ifndef TCADDCARDWIDGET_H
#define TCADDCARDWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

namespace Ui {
class tcAddCardWidget;
}

class tcAddCardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit tcAddCardWidget(QWidget *parent = 0);
    ~tcAddCardWidget();

    void setDatabase (QSqlDatabase adDatabase, QString asCurrSection);

signals:
    void closed();

private slots:
    void slot_btnAdd_clicked();
    void slot_btnClose_clicked();
    void slot_btnClearBoxText_clicked();

    void closeEvent(QCloseEvent*);

private:
    Ui::tcAddCardWidget *ui;

    void addSectionsToGui(QString asCurrSection);

    QSqlDatabase mdDatabase;
};

#endif // TCADDCARDWIDGET_H
