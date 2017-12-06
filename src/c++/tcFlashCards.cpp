#include "tcFlashCards.h"
#include "ui_tcFlashCards.h"

tcFlashCards::tcFlashCards(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::tcFlashCards)
{
    // Initialize variable
    mnCurrIdx = 0;
    msCurrSection = "";
    msDatabaseFilename = "../resources/flashcardsDB.sqlite";

    // Setup UI
    ui->setupUi(this);
    this->setWindowTitle("Flash Cards");

    // Initialize pointers
    mpAddCardWidget = new tcAddCardWidget();
    mpAddSectionWidget = new tcAddSectionWidget();
    mpSecDataWidget = new tcSectionDataWidget();

    // Set table horizontal headers
    QHeaderView* header = ui->tableSections->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    // Connect GUI buttons to slots
    connect (ui->btnAdd,                SIGNAL(clicked()),   this, SLOT(slot_btnAddCard_clicked()));
    connect (ui->btnAddSections,        SIGNAL(clicked()),   this, SLOT(slot_actionAddNewSection_clicked()));
    connect (ui->btnSectionsList,       SIGNAL(clicked()),   this, SLOT(slot_btnSectionsList_clicked()));
    connect (ui->btnShowAnswer,         SIGNAL(clicked()),   this, SLOT(slot_btnShowAnswer_clicked()));
    connect (ui->actionAddNewCard,      SIGNAL(triggered()), this, SLOT(slot_btnAddCard_clicked()));
    connect (ui->actionAddNewSection,   SIGNAL(triggered()), this, SLOT(slot_actionAddNewSection_clicked()));
    connect (ui->btnRemindAgain,        SIGNAL(clicked()),   this, SLOT(slot_btnRemindAgain_clicked()));
    connect (ui->btnRemind1Day,         SIGNAL(clicked()),   this, SLOT(slot_btnRemind1Day_clicked()));
    connect (ui->btnRemind2Day,         SIGNAL(clicked()),   this, SLOT(slot_btnRemind2Day_clicked()));
    connect (ui->btnRemind3Day,         SIGNAL(clicked()),   this, SLOT(slot_btnRemind3Day_clicked()));
    connect (ui->tableSections,         SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slot_tableOpenSection_clicked(QModelIndex)));
    connect (ui->btnShowSecData,        SIGNAL(clicked()),   this, SLOT(slot_btnShowSecData_clicked()));
    connect (mpAddCardWidget,           SIGNAL(closed()),    this, SLOT(slot_addSectionsToTable()));
    connect (mpAddSectionWidget,        SIGNAL(closed()),    this, SLOT(slot_addSectionsToTable()));
    connect (mpSecDataWidget,           SIGNAL(closed()),    this, SLOT(slot_addSectionsToTable()));
    connect (ui->btnDelSection,         SIGNAL(clicked()),   this, SLOT(slot_btnDelSection_clicked()));

    // Initialize database
    if (!msDatabaseFilename.isEmpty())
    {
        // Create a database file to fill the tables into.
        // Check if the database connection already exists
        if (QSqlDatabase::contains("mdDatabase_Connection"))
        {
            mdDatabase = QSqlDatabase::database("mdDatabase_Connection");
        }
        else
        {
            mdDatabase = QSqlDatabase::addDatabase("QSQLITE", "mdDatabase_Connection");
            mdDatabase.setDatabaseName(msDatabaseFilename);
        }

        if(!mdDatabase.open())
        {
            mdDatabase.open();
        }
    }

    // Add sections to table
    ui->stackedWidget->setCurrentIndex(0);
    slot_addSectionsToTable();
}

tcFlashCards::~tcFlashCards()
{
    delete ui;
}

void tcFlashCards::slot_addSectionsToTable()
{
    // Clear the table
    ui->tableSections->setRowCount(0);

    // Run the query
    QString queryStr;
    QSqlQuery oQuery(mdDatabase);

    queryStr = "SELECT name FROM sqlite_master where type='table'";
    oQuery.exec(queryStr);

    // Fill the table
    int nRowIdx = 0;
    while(oQuery.next())
    {
        // Fil first column (Section name)
        ui->tableSections->setRowCount(ui->tableSections->rowCount() + 1);
        QTableWidgetItem *item = new QTableWidgetItem(oQuery.value(0).toString());
        ui->tableSections->setItem(nRowIdx, 0, item);

        // Fill second column (Number of cards)
        QSqlQuery oSubQuery(mdDatabase);
        oSubQuery.exec(QString("SELECT Count(*) FROM [%1]")
                       .arg(oQuery.value(0).toString()));

        while(oSubQuery.next())
        {
            QTableWidgetItem *subItem = new QTableWidgetItem(QString::number(oSubQuery.value(0).toInt()));
            ui->tableSections->setItem(nRowIdx, 1, subItem);
        }

        // Increment index
        nRowIdx++;
    }

}

void tcFlashCards::slot_btnAddCard_clicked()
{
    QString sSelectedSec = "";
    if(mpAddCardWidget->isVisible())
    {
        mpAddCardWidget->close();
    }

    // Get selection
    QModelIndexList selection = ui->tableSections->selectionModel()->selectedRows();

    if(!selection.isEmpty())
    {
        sSelectedSec = ui->tableSections->item(selection.at(0).row(), 0)->text();
    }

    // Open add card widget
    mpAddCardWidget->setDatabase(mdDatabase, sSelectedSec);
    mpAddCardWidget->show();
}

void tcFlashCards::slot_btnSectionsList_clicked()
{
    // Set stacked widget indexS
    ui->stackedWidget->setCurrentIndex(0);

    // Refresh sections list
    slot_addSectionsToTable();
}

void tcFlashCards::slot_actionAddNewSection_clicked()
{
    if(mpAddSectionWidget->isVisible())
    {
        mpAddSectionWidget->close();
    }

    mpAddSectionWidget->setDatabase(mdDatabase);
    mpAddSectionWidget->show();
}

void tcFlashCards::slot_tableOpenSection_clicked(QModelIndex index)
{
    // Get index of selected row
    int nIdx = index.row();
    msCurrSection = "";
    mlIdxList.clear();
    mnCurrIdx = 0;

    // Get selected section name
    msCurrSection = ui->tableSections->item(nIdx, 0)->text();

    // Start the study session
    startSectionStudy();
}

void tcFlashCards::startSectionStudy()
{
    // Run the query to get all IDs of today's date
    QSqlQuery oQuery(mdDatabase);
    oQuery.exec(QString("SELECT id FROM [%1] WHERE CardDate<=date('now', 'localtime')")
                        .arg(msCurrSection));

    while(oQuery.next())
    {
        mlIdxList.append(oQuery.value(0).toInt());
    }

    // Deal with empty list
    if (mlIdxList.isEmpty())
    {
        oQuery.exec(QString("SELECT MIN(CardDate) FROM [%1]").arg(msCurrSection));

        QString sDate = "";
        while(oQuery.next())
        {
            sDate = oQuery.value(0).toString();
            break;
        }

        // If no date, then table is empty
        if(sDate.isEmpty())
        {
            QMessageBox::information(this, "No cards", "Selected section has no cards. Please select a "
                                     "different section, or add cards in the current section", QMessageBox::Ok);
        }
        // Else, let user know when the section will be available
        else
        {
            int nButton = QMessageBox::information(this, "Selected section not available",
                                     QString("Selected section will not be available until %1")
                                     .arg(sDate),QMessageBox::Ok, QMessageBox::RestoreDefaults);

            if(nButton == QMessageBox::RestoreDefaults)
            {
                restoreDatesInSection();
            }
        }
        return;
    }

    // Get cards
    getCardInfo();
}

void tcFlashCards::getCardInfo()
{
    // Clear texts
    ui->teFront->setText("");
    ui->teBack->setText("");
    ui->stackedWidget->setCurrentIndex(1);
    ui->swCardDetails->setCurrentIndex(0);

    // Read DB
    QSqlQuery oQuery(mdDatabase);
    int nHigh = mlIdxList.size();

    // Alert if section has ended
    if(mlIdxList.isEmpty())
    {
        QMessageBox::information(this, "End of section!", "Current section has ended", QMessageBox::Ok);
        ui->stackedWidget->setCurrentIndex(0);
        return;
    }

    // Generate random index
    mnCurrIdx = QDateTime::currentMSecsSinceEpoch() % nHigh;

    QString sQueryStr = QString("SELECT * FROM [%1] WHERE id=%2;")
                            .arg(msCurrSection)
                            .arg(mlIdxList.at(mnCurrIdx));

    oQuery.exec(sQueryStr);

    // Set Front of card
    while(oQuery.next())
    {
        ui->teFront->setText(oQuery.value(1).toString());
    }
}

void tcFlashCards::slot_btnShowAnswer_clicked()
{
    // Run query
    QSqlQuery oQuery(mdDatabase);
    QString sQueryStr = QString("SELECT * FROM [%1] WHERE id=%2;")
                            .arg(msCurrSection)
                            .arg(mlIdxList.at(mnCurrIdx));

    oQuery.exec(sQueryStr);

    // Set back of card
    while(oQuery.next())
    {
        ui->teBack->setText(oQuery.value("CardBack").toString());
    }

    // Change answer stacked widget index
    ui->swCardDetails->setCurrentIndex(1);
}

void tcFlashCards::slot_btnRemindAgain_clicked()
{
    // Do nothing and get next card
    getCardInfo();
}

void tcFlashCards::slot_btnRemind1Day_clicked()
{
    // Run the query
    QSqlQuery oQuery(mdDatabase);
    mdDatabase.transaction();
    QString sQueryStr = QString("UPDATE [%1] SET CardDate=date(%1.CardDate,'+'||1||' days') WHERE id=%2;")
                                .arg(msCurrSection)
                                .arg(mlIdxList.at(mnCurrIdx));

    oQuery.exec(sQueryStr);
    mdDatabase.commit();

    // Remove index from list
    mlIdxList.removeAt(mnCurrIdx);

    // Get next card
    getCardInfo();
}

void tcFlashCards::slot_btnRemind2Day_clicked()
{
    // Run the query
    QSqlQuery oQuery(mdDatabase);
    mdDatabase.transaction();
    QString sQueryStr = QString("UPDATE [%1] SET CardDate=date(%1.CardDate,'+'||2||' days') WHERE id=%2;")
                                .arg(msCurrSection)
                                .arg(mlIdxList.at(mnCurrIdx));

    oQuery.exec(sQueryStr);
    mdDatabase.commit();

    // Remove index from list
    mlIdxList.removeAt(mnCurrIdx);

    // Get next card
    getCardInfo();
}

void tcFlashCards::slot_btnRemind3Day_clicked()
{
    // Run the query
    QSqlQuery oQuery(mdDatabase);
    mdDatabase.transaction();
    QString sQueryStr = QString("UPDATE [%1] SET CardDate=date(%1.CardDate,'+'||3||' days') WHERE id=%2;")
                                .arg(msCurrSection)
                                .arg(mlIdxList.at(mnCurrIdx));

    oQuery.exec(sQueryStr);
    mdDatabase.commit();

    // Remove index from list
    mlIdxList.removeAt(mnCurrIdx);

    // Get next card
    getCardInfo();
}

void tcFlashCards::slot_btnShowSecData_clicked()
{
    // Get selection
    QModelIndexList selection = ui->tableSections->selectionModel()->selectedRows();

    if(selection.isEmpty())
    {
        QMessageBox::information(this, "No section selected!", "Please select a Section from the table first.", QMessageBox::Ok);
        return;
    }

    int nSelRow = selection.at(0).row();

    // Show details
    if(mpSecDataWidget->isVisible())
    {
        mpSecDataWidget->close();
    }
    mpSecDataWidget->setDatabase(mdDatabase, ui->tableSections->item(nSelRow, 0)->text());
    mpSecDataWidget->show();
}

void tcFlashCards::slot_btnDelSection_clicked()
{
    // Get selection
    QModelIndexList selection = ui->tableSections->selectionModel()->selectedRows();

    if(selection.isEmpty())
    {
        QMessageBox::information(this, "No section selected!", "Please select a Section from the table first.", QMessageBox::Ok);
        return;
    }
    else
    {
        int sel = QMessageBox::warning(this, "Delete Section?", "Are you sure you want to delete this section?",
                                       QMessageBox::Yes, QMessageBox::No);

        if(sel == QMessageBox::No)
        {
            return;
        }
    }

    int nSelRow = selection.at(0).row();



    QSqlQuery oQuery(mdDatabase);

    mdDatabase.transaction();
    oQuery.exec(QString("DROP TABLE [%1]")
                        .arg(ui->tableSections->item(nSelRow, 0)->text()));
    mdDatabase.commit();

    slot_btnSectionsList_clicked();
}

void tcFlashCards::restoreDatesInSection()
{
    QSqlQuery oQuery(mdDatabase);

    mdDatabase.transaction();
    oQuery.exec(QString("UPDATE [%1] SET CardDate=date('now', 'localtime');")
                        .arg(msCurrSection));
    mdDatabase.commit();
}

