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
    connectString.append("Database=PhotoStudio;");

    connectString.append("Uid=Qt;");
    // connectString.append(login);
    // connectString.append(";");

    connectString.append("Pwd=Qt2019Base;");
    // connectString.append(pass);
    // connectString.append(";");

    // Пытаемся подключиться к серверу БД
    db = QSqlDatabase::addDatabase("QODBC");
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

bool Database::WorkerLogin(QString login, QString pass, QString &name, int &id, int &WorkerType, int& ProfID)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC CheckWorkerAccountExists :login, :pass");
    qry->bindValue(":login", login);
    qry->bindValue(":pass", pass);
    if (qry->exec()) {
        qry->first();
        if ((qry->value("Login").toString() == login) && (qry->value("Pass").toString() == pass)) {
            name = qry->value("Name").toString();
            id = qry->value("ID").toInt();
            WorkerType = qry->value("UserTypeID").toInt();
            ProfID = qry->value("ProfID").toInt();
            qDebug() << "Добро пожаловать, " << name << "!";
            return true;
        }
    }
    return false;
}

bool Database::FindChecks(QDate fromDate, QDate toDate, int paymentStatus, int page, QStandardItemModel *model)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC FindChecks :fromDate, :toDate, :paymentStatus, :page");
    qry->bindValue(":fromDate", fromDate.toString("yyyy-MM-dd"));
    qry->bindValue(":toDate", toDate.toString("yyyy-MM-ddT23:59:59.999"));
    qry->bindValue(":paymentStatus", paymentStatus);
    qry->bindValue(":page", page);
    if (qry->exec()) {
        while (qry->next()) {
            QList<QStandardItem*> lst;
            lst.append(new QStandardItem(qry->value("ID").toString()));
            lst.append(new QStandardItem(qry->value("PhotographerName").toString()));
            lst.append(new QStandardItem(qry->value("CashierName").toString()));
            lst.append(new QStandardItem(qry->value("OutDate").toDateTime().toString()));
            lst.append(new QStandardItem(QString::number(qry->value("Total").toDouble())));
            lst.append(new QStandardItem(qry->value("StatusName").toString()));
            model->appendRow(lst);
        }
        return true;
    }
    return false;
}

bool Database::FindChecksForReport(QDate fromDate, QDate toDate, int paymentStatus, int page, QSqlQuery *queryRef)
{
    queryRef->prepare("EXEC FindChecks :fromDate, :toDate, :paymentStatus, :page");
    queryRef->bindValue(":fromDate", fromDate.toString("yyyy-MM-dd"));
    queryRef->bindValue(":toDate", toDate.toString("yyyy-MM-ddT23:59:59.999"));
    queryRef->bindValue(":paymentStatus", paymentStatus);
    queryRef->bindValue(":page", page);
    if (queryRef->exec()) {
        return true;
    }
    return false;
}

QStringList Database::QueryDataToString(QSqlQuery *query)
{
    QStringList res;
    res << query->value("ID").toString();
    res << query->value("PhotographerName").toString();
    res << query->value("CashierName").toString();
    res << query->value("OutDate").toDateTime().toString();
    res << query->value("Total").toString();
    // res << query->value("StatusName").toString();
    return res;
}

bool Database::GetPagesCount(QDate fromDate, QDate toDate, int paymentStatus, int &pagesCount)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC GetPagesCount :fromDate, :toDate, :paymentStatus");
    qry->bindValue(":fromDate", fromDate.toString("yyyy-MM-dd"));
    qry->bindValue(":toDate", toDate.toString("yyyy-MM-ddT23:59:59.999"));
    qry->bindValue(":paymentStatus", paymentStatus);
    if (qry->exec()) {
        while (qry->next()) {
            pagesCount = qry->value("Count").toInt();
        }
        return true;
    }
    return false;
}

bool Database::AddCheck(int PhotographerID, int CashierID, QDateTime Date, double Total, int &CheckID)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC AddNewCheck :PhotographerID, :CashierID, :OutDate, :Total, :StatusID");
    qry->bindValue(":PhotographerID", PhotographerID);
    qry->bindValue(":CashierID", CashierID);
    qry->bindValue(":OutDate", Date.toString("yyyy-MM-ddThh:mm:ss.000"));
    qry->bindValue(":Total", Total);
    qry->bindValue(":StatusID", 1);
    if (qry->exec()) {
        qry->first();
        CheckID = qry->value("ID").toInt();
        return true;
    }
    return false;
}

bool Database::AddWork(int WorkTypeID, int Count, int CheckID)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC AddNewWork :WorkTypeID, :Count, :CheckID");
    qry->bindValue(":WorkTypeID", WorkTypeID);
    qry->bindValue(":Count", Count);
    qry->bindValue(":CheckID", CheckID);
    if (qry->exec()) {
        return true;
    }
    return false;
}

bool Database::CreateProduct(QString ProductName, QDate ProductionDate, int WorkID)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC CreateProduct :ProductName, :ProductionDate, :WorkID");
    qry->bindValue(":ProductName", ProductName);
    qry->bindValue(":ProductionDate", ProductionDate.toString("yyyy-MM-dd"));
    qry->bindValue(":WorkID", WorkID);
    if (qry->exec()) {
        return true;
    }
    return false;
}

bool Database::GetWorkTypes(QMap<QString, double> *PriceList)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC GetWorkTypes");
    if (qry->exec()) {
        while (qry->next()) {
            PriceList->insert(qry->value("Name").toString(), qry->value("Price").toInt());
        }
        return true;
    }
    return false;
}

bool Database::GetPayStates(QStringList *lst)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC GetPaymentStates");
    if (qry->exec()) {
        while (qry->next()) {
            lst->append(qry->value("Name").toString());
        }
        return true;
    }
    return false;
}

bool Database::GetPhotographerNames(QStringList *lst)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC GetPhotographerNames");
    if (qry->exec()) {
        while (qry->next()) {
            lst->append(qry->value("Name").toString());
        }
        return true;
    }
    return false;
}

bool Database::GetCashierNames(QStringList *lst)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC GetCashierNames");
    if (qry->exec()) {
        while (qry->next()) {
            lst->append(qry->value("Name").toString());
        }
        return true;
    }
    return false;
}

bool Database::GetCheckWorks(int CheckID, QStandardItemModel *model)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC GetCheckWorks :CheckID");
    qry->bindValue(":CheckID", CheckID);
    if (qry->exec()) {
        while (qry->next()) {
            QList<QStandardItem*> lst;
            lst.append(new QStandardItem(qry->value("ID").toString()));
            lst.append(new QStandardItem(qry->value("WorkType").toString()));
            lst.append(new QStandardItem(qry->value("Count").toString()));
            lst.append(new QStandardItem(QString::number(qry->value("WorkPrice").toDouble())));
            lst.append(new QStandardItem(QString::number(qry->value("Cost").toDouble())));
            lst.append(new QStandardItem(qry->value("ProductID").toString()));
            model->appendRow(lst);
        }
        return true;
    }
    return false;
}

bool Database::GetCheckWorkIDs(int CheckID, QVector<int> *IDs)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC GetCheckWorks :CheckID");
    qry->bindValue(":CheckID", CheckID);
    if (qry->exec()) {
        while (qry->next()) {
            IDs->append(qry->value("ID").toInt());
        }
        return true;
    }
    return false;
}

bool Database::GetNextCheckInQueue(int PhotographerID, int CheckNum, CheckStruct *CheckInfo)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC GetNextCheckInQueue :PhotographerID, :CheckNum");
    qry->bindValue(":PhotographerID", PhotographerID);
    qry->bindValue(":CheckNum", CheckNum);
    if (qry->exec()) {
        if (!qry->first()) {
            return false;
        }
        CheckInfo->ID = qry->value("ID").toInt();
        CheckInfo->DateTime = qry->value("OutDate").toDateTime();
        CheckInfo->StatusID = qry->value("StatusID").toInt();
        CheckInfo->PhotographerID = PhotographerID;
        CheckInfo->CashierID = qry->value("CashierID").toInt();
        CheckInfo->CashierName = qry->value("CashierName").toString();
        return true;
    }
    return false;
}

bool Database::ChangeCheckState(int StatusID, int CheckID)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC ChangeCheckState :StatusID, :CheckID");
    qry->bindValue(":StatusID", StatusID);
    qry->bindValue(":CheckID", CheckID);
    if (qry->exec()) {
        return true;
    }
    return false;
}

bool Database::GiveCheck(int CheckID)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC GiveCheck :CheckID");
    qry->bindValue(":CheckID", CheckID);
    if (qry->exec()) {
        return true;
    }
    return false;
}

bool Database::EditWork(int WorkID, int CheckID, int WorkTypeID, int Count)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC EditWork :WorkID, :CheckID, :WorkTypeID, :Count");
    qry->bindValue(":WorkID", WorkID);
    qry->bindValue(":CheckID", CheckID);
    qry->bindValue(":WorkTypeID", WorkTypeID);
    qry->bindValue(":Count", Count);
    if (qry->exec()) {
        return true;
    }
    return false;
}

bool Database::EditCheck(int CheckID, int PhotographerID, int CashierID, QDateTime OutDate, double Total, int StatusID)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC EditCheck :CheckID, :PhotographerID, :CashierID, :OutDate, :Total, :StatusID");
    qry->bindValue(":CheckID", CheckID);
    qry->bindValue(":PhotographerID", PhotographerID);
    qry->bindValue(":CashierID", CashierID);
    qry->bindValue(":OutDate", OutDate.toString("yyyy-MM-ddThh:mm:00.000"));
    qry->bindValue(":Total", Total);
    qry->bindValue(":StatusID", StatusID);
    if (qry->exec()) {
        return true;
    }
    return false;
}

bool Database::RemoveWork(int WorkID)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC RemoveWork :WorkID");
    qry->bindValue(":WorkID", WorkID);
    if (qry->exec()) {
        return true;
    }
    return false;
}

bool Database::RemoveCheck(int CheckID)
{
    QSqlQuery* qry = new QSqlQuery();
    qry->prepare("EXEC RemoveCheck :CheckID");
    qry->bindValue(":CheckID", CheckID);
    if (qry->exec()) {
        return true;
    }
    return false;
}

//
// TODO:
//

// Проверки на qry.first() == true
// Проверки удачного исполнения каждой из этих функций на месте их вызова

