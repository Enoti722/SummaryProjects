#include "adddatedialog.h"
#include "ui_adddatedialog.h"

AddDateDialog::AddDateDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddDateDialog)
{
    ui->setupUi(this);

    ui->NewDateEdit->setDate(QDate::currentDate());
    ui->NewDateEdit->setMaximumDate(QDate::currentDate());
}

AddDateDialog::~AddDateDialog()
{
    delete ui;
}

void AddDateDialog::on_AddBtn_clicked()
{
    emit sendNewDate(ui->NewDateEdit->date());
    close();
}
