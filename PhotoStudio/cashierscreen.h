#ifndef CASHIERSCREEN_H
#define CASHIERSCREEN_H

#include "workerswindowparent.h"

#include "structs.h"

namespace Ui {
class CashierScreen;
}

class CashierScreen : public WorkersWindowParent
{
    Q_OBJECT

public:
    explicit CashierScreen(QWidget *parent = nullptr);
    ~CashierScreen();

signals:
    void sendData(CheckStruct* Check, bool Editing, bool isPhotographer, Database *db);

private slots:
    void on_ExitAction_triggered();

    void on_ChangeUserAction_triggered();

    void on_RefreshTableBtn_clicked();

    void on_AddCheckBtn_clicked();

    void on_EditCheckBtn_clicked();

    void on_RemoveCheckBtn_clicked();

    void on_GiveCheckBtn_clicked();

    void on_ChecksTableView_clicked(const QModelIndex &index);

    void on_ChecksTableView_doubleClicked(const QModelIndex &index);

    void on_CheckPayedBtn_clicked();

    void on_PrevPageBtn_clicked();

    void on_NextPageBtn_clicked();

protected:
    virtual void receiveDataFunc(int WID, int PID, QString WName, Database* ndb) override;

private:
    int GetSelectedRowIndex();

    void RefreshTable();

    void RefreshPage();

    int page;

    Ui::CashierScreen *ui;

};

#endif // CASHIERSCREEN_H
