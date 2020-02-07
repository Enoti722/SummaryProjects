#ifndef PHOTOGRAPHERSCREEN_H
#define PHOTOGRAPHERSCREEN_H

#include "workerswindowparent.h"

namespace Ui {
class PhotographerScreen;
}

class PhotographerScreen : public WorkersWindowParent
{
    Q_OBJECT

public:
    explicit PhotographerScreen(QWidget *parent = nullptr);
    ~PhotographerScreen();

signals:
    void sendData(CheckStruct* Check, bool Editing, bool isPhotographer, Database *db);

protected:
    virtual void receiveDataFunc(int WID, int PID, QString WName, Database *ndb) override;

private slots:
    void on_GetNextCheckInQueueBtn_clicked();

    void on_AcceptClientBtn_clicked();

    void on_WorkDoneBtn_clicked();

    void on_SkipCheck_clicked();

    void on_EditCheckBtn_clicked();

    void on_ExitAction_triggered();

    void on_ChangeUserAction_triggered();

private:
    void GetCheckAndFillTable();


    Ui::PhotographerScreen *ui;

    int CheckSkipCounter;

    // QVector<WorkStruct*> Works;

    CheckStruct* Info;
};

#endif // PHOTOGRAPHERSCREEN_H
