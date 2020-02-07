#include "workerswindowparent.h"

WorkersWindowParent::WorkersWindowParent(QWidget *parent) : QMainWindow(parent)
{

}

void WorkersWindowParent::receiveData(int WID, int PID, QString WName, Database *ndb)
{
    receiveDataFunc(WID, PID, WName, ndb);
}

void WorkersWindowParent::receiveDataFunc(int WID, int PID, QString WName, Database *ndb)
{
    WorkerID = WID;
    ProfID = PID;
    WorkerName = WName;
    db = ndb;
}
