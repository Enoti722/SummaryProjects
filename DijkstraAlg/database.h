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

// Директивы имен таблиц и их полей

#define TABLE_POINTS            "Points"
#define TABLE_LENGTHS           "Lengths"
#define TABLE_SETTINGS          "Settings"

#define START_POINT             "StartPoint"
#define END_POINT               "EndPoint"
#define LENGTH                  "Length"
#define IS_ORIENTED             "IsOriented"
#define ALL_POINTS              "AllPoints"
#define POINTS_COUNT            "PointsCount"

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    void connectToDataBase(QString dbName);

private:
    QSqlDatabase      db;
    QString DataBaseName;

private:
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createDeviceTable();
};

#endif
