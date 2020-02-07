#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "teacherscreen.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new Database();

    ui->LoginEdit->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ConnectBtn_clicked()
{
    if (ui->LoginEdit->text().isEmpty() || ui->PassEdit->text().isEmpty()) {
        ui->statusBar->showMessage("Одно или оба поля не заполнены");
        return;
    }
    QString name;
    int id;
    if (db->connectToDataBase()) {
        db->TeacherLoginQuery(ui->LoginEdit->text(), ui->PassEdit->text(), name, id);
        if (!name.isEmpty()) {
            TeacherScreen* teacherScreen = new TeacherScreen(this);
            connect(this, SIGNAL(sendData(int, QString, Database*)), teacherScreen, SLOT(recieveData(int, QString, Database*)), Qt::DirectConnection);
            emit sendData(id, name, db);
            teacherScreen->setAttribute(Qt::WA_DeleteOnClose);
            teacherScreen->show();
            this->hide();

            ui->LoginEdit->clear();
            ui->PassEdit->clear();
        }
        else {
            ui->statusBar->showMessage("Неверная связка логин/пароль");
        }
    }
}
