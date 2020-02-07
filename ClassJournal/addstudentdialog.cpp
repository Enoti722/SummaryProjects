#include "addstudentdialog.h"
#include "ui_addstudentdialog.h"

#include <QRegExpValidator>
#include <QMessageBox>

AddStudentDialog::AddStudentDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddStudentDialog)
{
    ui->setupUi(this);

    ui->SecondNameEdit->setValidator(new QRegExpValidator(QRegExp("[а-яА-я-]*")));
    ui->FirstNameEdit->setValidator(new QRegExpValidator(QRegExp("[а-яА-я-]*")));
    ui->LastNameEdit->setValidator(new QRegExpValidator(QRegExp("[а-яА-я-]*")));
}

AddStudentDialog::~AddStudentDialog()
{
    delete ui;
}

void AddStudentDialog::on_AddBtn_clicked()
{
    if (ui->SecondNameEdit->text().isEmpty() || ui->FirstNameEdit->text().isEmpty() || ui->LastNameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка ввода", "Не все поля заполнены!");
        return;
    }
    QString newName = ui->SecondNameEdit->text() + " " + ui->FirstNameEdit->text() + " " + ui->LastNameEdit->text();
    emit sendNewStudent(newName);
    close();
}
