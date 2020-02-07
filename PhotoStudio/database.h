#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QStandardItemModel>
#include <QMap>
#include "structs.h"

// Директивы имен таблиц и их полей

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    bool connectToDataBase();
    bool WorkerLogin(QString login, QString pass, QString &name, int &id, int& WorkerType, int& ProfID);
    bool FindChecks(QDate fromDate, QDate toDate, int paymentStatus, int page, QStandardItemModel* model);
    bool FindChecksForReport(QDate fromDate, QDate toDate, int paymentStatus, int page, QSqlQuery* queryRef);
    QStringList QueryDataToString(QSqlQuery* query);
    bool GetPagesCount(QDate fromDate, QDate toDate, int paymentStatus, int& pagesCount);
    bool AddCheck(int PhotographerID, int CashierID, QDateTime date, double Total, int &CheckID);
    bool AddWork(int WorkTypeID, int Count, int CheckID);
    bool CreateProduct(QString ProductName, QDate ProductionDate, int WorkID);
    bool EditWork(int WorkID, int CheckID, int WorkTypeID, int Count);
    bool EditCheck(int CheckID, int PhotographerID, int CashierID, QDateTime OutDate, double Total, int StatusID);
    bool RemoveWork(int WorkID);
    bool RemoveCheck(int CheckID);
    bool GetWorkTypes(QMap<QString, double> *PriceList);
    bool GetPayStates(QStringList* lst);
    bool GetPhotographerNames(QStringList* lst);
    bool GetCashierNames(QStringList* lst);
    bool GetCheckWorks(int CheckID, QStandardItemModel* model);
    bool GetCheckWorkIDs(int CheckID, QVector<int>* IDs);
    bool GetNextCheckInQueue(int PhotographerID, int CheckNum, CheckStruct* CheckInfo);
    bool ChangeCheckState(int StatusID, int CheckID);
    bool GiveCheck(int CheckID);

private:
    QSqlDatabase db;

private:
    void closeDataBase();

    bool findStudent(QStandardItemModel* model, QString studentName, int& row);
};

#endif
