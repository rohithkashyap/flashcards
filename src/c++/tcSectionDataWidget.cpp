#include "tcSectionDataWidget.h"
#include "ui_tcSectionDataWidget.h"

tcSectionDataWidget::tcSectionDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tcSectionDataWidget)
{
    // Setup UI
    ui->setupUi(this);
    this->setWindowTitle("Section Data");

    // Set table horizontal headers
    QHeaderView* header = ui->twSectionData->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->btnDeleteSelectedCard, SIGNAL(clicked()), this, SLOT(slot_btnDeleteSelectedCard_clicked()));
}

tcSectionDataWidget::~tcSectionDataWidget()
{
    delete ui;
}

void tcSectionDataWidget::setDatabase(QSqlDatabase adDatabase, QString asSection)
{
    // Set database
    mdDatabase = adDatabase;
    msSectionName = asSection;

    // Add section data
    addSectionDataToTable();
}

void tcSectionDataWidget::slot_btnDeleteSelectedCard_clicked()
{
    // Get selection
    QModelIndexList selection = ui->twSectionData->selectionModel()->selectedRows();

    if(selection.isEmpty())
    {
        QMessageBox::information(this, "No card selected!", "Please select a Card from the table first.", QMessageBox::Ok);
        return;
    }

    int nSelRow = selection.at(0).row();

    QSqlQuery oQuery(mdDatabase);
    oQuery.exec(QString("DELETE FROM %1 WHERE id=%2")
                .arg(msSectionName)
                .arg(nSelRow + 1));

    // Refresh table
    addSectionDataToTable();

}

void tcSectionDataWidget::closeEvent(QCloseEvent *)
{
    emit closed();
    ui->twSectionData->setRowCount(0);
}

void tcSectionDataWidget::addSectionDataToTable()
{
    ui->twSectionData->setRowCount(0);
    QSqlQuery oQuery(mdDatabase);

    oQuery.exec(QString("SELECT * FROM %1").arg(msSectionName));

    int nRowIdx = 0;
    while(oQuery.next())
    {
        ui->twSectionData->setRowCount(ui->twSectionData->rowCount() + 1);
        QTableWidgetItem *front = new QTableWidgetItem(oQuery.value("CardFront").toString());
        ui->twSectionData->setItem(nRowIdx, 0, front);

        QTableWidgetItem *back = new QTableWidgetItem(oQuery.value(2).toString());
        ui->twSectionData->setItem(nRowIdx, 1, back);

        // Increment index
        nRowIdx++;
    }
}
