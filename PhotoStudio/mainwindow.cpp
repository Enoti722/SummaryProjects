#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "photographerscreen.h"
#include "cashierscreen.h"
#include "accountantscreen.h"

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
    int WorkerType;
    int ProfID;
    if (db->connectToDataBase()) {
        db->WorkerLogin(ui->LoginEdit->text(), ui->PassEdit->text(), name, id, WorkerType, ProfID);
        if (!name.isEmpty()) {
            WorkersWindowParent* nextScreen;
            if (WorkerType == 1) {
                nextScreen = new PhotographerScreen(this);
            }
            else if (WorkerType == 2) {
                nextScreen = new CashierScreen(this);
            }
            else if (WorkerType == 3) {
                nextScreen = new AccountantScreen(this);
            }
            connect(this, SIGNAL(sendData(int, int, QString, Database*)), nextScreen, SLOT(receiveData(int, int, QString, Database*)), Qt::DirectConnection);
            emit sendData(id, ProfID, name, db);
            nextScreen->setAttribute(Qt::WA_DeleteOnClose);
            nextScreen->show();
            this->hide();

            ui->LoginEdit->clear();
            ui->PassEdit->clear();
        }
        else {
            ui->statusBar->showMessage("Неверная связка логин/пароль");
        }
    }
}
