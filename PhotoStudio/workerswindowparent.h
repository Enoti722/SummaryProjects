#ifndef WORKERSWINDOWPARENT_H
#define WORKERSWINDOWPARENT_H

#include <QMainWindow>

#include "database.h"

class WorkersWindowParent : public QMainWindow
{
    Q_OBJECT
public:
    explicit WorkersWindowParent(QWidget *parent = nullptr);

public slots:
    virtual void receiveData(int WID, int PID, QString WName, Database* ndb);

protected:
    virtual void receiveDataFunc(int WID, int PID, QString WName, Database* ndb);

protected:
    Database* db;
    int WorkerID;
    int ProfID;
    QString WorkerName;

    QStandardItemModel *model;
};

#endif // WORKERSWINDOWPARENT_H
