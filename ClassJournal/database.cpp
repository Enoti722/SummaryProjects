#include "database.h"
#include <QDebug>

Database::Database(QObject *parent) : QObject(parent) { }

Database::~Database()
{
    // при закрытии программы и уничтожении
    // этого класса закрываем базу  данных
    closeDataBase();
}

// Подключение к БД
bool Database::connectToDataBase()
{
    // Строка для подключения
    QString connectString = "Driver={SQL Server};";
    connectString.append("Server=87.228.2.152;");
    connectString.append("Database=Journal;");

    connectString.append("Uid=Qt;");
    // connectString.append(login);
    // connectString.append(";");

    connectString.append("Pwd=Qt2019Base;");
    // connectString.append(pass);
    // connectString.append(";");

    // Пытаемся подключиться к серверу БД
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(connectString);
    // Проверка на подключение
    if(db.open()){ // если таблица успешно открыта - сообщаем об этом
        qDebug() << "Успешное подключение";
        return true;
    } else {
        qDebug() << "Подключение не удалось. " << db.lastError();
        return false; // иначе сообщаем, что нам не удалось ее открыть
    }
}

// Функция для закрытия базы
void Database::closeDataBase()
{
    db.close();
}

bool Database::TeacherLoginQuery(QString login, QString pass, QString &name, int &id)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC CheckTeacherAccountExists :login, :pass");
    qry->bindValue(":login", login);
    qry->bindValue(":pass", pass);
    if (qry->exec()) {
        qry->first();
        if ((qry->value("login").toString() == login) && (qry->value("pass").toString() == pass)) {
            name = qry->value("name").toString();
            id = qry->value("ID").toInt();
            qDebug() << "Добро пожаловать, " << name << "!";
            return true;
        }
    }
    return false;
}

bool Database::FindTeachersSubjects(QString TeacherName, QMap<int, QString> &subjectsMap)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC FindTeachersSubjects :name");
    qry->bindValue(":name", TeacherName);
    if (qry->exec()) {
        while (qry->next()) {
            subjectsMap.insert(qry->value("ID").toInt(), qry->value("subjectName").toString());
        }
        return true;
    }
    return false;
}

bool Database::FindSubjectsGroups(int subjectID, int teacherID, QMap<int, QString> &groupsMap)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC FindSubjectsGroups :subjectID, :teacherID");
    qry->bindValue(":subjectID", subjectID);
    qry->bindValue(":teacherID", teacherID);
    if (qry->exec()) {
        while (qry->next()) {
            groupsMap.insert(qry->value("groupID").toInt(), qry->value("groupName").toString());
        }
        return true;
    }
    return false;
}

bool Database::FindGroupsStudents(int groupID, QStandardItemModel *model, QMap<int, QString> &StudentsMap) {
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC FindGroupsStudents :groupID");
    qry->bindValue(":groupID", groupID);
    if (qry->exec()) {
        while (qry->next()) {
            int newStudentID = qry->value("studentID").toInt();
            QString newStudentName = qry->value("studentName").toString();

            StudentsMap.insert(newStudentID, newStudentName);
            model->insertRow(model->rowCount());
            model->setVerticalHeaderItem(model->rowCount()-1, new QStandardItem(newStudentName));
        }
        return true;
    }
    return false;
}

bool Database::FindGroupsStudents(int groupID, QMap<int, QString> &StudentsMap) {
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC FindGroupsStudents :groupID");
    qry->bindValue(":groupID", groupID);
    if (qry->exec()) {
        while (qry->next()) {
            int newStudentID = qry->value("studentID").toInt();
            QString newStudentName = qry->value("studentName").toString();

            StudentsMap.insert(newStudentID, newStudentName);
        }
        return true;
    }
    return false;
}

bool Database::FindMarks(int groupID, int subjectID, int teacherID, QDate fromDate, QDate toDate, QStandardItemModel *model)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC FindLessons :groupID, :subjectID, :teacherID, :fromDate, :toDate");
    qry->bindValue(":groupID", groupID);
    qry->bindValue(":subjectID", subjectID);
    qry->bindValue(":teacherID", teacherID);
    qry->bindValue(":fromDate", fromDate.toString("yyyy-MM-dd"));
    qry->bindValue(":toDate", toDate.toString("yyyy-MM-dd"));
    if (qry->exec()) {
        // Данные в пришедшем ответе на запрос сортированы по возрастанию даты!
        QString currentDate = "";
        QString currentNum = "";

        while (qry->next()) {
            QString newDate = qry->value("lessonDate").toString();
            QString newNum = qry->value("lessonNum").toString();
            QString newStudent = qry->value("studentName").toString();
            QChar mark = qry->value("Mark").toString()[0];


            int studentRow = 0;
            findStudent(model, newStudent, studentRow);
            if (currentDate == newDate) {
                if (currentNum != newNum) {
                    currentNum = newNum;
                    model->insertColumn(model->columnCount()); // Ещё одна пара в этот день
                }
                model->setHorizontalHeaderItem(model->columnCount()-1, new QStandardItem(newDate));
                model->setItem(studentRow, model->columnCount()-1, new QStandardItem(mark == '0' ? ' ' : mark)); // Просто марк (и ниже тоже) ? (Если нули убрали в итоге)
            }
            else if (currentDate != newDate) {
                currentDate = newDate;
                currentNum = newNum;
                model->insertColumn(model->columnCount()); // Новый день
                model->setHorizontalHeaderItem(model->columnCount()-1, new QStandardItem(newDate));
                model->setItem(studentRow, model->columnCount()-1, new QStandardItem(mark == '0' ? ' ' : mark));
            }
        }
        return true;
    }
    return false;
}

bool Database::FindAvgMarks(int groupID, int subjectID, int teacherID, QDate fromDate, QDate toDate, QMap<int, qreal> &AvgMarks)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC FindAvgMarks :groupID, :subjectID, :teacherID, :fromDate, :toDate");
    qry->bindValue(":groupID", groupID);
    qry->bindValue(":subjectID", subjectID);
    qry->bindValue(":teacherID", teacherID);
    qry->bindValue(":fromDate", fromDate.toString("yyyy-MM-dd"));
    qry->bindValue(":toDate", toDate.toString("yyyy-MM-dd"));
    if (qry->exec()) {
        while (qry->next()) {
            AvgMarks.insert(qry->value("ID").toInt(), qry->value("mark").toReal());
        }
        return true;
    }
    return false;
}

bool Database::CheckDateAndFindNums(int groupID, int subjectID, int teacherID, QString checkingDate, int &nextNum)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC CheckDateAndFindNums :groupID, :subjectID, :teacherID, :checkingDate");
    qry->bindValue(":groupID", groupID);
    qry->bindValue(":subjectID", subjectID);
    qry->bindValue(":teacherID", teacherID);
    qry->bindValue(":checkingDate", checkingDate);
    if (qry->exec()) {
        // Данные в пришедшем ответе на запрос сортированы по возрастанию номера пары
        qry->first();
        QVariant var = qry->value("num");
        nextNum = var.isValid() && var.toInt() > nextNum ? nextNum : var.toInt();
        return true;
    }
    return false;
}

bool Database::UpdateLesson(int num, QString date, int subjectID, int studentID, int mark)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC UpdateLesson :num, :date, :subjectID, :studentID, :mark");
    qry->bindValue(":num", num);
    qry->bindValue(":date", date);
    qry->bindValue(":subjectID", subjectID);
    qry->bindValue(":studentID", studentID);
    qry->bindValue(":mark", mark);
    if (qry->exec()) {
        return true;
    }
    return false;
}

bool Database::InsertLesson(int num, QString date, int subjectID, QList<int> &studentsKeys)
{
    for (int i = 0; i < studentsKeys.length(); i++) {
        QSqlQuery* qry = new QSqlQuery();
        qry->prepare("EXEC InsertLesson :num, :date, :subjectID, :studentID");
        qry->bindValue(":num", num);
        qry->bindValue(":date", date);
        qry->bindValue(":subjectID", subjectID);
        qry->bindValue(":studentID", studentsKeys[i]);
        if (!qry->exec()) {
            return false;
        }
    }
    return true;
}

bool Database::FindStatisticsData(int subjectID, int groupID, int studentID, int teacherID, QString fromDate, QString toDate, int &MarksCount, int &LessonsCount, int&StudentsCount)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC FindStatisticsData :subjectID, :groupID, :studentID, :teacherID, :fromDate, :toDate");
    qry->bindValue(":subjectID", subjectID);
    qry->bindValue(":groupID", groupID);
    qry->bindValue(":studentID", studentID);
    qry->bindValue(":teacherID", teacherID);
    qry->bindValue(":fromDate", fromDate);
    qry->bindValue(":toDate", toDate);
    if (qry->exec()) {
        if (qry->first()) {
            MarksCount = qry->value("MarksCount").toInt();
            LessonsCount = qry->value("LessonsCount").toInt();
            StudentsCount = qry->value("StudentsCount").toInt();
            return true;
        }
    }
    return false;
}

bool Database::FindMarksForStatistics(int subjectID, int groupID, int studentID, int teacherID, QString fromDate, QString toDate, QMap<int, int> &MarksMap)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC FindMarksForStatistics :subjectID, :groupID, :studentID, :teacherID, :fromDate, :toDate");
    qry->bindValue(":subjectID", subjectID);
    qry->bindValue(":groupID", groupID);
    qry->bindValue(":studentID", studentID);
    qry->bindValue(":teacherID", teacherID);
    qry->bindValue(":fromDate", fromDate);
    qry->bindValue(":toDate", toDate);
    if (qry->exec()) {
        while (qry->next()) {
            MarksMap.insert(qry->value("MarkTypeID").toInt(), qry->value("MarksCount").toInt());
        }
        return true;
    }
    return false;
}

bool Database::FindPerformanceCount(int subjectID, int groupID, int teacherID, QString fromDate, QString toDate, int &lowCount, int &midCount, int &highCount)
{
    lowCount = 0;
    midCount = 0;
    highCount = 0;
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC FindPerformanceCount :subjectID, :groupID, :teacherID, :fromDate, :toDate");
    qry->bindValue(":subjectID", subjectID);
    qry->bindValue(":groupID", groupID);
    qry->bindValue(":teacherID", teacherID);
    qry->bindValue(":fromDate", fromDate);
    qry->bindValue(":toDate", toDate);
    if (qry->exec()) {
        while (qry->next()) {
            qreal currentAvg = qry->value("avgMark").toReal();
            if (currentAvg == 0 || currentAvg == 1) { lowCount++; }
            else if (currentAvg >= 0 && currentAvg <= 3) { lowCount++; }
            else if (currentAvg > 3 && currentAvg < 4) { midCount++; }
            else if (currentAvg >= 4 && currentAvg <= 5) { highCount++; }
        }
        return true;
    }
    return false;
}

bool Database::RemoveDay(int num, QString date, int subjectID, int groupID)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC RemoveDay :num, :date, :subjectID, :groupID");
    qry->bindValue(":num", num);
    qry->bindValue(":date", date);
    qry->bindValue(":subjectID", subjectID);
    qry->bindValue(":groupID", groupID);
    if (!qry->exec()) {
        return false;
    }
    return true;
}

bool Database::AddStudent(int groupID, QString name, int &studentID)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC AddStudent :groupID, :name");
    qry->bindValue(":groupID", groupID);
    qry->bindValue(":name", name);
    if (qry->exec()) {
        if (qry->first()) {
            studentID = qry->value("ID").toInt();
            return true;
        }
    }
    return false;
}

bool Database::GetStudentLessonsDates(int groupID, QVector<QString> &dates, QVector<int> &nums, QVector<int> &subjectIDs)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC GetStudentLessonsDates :groupID");
    qry->bindValue(":groupID", groupID);
    if (qry->exec()) {
        while (qry->next()) {
            dates.append(qry->value("Date").toDate().toString("yyyy-MM-dd"));
            nums.append(qry->value("Num").toInt());
            subjectIDs.append(qry->value("SubjectID").toInt());
        }
        return true;
    }
    return false;
}

bool Database::AddStudentLesson(int num, QString date, int subjectID, int studentID)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC AddStudentLesson :num, :date, :subjectID, :studentID");
    qry->bindValue(":num", num);
    qry->bindValue(":date", date);
    qry->bindValue(":subjectID", subjectID);
    qry->bindValue(":studentID", studentID);
    if (qry->exec()) {
        return true;
    }
    return false;
}

bool Database::RemoveStudent(int groupID, int studentID)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC RemoveStudent :groupID, :studentID");
    qry->bindValue(":groupID", groupID);
    qry->bindValue(":studentID", studentID);
    if (qry->exec()) {
        return true;
    }
    return false;
}

bool Database::findStudent(QStandardItemModel* model, QString studentName, int& row) {
    for (int i = 0; i < model->rowCount(); i++) {
        if (model->verticalHeaderItem(i)->text() == studentName) {
            row = i;
            return true;
        }
    }
    return false;
}

//
// TODO:
//

// Проверки на qry.first() == true
// Проверки удачного исполнения каждой из этих функций на месте их вызова

