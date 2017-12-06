#include "tcAddCardWidget.h"
#include "ui_tcAddCardWidget.h"

tcAddCardWidget::tcAddCardWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tcAddCardWidget)
{
    // Sstup UI
    ui->setupUi(this);
    this->setWindowTitle("Add Card");

    // Connect GUI buttons to slots
    connect(ui->btnAdd,     SIGNAL(clicked()), this, SLOT(slot_btnAdd_clicked()));
    connect(ui->btnClose,   SIGNAL(clicked()), this, SLOT(slot_btnClose_clicked()));
    connect(ui->btnClearBoxText, SIGNAL(clicked()), this, SLOT(slot_btnClearBoxText_clicked()));
}

tcAddCardWidget::~tcAddCardWidget()
{
    delete ui;
}

void tcAddCardWidget::setDatabase(QSqlDatabase adDatabase, QString asCurrSection)
{
    // Update database
    mdDatabase = adDatabase;

    // Add sections to combobox
    addSectionsToGui(asCurrSection);
}

void tcAddCardWidget::slot_btnAdd_clicked()
{
    // Get GUI info
    QString sSection = ui->cbSections->currentText();
    QString sFront = ui->teFront->toPlainText();
    QString sBack = ui->teBack->toPlainText();

    // Run the query
    QString queryCreate;
    bool bQuery = false;

    QSqlQuery oQuery(mdDatabase);

    queryCreate = QString("INSERT INTO [%1] (CardFront, CardBack, CardDate) VALUES ('%2', '%3', date('now', 'localtime'));")
                        .arg(sSection)
                        .arg(sFront)
                        .arg(sBack);

    // Execute the query
    bQuery = oQuery.exec(queryCreate);

    // Let user know
    if(bQuery)
    {
//        QMessageBox::information(this, "Card added successfully!",
//                                 QString("Card has been added successfully to %1.").arg(sSection),
//                                 QMessageBox::Ok);

        ui->teBack->setText("");
        ui->teFront->setText("");
    }
    else
    {
        QMessageBox::information(this, "Card add failed!",
                                 QString("The following error occured: %1").arg(oQuery.lastError().text()),
                                 QMessageBox::Ok);
    }
}

void tcAddCardWidget::slot_btnClose_clicked()
{
    // Close the widget
    this->close();
}

void tcAddCardWidget::slot_btnClearBoxText_clicked()
{
    ui->teBack->setText("");
    ui->teFront->setText("");
}

void tcAddCardWidget::closeEvent(QCloseEvent *)
{
    emit closed();
    ui->teBack->setText("");
    ui->teFront->setText("");
    ui->cbSections->setCurrentIndex(0);
}

void tcAddCardWidget::addSectionsToGui(QString /*asCurrSection*/)
{
    // Run the query
    QString queryStr;
    bool bQuery = false;
    QSqlQuery oQuery(mdDatabase);

    ui->cbSections->clear();

    queryStr = "SELECT name FROM sqlite_master where type='table'";

    bQuery = oQuery.exec(queryStr);

    // Let user know
    if(bQuery)
    {
        int nIdx = 0;
        while(oQuery.next())
        {
            ui->cbSections->addItem(oQuery.value(0).toString());
            ui->cbSections->setCurrentIndex(nIdx);
            nIdx++;
        }
    }
    else
    {
        QMessageBox::critical(this, "Unable to load sections!",
                              "Unable to load all sections. Please restart the application.",
                              QMessageBox::Ok);
    }

}
