#ifndef ADDWORKDIALOG_H
#define ADDWORKDIALOG_H

#include <QDialog>

#include "database.h"
#include "structs.h"

namespace Ui {
class AddWorkDialog;
}

class AddWorkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddWorkDialog(QWidget *parent = nullptr);
    ~AddWorkDialog();

public slots:
    void receiveData(WorkStruct* Work, int CheckID, bool Editing, int ID, Database *db, QMap<QString, double>* PriceList);

private slots:
    void on_CancelBtn_clicked();

    void on_AcceptBtn_clicked();

    void on_WorkTypeCB_currentIndexChanged(const QString &arg1);

    void on_CountSB_valueChanged(const QString &arg1);

private:
    void RefreshCost();

private:
    Ui::AddWorkDialog *ui;

    Database* db;

    QMap<QString, double>* PriceList;

    WorkStruct* Work;
    int CheckID;
    int ID;
};

#endif // ADDWORKDIALOG_H
