#include "addcheckdialog.h"
#include "ui_addcheckdialog.h"

#include "addworkdialog.h"
#include <QMessageBox>

AddCheckDialog::AddCheckDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCheckDialog)
{
    ui->setupUi(this);
    // Works = new QVector<WorkStruct*>();
    PriceList = new QMap<QString, double>();
    db->GetWorkTypes(PriceList);
    QStringList* lst = new QStringList();
    db->GetPhotographerNames(lst);
    ui->PhotographerNameCB->addItems(*lst);
    SetNowDateTime();
}

AddCheckDialog::~AddCheckDialog()
{
    delete ui;
}

void AddCheckDialog::receiveData(CheckStruct* Check, bool Editing, bool isPhotographer, Database *db)
{
    model = new QStandardItemModel();

    ui->CashierNameLabel->setText(Check->CashierName);
    this->Editing = Editing;
    this->db = db;
    CashierID = Check->CashierID;
    QStringList* lst = new QStringList();
    db->GetPayStates(lst);
    if (Editing) {
        CheckID = Check->ID;
        ui->OutDE->setDate(Check->DateTime.date());
        ui->OutTE->setTime(Check->DateTime.time());
        db->GetCheckWorks(this->CheckID, model);
        ui->WorksTableView->setModel(model);
        ui->WorksTableView->hideColumn(5);
        ui->WorksTableView->hideColumn(0);
        ui->PhotographerNameCB->setCurrentIndex(Check->PhotographerID-1);
        ui->PhotographerNameLabel->setText(ui->PhotographerNameCB->currentText());

        StatusID = Check->StatusID;
        ui->StatusNameLbl->setText(lst->at(StatusID-1));

        for (int i = 0; i < model->rowCount(); i++) {
            WorkStruct* Work = new WorkStruct();
            Work->WorkID = model->item(i, 0)->text().toInt();
            Work->WorkType = model->item(i, 1)->text();
            Work->Count = model->item(i, 2)->text().toInt();
            Work->Price = model->item(i, 3)->text().toDouble();
            Work->Cost = model->item(i, 4)->text().toDouble();
            Work->Exists = true;
            Works.append(Work);
        }
        UpdateTable();


        if (isPhotographer) {
            ui->PhotographerNameCB->setHidden(true);
        }
        else {
            ui->PhotographerNameLabel->setHidden(true);
        }
    }
    else {
        ui->PhotographerNameLabel->setHidden(true);
    }
}

void AddCheckDialog::on_AddWorkBtn_clicked()
{
    AddWorkDialog* dialog = new AddWorkDialog(this);
    connect(this, SIGNAL(sendData(WorkStruct*, int, bool, int, Database*, QMap<QString, double>*)),
            dialog, SLOT(receiveData(WorkStruct*, int, bool, int, Database*, QMap<QString, double>*)), Qt::DirectConnection);
    Works.append(new WorkStruct());
    emit sendData(Works.last(), CheckID, false, Works.length()-1, db, PriceList);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setAttribute(Qt::WA_ShowModal);
    if (dialog->exec() == QDialog::Rejected) {
        Works.remove(Works.length()-1);
        UpdateTable();
    }
    else {
        UpdateTable();
    }
}

void AddCheckDialog::UpdateTable() {
    model = new QStandardItemModel();

    for (int i = 0; i < Works.length(); i++) {
        QList<QStandardItem*> lst;
        lst.append(new QStandardItem(QString::number(Works.at(i)->WorkID)));
        lst.append(new QStandardItem(Works.at(i)->WorkType));
        lst.append(new QStandardItem(QString::number(Works.at(i)->Count)));
        lst.append(new QStandardItem(QString::number(Works.at(i)->Price)));
        lst.append(new QStandardItem(QString::number(Works.at(i)->Cost)));
        // ID продукта не берём - не нужен в этом окне
        model->appendRow(lst);
    }

    QStringList strs;
    strs << "ID" << "Тип работы" << "Кол-во" << "Цена" << "Стоимость";
    model->setHorizontalHeaderLabels(strs);

    ui->WorksTableView->setModel(model);
    ui->WorksTableView->hideColumn(0);
    ui->WorksTableView->hideColumn(5);
    RefreshCost();
}


//    EDITING
//    if (ui->WorksTableView->currentIndex().isValid()) {
//        WorkID = model->item(ui->WorksTableView->currentIndex().row())->text().toInt();
//    }

void AddCheckDialog::on_EditWorkBtn_clicked()
{
    if (model->rowCount() > 0) {
        AddWorkDialog* dialog = new AddWorkDialog(this);
        connect(this, SIGNAL(sendData(WorkStruct*, int, bool, int, Database*, QMap<QString, double>*)),
                dialog, SLOT(receiveData(WorkStruct*, int, bool, int, Database*, QMap<QString, double>*)), Qt::DirectConnection);
        int SelectedRow = GetSelectedRowIndex();
        if (SelectedRow == 0) {
            ui->WorksTableView->selectRow(0);
        }
        WorkStruct* wrk = Works.at(SelectedRow);
        emit sendData(wrk, CheckID, true, Works.length()-1, db, PriceList);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->setAttribute(Qt::WA_ShowModal);
        if (dialog->exec()) {
            UpdateTable();
        }
    }
}

void AddCheckDialog::on_RemoveWorkBtn_clicked()
{
    if (model->rowCount() > 0) {
        QMessageBox::StandardButton dialogResult;
        dialogResult = QMessageBox::question(this, "Удалить?",
                                             "Вы действительно хотите удалить запись? Данные нельзя будет восстановить.",
                                             QMessageBox::Yes|QMessageBox::No);
        if (dialogResult == QMessageBox::Yes) {
            WorkStruct* Work = Works.at(GetSelectedRowIndex());
            if (Work->Exists) {
                db->RemoveWork(Work->WorkID);
            }
            Works.removeOne(Work);
            UpdateTable();
        }
    }
}

void AddCheckDialog::on_AcceptCheck_clicked()
{
    if (model->rowCount() > 0) {
        QDateTime OutDate;
        OutDate.setDate(ui->OutDE->date());
        OutDate.setTime(ui->OutTE->time());
        if (Editing) {
            db->EditCheck(CheckID, ui->PhotographerNameCB->currentIndex()+1, CashierID, OutDate, ui->CostLabel->text().toDouble(), StatusID);
        }
        else {
            db->AddCheck(ui->PhotographerNameCB->currentIndex()+1, CashierID, OutDate, ui->CostLabel->text().toDouble(), CheckID);
        }
        for (int i = 0; i < Works.length(); i++) {
            WorkStruct* Work = Works.at(i);
            QList<QString> lst = PriceList->keys();
            if (Work->Exists) {
                db->EditWork(Work->WorkID, CheckID, lst.indexOf(Work->WorkType)+1, Work->Count);
            }
            else {
                db->AddWork(lst.indexOf(Work->WorkType)+1, Work->Count, CheckID);
            }
        }
        accept();
    }
    else {
        QMessageBox::warning(this, "Ошибка ввода", "Вы оставили чек пустым. Добавьте хотя бы 1 услугу в список!");
    }
}

void AddCheckDialog::SetNowDateTime() {
    ui->OutDE->setDate(QDate::currentDate());
    ui->OutTE->setTime(QDateTime::currentDateTime().time());
}

void AddCheckDialog::RefreshCost()
{
    double Cost = 0;
    for (int i = 0; i < model->rowCount(); i++) {
        Cost += model->item(i, 4)->text().toDouble();
    }
    ui->CostLabel->setText(QString::number(Cost));
}

void AddCheckDialog::on_NowBtn_clicked()
{
    SetNowDateTime();
}

int AddCheckDialog::GetSelectedRowIndex() {
    int Index = ui->WorksTableView->currentIndex().row();
    if (Index == -1) {
        Index = 0;
    }
    return Index;
}

void AddCheckDialog::closeEvent(QCloseEvent * e)
{
    if (Editing) {
        if (!HasExistingWorks()) {
            if (model->rowCount() == 0) {
                QMessageBox::warning(this, "Ошибка ввода", "Вы оставили чек пустым. Добавьте хотя бы 1 услугу в список!");
                e->ignore();
            }
            else {
                QMessageBox::StandardButton dialogResult;
                dialogResult = QMessageBox::question(this, "Применить?",
                                                     "Применить изменения в квитанции?",
                                                     QMessageBox::Yes|QMessageBox::No);
                if (dialogResult == QMessageBox::Yes) {
                    on_AcceptCheck_clicked();
                    e->accept();
                }
                else {
                    e->ignore();
                }
            }
        }
        else {
            e->accept();
        }
    }
}

bool AddCheckDialog::HasExistingWorks() {
    for (int i = 0; i < Works.length(); i++) {
        if (Works.at(i)->Exists) { return true; }
    }
    return false;
}
