#ifndef TEACHERSCREEN_H
#define TEACHERSCREEN_H

#include <QMainWindow>
#include "database.h"

namespace Ui {
class TeacherScreen;
}

class TeacherScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit TeacherScreen(QWidget *parent = nullptr);
    ~TeacherScreen();

    void updateTable();

signals:
    void sendData(Database* db, QMap<int, QString>& subjectsMap, int currentSubject, QMap<int, QString>& groupsMap, int currentGroup, QMap<int, QString>& studentsMap, int currentStudent, QDate fromDate, QDate toDate, int TeacherID);
    void sendAvgData(Database* db, QMap<int, QString>& subjectsMap, int currentSubject, QMap<int, QString>& groupsMap, int currentGroup, QDate fromDate, QDate toDate, int TeacherID);

public slots:
    void recieveData(int TeacherID, QString TeacherName, Database* db);
    void receiveNewDate(QDate newDate);
    void receiveNewStudent(QString newStudentName);

private slots:
    void on_ExitAction_triggered();

    void on_ChangeUserAction_triggered();

    void on_StatisticsAction_triggered();

    void on_AddDayBtn_clicked();

    void onTableEdited(QStandardItem* item);

    void on_AverageStatisticsAction_triggered();

    void on_RemoveDayBtn_clicked();

    void on_AddStudentBtn_clicked();

    void on_RemoveStudentBtn_clicked();

    void on_SubjectNameComboBox_currentIndexChanged(const QString &arg1);

    void on_GroupNameComboBox_currentIndexChanged(const QString &arg1);

    void on_FromDateEdit_userDateChanged(const QDate &date);

    void on_ToDateEdit_userDateChanged(const QDate &date);

    void on_action_triggered();

private:
    Ui::TeacherScreen *ui;

    QMap<int, QString> subjectsMap;
    QMap<int, QString> groupsMap;
    QMap<int, QString> studentsMap;

    QStandardItemModel *model;
    QStandardItemModel* avgModel;

    bool SomethingChanged = false;

    int TeacherID;
    QString TeacherName;
    Database* db;
};

#endif // TEACHERSCREEN_H
