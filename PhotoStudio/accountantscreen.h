#ifndef ACCOUNTANTSCREEN_H
#define ACCOUNTANTSCREEN_H

#include "workerswindowparent.h"

namespace Ui {
class AccountantScreen;
}

class AccountantScreen : public WorkersWindowParent
{
    Q_OBJECT

public:
    explicit AccountantScreen(QWidget *parent = nullptr);
    ~AccountantScreen();

protected:
    virtual void receiveDataFunc(int WID, int PID, QString WName, Database* ndb) override;

private slots:
    void on_RefreshTableBtn_clicked();

    void on_PrevPageBtn_clicked();

    void on_NextPageBtn_clicked();

    void on_HTML_Action_triggered();

private:
    Ui::AccountantScreen *ui;

    void RefreshTable();

    void RefreshPage();

    bool createHtmlTableFromModel();

    int page;
};

#endif // ACCOUNTANTSCREEN_H
