#include "addworkdialog.h"
#include "ui_addworkdialog.h"

AddWorkDialog::AddWorkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddWorkDialog)
{
    ui->setupUi(this);
}

AddWorkDialog::~AddWorkDialog()
{
    delete ui;
}

void AddWorkDialog::receiveData(WorkStruct* Work, int CheckID, bool Editing, int ID, Database *db, QMap<QString, double>* PriceList)
{
    this->db = db;
    this->Work = Work;
    this->CheckID = CheckID;
    this->ID = ID;
    this->PriceList = PriceList;
    ui->WorkTypeCB->addItems(PriceList->keys());
    if (Editing) {
        ui->WorkTypeCB->setCurrentIndex(PriceList->keys().indexOf(Work->WorkType));
        ui->CountSB->setValue(Work->Count);
    }
    else {
        ui->WorkTypeCB->setCurrentIndex(0);
    }
    RefreshCost();
}

void AddWorkDialog::on_CancelBtn_clicked()
{
    reject();
    close();
}

void AddWorkDialog::on_AcceptBtn_clicked()
{
    Work->WorkType = ui->WorkTypeCB->currentText();
    Work->Count = ui->CountSB->value();
    Work->Price = PriceList->value(Work->WorkType);
    Work->Cost = Work->Count*Work->Price;
    accept();
    close();

//    if (ID) {
//        //db->EditWork(Work->WorkID, CheckID, ui->WorkTypeCB->currentIndex()+1, ui->CountSB->value()); /// TODO: Мб чек не нужен?
//        /// TODO: Обновление инфы
//        this->close();
//    }
//    else {
//        //db->AddWork(ui->WorkTypeCB->currentIndex()+1, ui->CountSB->value(), CheckID);
//        /// TODO: Как обновлять инфу в таблице?
//        this->close();
//    }
}

void AddWorkDialog::RefreshCost()
{
    ui->WorkCostLabel->setText(QString::number(PriceList->value(ui->WorkTypeCB->currentText()) * ui->CountSB->value()));
}

void AddWorkDialog::on_WorkTypeCB_currentIndexChanged(const QString &arg1)
{
    RefreshCost();
}

void AddWorkDialog::on_CountSB_valueChanged(const QString &arg1)
{
    RefreshCost();
}
