#include "photographerscreen.h"
#include "ui_photographerscreen.h"

#include "addcheckdialog.h"

PhotographerScreen::PhotographerScreen(QWidget *parent) :
    WorkersWindowParent(parent),
    ui(new Ui::PhotographerScreen)
{
    ui->setupUi(this);
}

PhotographerScreen::~PhotographerScreen()
{
    delete ui;
}

void PhotographerScreen::receiveDataFunc(int WID, int PID, QString WName, Database *ndb)
{
    WorkersWindowParent::receiveDataFunc(WID, PID, WName, ndb);

    setWindowTitle("Окно фотографа. Фотограф: " + WorkerName);

    on_GetNextCheckInQueueBtn_clicked();
}

void PhotographerScreen::on_GetNextCheckInQueueBtn_clicked()
{
    CheckSkipCounter = 1;
    GetCheckAndFillTable();
}

void PhotographerScreen::on_AcceptClientBtn_clicked()
{
    Info->StatusID = 2;
    db->ChangeCheckState(2, Info->ID);
    ui->AcceptClientBtn->setEnabled(false);
    ui->GetNextCheckInQueueBtn->setEnabled(false);
    ui->WorkDoneBtn->setEnabled(true);
    ui->SkipCheck->setEnabled(false);
    ui->EditCheckBtn->setEnabled(true);
    ui->AcceptClientBtn->setText("Клиент принят");
}

void PhotographerScreen::on_WorkDoneBtn_clicked()
{
    Info->StatusID = 3;
    db->ChangeCheckState(3, Info->ID);
    ui->AcceptClientBtn->setEnabled(true);
    ui->GetNextCheckInQueueBtn->setEnabled(true);
    ui->SkipCheck->setEnabled(true);
    ui->EditCheckBtn->setEnabled(false);
    ui->AcceptClientBtn->setText("Принять клиента");

    // Создание продукта в базе

    for(int i = 0; i < model->rowCount(); i++) {
        QString ProductName = model->item(i, 1)->text() + " X" + model->item(i, 2)->text() + " =" + model->item(i, 4)->text() + " " + QDate::currentDate().toString();
        db->CreateProduct(ProductName, QDate::currentDate(), model->item(i, 0)->text().toInt());
    }

    on_GetNextCheckInQueueBtn_clicked();
}

void PhotographerScreen::on_SkipCheck_clicked()
{
    CheckSkipCounter++;
    GetCheckAndFillTable();
}

void PhotographerScreen::GetCheckAndFillTable() {
    Info = new CheckStruct();
    if (!db->GetNextCheckInQueue(ProfID, CheckSkipCounter, Info)) {
        ui->statusbar->showMessage("Пока клиентов нет!", 10000);
        ui->AcceptClientBtn->setEnabled(false);


        QStringList strs;
        model = new QStandardItemModel();
        strs << "ID" << "Тип работы" << "Кол-во" << "Цена" << "Стоимость";
        model->setHorizontalHeaderLabels(strs);

        ui->WorksTableView->setModel(model);

        ui->WorksTableView->hideColumn(5);
        ui->WorksTableView->hideColumn(0);

        ui->CashierNameLbl->setText("");
        ui->CheckNumberLbl->setText("");
        ui->CheckDateLbl->setText("");
        ui->CheckTimeLbl->setText("");
        ui->SkipCheck->setEnabled(false);
        ui->WorkDoneBtn->setEnabled(false);
        ui->AcceptClientBtn->setEnabled(false);
        ui->EditCheckBtn->setEnabled(false);

        return;
    }
    model = new QStandardItemModel();
    db->GetCheckWorks(Info->ID, model);

    QStringList strs;
    strs << "ID" << "Тип работы" << "Кол-во" << "Цена" << "Стоимость";
    model->setHorizontalHeaderLabels(strs);

    ui->WorksTableView->setModel(model);

    ui->CashierNameLbl->setText(Info->CashierName);
    ui->CheckDateLbl->setText(Info->DateTime.date().toString("dd.MM.yyyy"));
    ui->CheckTimeLbl->setText(Info->DateTime.time().toString("hh:mm"));
    ui->CheckNumberLbl->setText(QString::number(Info->ID));

    ui->WorksTableView->hideColumn(5);
    ui->WorksTableView->hideColumn(0);

    if (Info->StatusID == 2) {
        ui->AcceptClientBtn->setEnabled(false);
        ui->GetNextCheckInQueueBtn->setEnabled(false);
        ui->WorkDoneBtn->setEnabled(true);
        ui->SkipCheck->setEnabled(false);
        ui->EditCheckBtn->setEnabled(true);
        ui->AcceptClientBtn->setText("Клиент принят");
    }
    else if (Info->StatusID == 1) {
        ui->statusbar->showMessage("Найден новый клиент!", 10000);
        ui->AcceptClientBtn->setEnabled(true);
        ui->SkipCheck->setEnabled(true);
        ui->WorkDoneBtn->setEnabled(false);
        ui->EditCheckBtn->setEnabled(false);
        ui->AcceptClientBtn->setText("Принять клиента");
    }
}

void PhotographerScreen::on_EditCheckBtn_clicked()
{
    AddCheckDialog* dialog = new AddCheckDialog(this);
    connect(this, SIGNAL(sendData(CheckStruct*, bool, bool, Database*)), dialog, SLOT(receiveData(CheckStruct*, bool, bool, Database*)), Qt::DirectConnection);
    emit sendData(Info, true, true, db);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setAttribute(Qt::WA_ShowModal);
    if (dialog->exec() == QDialog::Accepted) {
        GetCheckAndFillTable();
    }
}

void PhotographerScreen::on_ExitAction_triggered()
{
    QApplication::exit();
}

void PhotographerScreen::on_ChangeUserAction_triggered()
{
    ((QWidget*)parent())->show();
    this->close();
}
