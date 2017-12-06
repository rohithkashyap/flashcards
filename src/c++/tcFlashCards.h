#ifndef TCFLASHCARDS_H
#define TCFLASHCARDS_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QDateTime>
#include <QButtonGroup>
#include "tcAddCardWidget.h"
#include "tcAddSectionWidget.h"
#include "tcSectionDataWidget.h"

namespace Ui {
class tcFlashCards;
}

class tcFlashCards : public QMainWindow
{
    Q_OBJECT

public:
    explicit tcFlashCards(QWidget *parent = 0);
    ~tcFlashCards();

private slots:
    void slot_btnAddCard_clicked();
    void slot_btnSectionsList_clicked();
    void slot_btnShowAnswer_clicked();
    void slot_actionAddNewSection_clicked();
    void slot_btnShowSecData_clicked();

    void slot_btnRemindAgain_clicked();
    void slot_btnRemind1Day_clicked();
    void slot_btnRemind2Day_clicked();
    void slot_btnRemind3Day_clicked();

    void slot_tableOpenSection_clicked(QModelIndex index);
    void slot_addSectionsToTable();
    void slot_btnDelSection_clicked();

private:
    Ui::tcFlashCards *ui;

    void startSectionStudy();
    void getCardInfo();
    void restoreDatesInSection();

    QString msDatabaseFilename;
    QSqlDatabase mdDatabase;
    QList<int> mlIdxList;
    QString msCurrSection;
    int mnCurrIdx;

    tcAddCardWidget *mpAddCardWidget;
    tcAddSectionWidget *mpAddSectionWidget;
    tcSectionDataWidget *mpSecDataWidget;
};

#endif // TCFLASHCARDS_H
