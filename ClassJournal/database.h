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

// Директивы имен таблиц и их полей

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    bool connectToDataBase();
    bool TeacherLoginQuery(QString login, QString pass, QString &name, int &id);
    bool FindTeachersSubjects(QString TeacherName, QMap<int, QString> &subjectsMap);
    bool FindSubjectsGroups(int subjectID, int teacherID, QMap<int, QString> &groupsMap);
    bool FindGroupsStudents(int groupID, QStandardItemModel *model, QMap<int, QString> &StudentsMap);
    bool FindGroupsStudents(int groupID, QMap<int, QString> &StudentsMap);
    bool FindMarks(int groupID, int subjectID, int teacherID, QDate fromDate, QDate toDate, QStandardItemModel* model);
    bool FindAvgMarks(int groupID, int subjectID, int teacherID, QDate fromDate, QDate toDate, QMap<int, qreal> &AvgMarks);

    bool CheckDateAndFindNums(int groupID, int subjectID, int teacherID, QString checkingDate, int &nextNum);

    bool UpdateLesson(int num, QString date, int subjectID, int studentID, int mark);
    bool InsertLesson(int num, QString date, int subjectID, QList<int> &studentsKeys);


    bool FindStatisticsData(int subjectID, int groupID, int studentID, int teacherID, QString fromDate, QString toDate, int &MarksCount, int &LessonsCount, int&StudentsCount);
    bool FindMarksForStatistics(int subjectID, int groupID, int studentID, int teacherID, QString fromDate, QString toDate, QMap<int, int> &MarksMap);
    bool FindPerformanceCount(int subjectID, int groupID, int teacherID, QString fromDate, QString toDate, int &lowCount, int &midCount, int &highCount);

    bool RemoveDay(int num, QString date, int subjectID, int groupID);
    bool AddStudent(int groupID, QString name, int &studentID);
    bool GetStudentLessonsDates(int groupID, QVector<QString> &dates, QVector<int> &nums, QVector<int> &subjectIDs);
    bool AddStudentLesson(int num, QString date, int subjectID, int studentID);
    bool RemoveStudent(int groupID, int studentID);

private:
    QSqlDatabase db;

private:
    void closeDataBase();

    bool findStudent(QStandardItemModel* model, QString studentName, int& row);
};

#endif
