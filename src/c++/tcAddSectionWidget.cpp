#include "tcAddSectionWidget.h"
#include "ui_tcAddSectionWidget.h"

tcAddSectionWidget::tcAddSectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tcAddSectionWidget)
{
    // Setup UI
    ui->setupUi(this);
    this->setWindowTitle("Add Section");

    // Connect GUI buttons to slots
    connect(ui->btnAdd,    SIGNAL(clicked()), this, SLOT(slot_btnAdd_clicked()));
    connect(ui->btnClose,  SIGNAL(clicked()), this, SLOT(slot_btnClose_clicked()));
}

tcAddSectionWidget::~tcAddSectionWidget()
{
    delete ui;
}

void tcAddSectionWidget::slot_btnAdd_clicked()
{
    // Get section name
    QString sSection = ui->teSectionName->toPlainText();

    // Run the query
    QString queryCreate;
    bool bQuery = false;

    QSqlQuery oQuery(mdDatabase);

    queryCreate = QString("CREATE TABLE IF NOT EXISTS [%1] ("
                          "ID INTEGER PRIMARY KEY,"
                          "CardFront TEXT NOT NULL,"
                          "CardBack TEXT NOT NULL,"
                          "CardDate DATE);")
                            .arg(sSection);

    // Execute the query
    bQuery = oQuery.exec(queryCreate);

    if(bQuery)
    {
        QMessageBox::information(this, "Section added successfully!",
                                 QString("Section %1 has been added successfully.").arg(sSection),
                                 QMessageBox::Ok);
    }
    else
    {
        QMessageBox::information(this, "Section add failed!",
                                 QString("The following error occured: %1").arg(oQuery.lastError().text()),
                                 QMessageBox::Ok);
    }

}

void tcAddSectionWidget::slot_btnClose_clicked()
{
    // Close the wodget
    this->close();
}

void tcAddSectionWidget::closeEvent(QCloseEvent *)
{
    emit closed();
    ui->teSectionName->setText("");
}

void tcAddSectionWidget::setDatabase(QSqlDatabase adDatabase)
{
    mdDatabase = adDatabase;
}

// INSERT INTO Test2 (CardFront, CardBack, CardDate) VALUES ('1', '2', CURRENT_DATE, 'localtime');
// SELECT datetime(Test2.CardDate, '+'||3||' days') from Test2
// Update Test2 SET CardDate=date(Test2.CardDate,'+'||3||' days');
// INSERT INTO Test1 (CardFront, CardBack, CardDate) VALUES ('1', '2', date(CURRENT_DATE,'+'||3||' days'));
