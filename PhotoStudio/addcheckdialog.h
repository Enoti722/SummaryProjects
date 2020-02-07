#ifndef ADDCHECKDIALOG_H
#define ADDCHECKDIALOG_H

#include <QDialog>

#include "database.h"
#include "structs.h"
#include <QCloseEvent>

namespace Ui {
class AddCheckDialog;
}

class AddCheckDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddCheckDialog(QWidget *parent = nullptr);
    ~AddCheckDialog();

signals:
    void sendData(WorkStruct* Work, int CheckID, bool Editing, int ID, Database *db, QMap<QString, double>* PriceList);

public slots:
    void receiveData(CheckStruct* Check, bool Editing, bool isPhotographer, Database *db);

protected:
    virtual void closeEvent(QCloseEvent * e) override;

private slots:
    void on_AddWorkBtn_clicked();

    void on_EditWorkBtn_clicked();

    void on_RemoveWorkBtn_clicked();

    void on_AcceptCheck_clicked();

    void on_NowBtn_clicked();

private:
    void UpdateTable();
    void SetNowDateTime();
    void RefreshCost();
    int GetSelectedRowIndex();
    bool HasExistingWorks();

private:
    Ui::AddCheckDialog *ui;

    QStandardItemModel *model;

    Database* db;

    QVector<WorkStruct*> Works;

    QMap<QString, double>* PriceList;

    int CashierID;
    int StatusID;

    bool Editing;
    int CheckID;
};

#endif // ADDCHECKDIALOG_H
