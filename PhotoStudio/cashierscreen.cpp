#include "cashierscreen.h"
#include "ui_cashierscreen.h"

#include "addcheckdialog.h"
#include <QMessageBox>

CashierScreen::CashierScreen(QWidget *parent) :
    WorkersWindowParent(parent),
    ui(new Ui::CashierScreen)
{
    ui->setupUi(this);
    ui->ToDateDE->setDate(QDate::currentDate());
    QStringList* lst = new QStringList();
    db->GetPayStates(lst);
    ui->PayStateCB->addItems(*lst);
    page = 1;
}

CashierScreen::~CashierScreen()
{
    delete ui;
}

void CashierScreen::on_ExitAction_triggered()
{
    QApplication::exit();
}

void CashierScreen::on_ChangeUserAction_triggered()
{
    ((QWidget*)parent())->show();
    this->close();
}

void CashierScreen::receiveDataFunc(int WID, int PID, QString WName, Database *ndb)
{
    WorkersWindowParent::receiveDataFunc(WID, PID, WName, ndb);

    setWindowTitle("Окно кассира. Кассир: " + WorkerName);

    RefreshPage();
    RefreshTable();
}

void CashierScreen::on_RefreshTableBtn_clicked()
{
    RefreshPage();
    RefreshTable();
}

void CashierScreen::RefreshTable() {
    model = new QStandardItemModel();
    if (db->FindChecks(ui->FromDateDE->date(), ui->ToDateDE->date(), ui->PayStateCB->currentIndex(), page, model)) {
        ui->ChecksTableView->setModel(model);
        QStringList strs;
        strs << "ID" << "ФИО фотографа" << "ФИО кассира" << "Дата выдачи" << "Стоимость" << "Статус";
        model->setHorizontalHeaderLabels(strs);
        ui->ChecksTableView->hideColumn(0);
        qDebug() << "Таблица обновлена." << endl;
        ui->ChecksTableView->selectRow(0);
        on_ChecksTableView_clicked(model->index(0, 0));
    }
    else {
        qDebug() << "Невозможно обновить таблицу!" << endl;
    }
}

void CashierScreen::on_AddCheckBtn_clicked()
{
    AddCheckDialog* dialog = new AddCheckDialog(this);
    connect(this, SIGNAL(sendData(CheckStruct*, bool, bool, Database*)), dialog, SLOT(receiveData(CheckStruct*, bool, bool, Database*)), Qt::DirectConnection);
    CheckStruct* Check = new CheckStruct();
    Check->CashierID = ProfID;
    Check->CashierName = WorkerName;
    emit sendData(Check, false, false, db);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setAttribute(Qt::WA_ShowModal);
    if (dialog->exec() == QDialog::Accepted) {
        RefreshTable();
    }
}

void CashierScreen::on_EditCheckBtn_clicked()
{
    if (model->rowCount() > 0) {
        AddCheckDialog* dialog = new AddCheckDialog(this);
        connect(this, SIGNAL(sendData(CheckStruct*, bool, bool, Database*)), dialog, SLOT(receiveData(CheckStruct*, bool, bool, Database*)), Qt::DirectConnection);
        CheckStruct* Check = new CheckStruct();
        Check->ID = model->item(GetSelectedRowIndex(), 0)->text().toInt();
        Check->CashierID = ProfID;
        Check->CashierName = WorkerName;

        QStringList* lst = new QStringList();
        db->GetPayStates(lst);
        Check->StatusID = lst->indexOf(model->item(GetSelectedRowIndex(), 5)->text())+1;
        lst = new QStringList();
        db->GetPhotographerNames(lst);
        Check->PhotographerID = lst->indexOf(model->item(GetSelectedRowIndex(), 1)->text())+1;
        Check->DateTime = QDateTime::fromString(model->item(GetSelectedRowIndex(), 3)->text());
        emit sendData(Check, true, false, db);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->setAttribute(Qt::WA_ShowModal);
        if (dialog->exec() == QDialog::Accepted) {
            RefreshTable();
        }
    }
}

void CashierScreen::on_RemoveCheckBtn_clicked()
{
    if (model->rowCount() > 0) {
        QMessageBox::StandardButton dialogResult;
        dialogResult = QMessageBox::question(this, "Удалить?",
                                             "Вы действительно хотите удалить запись? Данные нельзя будет восстановить.",
                                             QMessageBox::Yes|QMessageBox::No);
        if (dialogResult == QMessageBox::Yes) {
            int CheckID = model->item(GetSelectedRowIndex())->text().toInt();
            QVector<int>* IDs = new QVector<int>();
            db->GetCheckWorkIDs(CheckID, IDs);
            for (int i : *IDs) {
                db->RemoveWork(i);
            }
            db->RemoveCheck(CheckID);
            RefreshTable();
        }
    }
}

int CashierScreen::GetSelectedRowIndex() {
    int Index = ui->ChecksTableView->currentIndex().row();
    if (Index == -1) {
        Index = 0;
    }
    return Index;
}

void CashierScreen::RefreshPage()
{
    ui->PrevPageBtn->setEnabled(true);
    ui->NextPageBtn->setEnabled(true);
    if (page <= 1) {
        page = 1;
        ui->PrevPageBtn->setEnabled(false);
    }
    int pagesCount;
    db->GetPagesCount(ui->FromDateDE->date(), ui->ToDateDE->date(), ui->PayStateCB->currentIndex(), pagesCount);
    if (page >= pagesCount) {
        page = pagesCount;
        ui->NextPageBtn->setEnabled(false);
    }
    RefreshTable();
    ui->PageNumberLabel->setText(QString::number(page) + " из " + QString::number(pagesCount));
}

void CashierScreen::on_CheckPayedBtn_clicked()
{
    db->ChangeCheckState(4, model->item(GetSelectedRowIndex(), 0)->text().toInt());
    RefreshTable();
}

void CashierScreen::on_GiveCheckBtn_clicked()
{
    db->GiveCheck(model->item(GetSelectedRowIndex(), 0)->text().toInt());
    RefreshTable();
}

void CashierScreen::on_ChecksTableView_clicked(const QModelIndex &index)
{
    if (model->item(index.row(), 5)->text() == "Создано") {
        ui->GiveCheckBtn->setEnabled(false);
        ui->CheckPayedBtn->setEnabled(false);
        ui->EditCheckBtn->setEnabled(true);
        ui->RemoveCheckBtn->setEnabled(true);
    }
    else if (model->item(index.row(), 5)->text() == "В работе") {
        ui->GiveCheckBtn->setEnabled(false);
        ui->CheckPayedBtn->setEnabled(true);
        ui->EditCheckBtn->setEnabled(false);
        ui->RemoveCheckBtn->setEnabled(true);
    }
    else if (model->item(index.row(), 5)->text() == "Готово") {
        ui->GiveCheckBtn->setEnabled(false);
        ui->CheckPayedBtn->setEnabled(true);
        ui->EditCheckBtn->setEnabled(false);
        ui->RemoveCheckBtn->setEnabled(false);
    }
    else if (model->item(index.row(), 5)->text() == "Оплачено") {
        ui->GiveCheckBtn->setEnabled(true);
        ui->CheckPayedBtn->setEnabled(false);
        ui->EditCheckBtn->setEnabled(false);
        ui->RemoveCheckBtn->setEnabled(false);
    }
    else if (model->item(index.row(), 5)->text() == "Выдано") {
        ui->GiveCheckBtn->setEnabled(false);
        ui->CheckPayedBtn->setEnabled(false);
        ui->EditCheckBtn->setEnabled(false);
        ui->RemoveCheckBtn->setEnabled(false);
    }
}

void CashierScreen::on_ChecksTableView_doubleClicked(const QModelIndex &index)
{
    QString status = model->item(index.row(), 5)->text();
    if (status == "Создано") {
        on_EditCheckBtn_clicked();
    }
}

void CashierScreen::on_PrevPageBtn_clicked()
{
    page--;
    RefreshPage();
    RefreshTable();
}

void CashierScreen::on_NextPageBtn_clicked()
{
    page++;
    RefreshPage();
    RefreshTable();
}
