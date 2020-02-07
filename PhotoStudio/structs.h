#ifndef STRUCTS_H
#define STRUCTS_H

#include <QString>
#include <QDateTime>

struct WorkStruct {
    int WorkID;
    QString WorkType;
    int Count;
    double Price;
    double Cost;
    bool Exists = false;
};

struct CheckStruct {
    int ID;
    int PhotographerID;
    int CashierID;
    QString CashierName;
    int StatusID;
    QDateTime DateTime;
};

#endif // STRUCTS_H
