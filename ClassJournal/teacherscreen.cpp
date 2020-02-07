#include "teacherscreen.h"
#include "ui_teacherscreen.h"
#include <QDate>
#include <QMessageBox>

#include "adddatedialog.h"
#include "addstudentdialog.h"
#include "statisticssettings.h"
#include "averagestatisticssettings.h"

TeacherScreen::TeacherScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TeacherScreen)
{
    ui->setupUi(this);

    // На старте программы нужно:
    // Найти данные о преподе | +
    // Найти все группы данного преподавателя | почти +
    // Заполнить таблицу по первой группе комбо-бокса за текущий месяц/год. +
}

TeacherScreen::~TeacherScreen()
{
    delete ui;
}

void TeacherScreen::recieveData(int TeacherID, QString TeacherName, Database* db)
{
    this->TeacherID = TeacherID;
    this->TeacherName = TeacherName;
    this->db = db;

    model = new QStandardItemModel();
    avgModel = new QStandardItemModel();

    connect(model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onTableEdited(QStandardItem*)));

    setWindowTitle("Журнал. Пользователь: " + TeacherName);
    db->FindTeachersSubjects(TeacherName, subjectsMap);
    SomethingChanged = true;
    ui->SubjectNameComboBox->addItems(subjectsMap.values());
    updateTable();

    SomethingChanged = true;
    ui->ToDateEdit->setDate(QDate::currentDate());
    ui->ToDateEdit->setMaximumDate(QDate::currentDate());
}

void TeacherScreen::receiveNewDate(QDate newDate)
{
    int nextNum = 0;
    int subjectID = subjectsMap.key(ui->SubjectNameComboBox->currentText());

    model->insertColumn(model->columnCount());
    model->setHorizontalHeaderItem(model->columnCount()-1, new QStandardItem(newDate.toString("yyyy-MM-dd")));

    for (int i = 0; i < model->columnCount(); i++) {
        if (model->horizontalHeaderItem(i)->text() == model->horizontalHeaderItem(model->columnCount()-1)->text()) {
            nextNum++;
        }
    }

    QList<int> keys = studentsMap.keys();
    db->InsertLesson(nextNum, newDate.toString("yyyy-MM-dd"), subjectID, keys);

    updateTable();
}

void TeacherScreen::receiveNewStudent(QString newStudentName)
{
    int currentGroup = groupsMap.key(ui->GroupNameComboBox->currentText());
    int studentID;
    db->AddStudent(currentGroup, newStudentName, studentID);

    QVector<QString> dates;
    QVector<int> nums;
    QVector<int> subjectIDs;
    db->GetStudentLessonsDates(currentGroup, dates, nums, subjectIDs);
    for (int i = 0; i < dates.count(); i++) {
        db->AddStudentLesson(nums[i], dates[i], subjectIDs[i], studentID);
    }

    updateTable();
}

void TeacherScreen::on_ExitAction_triggered()
{
    QApplication::exit();
}

void TeacherScreen::on_ChangeUserAction_triggered()
{
    ((QWidget*)parent())->show(); // Старый стиль каста :(
    this->close();
}

void TeacherScreen::on_StatisticsAction_triggered()
{
    StatisticsSettings* settings = new StatisticsSettings(this);
    connect(this, SIGNAL(sendData(Database*, QMap<int, QString>&, int, QMap<int, QString>&, int, QMap<int, QString>&, int, QDate, QDate, int)),
            settings, SLOT(receiveData(Database*, QMap<int, QString>&, int, QMap<int, QString>&, int, QMap<int, QString>&, int, QDate, QDate, int)));

    int currentSubject = subjectsMap.key(ui->SubjectNameComboBox->currentText());
    int currentGroup = groupsMap.key(ui->GroupNameComboBox->currentText());
    int row = ui->MarksTable->currentIndex().row();
    int currentStudent = studentsMap.key(model->verticalHeaderItem(row == -1 ? 0 : row)->text());

    emit sendData(db, subjectsMap, currentSubject, groupsMap, currentGroup,
                  studentsMap, currentStudent, ui->FromDateEdit->date(), ui->ToDateEdit->date(), TeacherID);
    settings->setAttribute(Qt::WA_ShowModal);
    settings->setAttribute(Qt::WA_DeleteOnClose);
    settings->show();
}

void TeacherScreen::on_AddDayBtn_clicked()
{
    AddDateDialog* dialog = new AddDateDialog(this);
    connect(dialog, SIGNAL(sendNewDate(QDate)), this, SLOT(receiveNewDate(QDate)));
    dialog->setAttribute(Qt::WA_ShowModal);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void TeacherScreen::onTableEdited(QStandardItem* item)
{
    bool success;
    int mark = item->text().toInt(&success, 10);
    if ((!success) && (item->text() == "н")) {
        mark = 0;
        success = true;
    }
    else if (((!success) && (item->text() == " ")) || ((!success) && (item->text() == ""))) {
        mark = 1;
        success = true;
    }
    else if (!success || mark > 5) {
        //QMessageBox msgBox;
        QMessageBox::warning(this, "Ошибка ввода", "Введено неверное значение в ячейке " + QString::number(item->column() + 1) + " строки " + QString::number(item->row() + 1) + ". Пожалуйста введите верное значение.");
        //msgBox.exec();
    }

    if (success && mark <= 5) {
        int nextNum = 0;
        int subjectID = subjectsMap.key(ui->SubjectNameComboBox->currentText());

        for (int i = 0; i <= item->column(); i++) {
            if (model->horizontalHeaderItem(i)->text() == model->horizontalHeaderItem(item->column())->text()) {
                nextNum++;
            }
        }

        db->UpdateLesson(nextNum, model->horizontalHeaderItem(item->column())->text(), subjectID, studentsMap.key(model->verticalHeaderItem(item->row())->text()), mark);

        updateTable();
    }
    else {
        //QModelIndex index = item->index();
        updateTable(); // Можно сделать как-то возвращение к прошлому значению.
        // item->setText(" ");
//        ui->MarksTable->selectColumn(index.column());
//        ui->MarksTable->selectRow(index.row());
    }
}

void TeacherScreen::on_AverageStatisticsAction_triggered()
{
    AverageStatisticsSettings* settings = new AverageStatisticsSettings(this);
    connect(this, SIGNAL(sendAvgData(Database*, QMap<int, QString>&, int, QMap<int, QString>&, int, QDate, QDate, int)),
            settings, SLOT(receiveData(Database*, QMap<int, QString>&, int, QMap<int, QString>&, int, QDate, QDate, int)));

    int currentSubject = subjectsMap.key(ui->SubjectNameComboBox->currentText());
    int currentGroup = groupsMap.key(ui->GroupNameComboBox->currentText());

    emit sendAvgData(db, subjectsMap, currentSubject, groupsMap, currentGroup, ui->FromDateEdit->date(), ui->ToDateEdit->date(), TeacherID);
    settings->setAttribute(Qt::WA_ShowModal);
    settings->setAttribute(Qt::WA_DeleteOnClose);
    settings->show();
}

// Update TableView
void TeacherScreen::updateTable() {
    int groupID = groupsMap.key(ui->GroupNameComboBox->currentText());

    disconnect(model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onTableEdited(QStandardItem*)));

    model->clear();
    studentsMap.clear();
    db->FindGroupsStudents(groupID, model, studentsMap);
    db->FindMarks(groupsMap.key(ui->GroupNameComboBox->currentText()), subjectsMap.key(ui->SubjectNameComboBox->currentText()), TeacherID, ui->FromDateEdit->date(), ui->ToDateEdit->date(), model);
    ui->MarksTable->setModel(model);

    connect(model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onTableEdited(QStandardItem*)));

    QMap<int, qreal> AvgMarks;

    db->FindAvgMarks(groupsMap.key(ui->GroupNameComboBox->currentText()), subjectsMap.key(ui->SubjectNameComboBox->currentText()), TeacherID, ui->FromDateEdit->date(), ui->ToDateEdit->date(), AvgMarks);

    avgModel->clear();
    avgModel->insertColumn(0);
    avgModel->setHorizontalHeaderItem(0, new QStandardItem("Средний балл"));
    for (int i = 0; i < model->rowCount(); i++) {
        avgModel->insertRow(i);
        avgModel->setItem(i, new QStandardItem(QString::number(AvgMarks.value(studentsMap.key(model->verticalHeaderItem(i)->text())), 'f', 2)));
    }
    ui->AvgMarksTable->setModel(avgModel);

}

void TeacherScreen::on_RemoveDayBtn_clicked()
{
    if (ui->MarksTable->selectionModel()->hasSelection()) {
        int col = ui->MarksTable->currentIndex().column();
        int currentSubject = subjectsMap.key(ui->SubjectNameComboBox->currentText());
        int currentGroup = groupsMap.key(ui->GroupNameComboBox->currentText());
        QString date = model->horizontalHeaderItem(col)->text();
        int nextNum = 0;
        for (int i = 0; i <= col; i++) {
            if (model->horizontalHeaderItem(i)->text() == model->horizontalHeaderItem(col)->text()) {
                nextNum++;
            }
        }

        db->RemoveDay(nextNum, date, currentSubject, currentGroup);

        updateTable(); // TODO: Можно самому удалять колонку, если удачно прошло, чтобы не делать запрос
    }
    else {
        QMessageBox::warning(this, "Ошибка удаления", "Не выбрана ячейка дня для удаления");
    }
}

void TeacherScreen::on_AddStudentBtn_clicked()
{
    AddStudentDialog* dialog = new AddStudentDialog(this);
    connect(dialog, SIGNAL(sendNewStudent(QString)), this, SLOT(receiveNewStudent(QString)));
    dialog->setAttribute(Qt::WA_ShowModal);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void TeacherScreen::on_RemoveStudentBtn_clicked()
{
    if (ui->MarksTable->selectionModel()->hasSelection()) {
        int currentGroup = groupsMap.key(ui->GroupNameComboBox->currentText());
        int row = ui->MarksTable->currentIndex().row();
        int currentStudent = studentsMap.key(model->verticalHeaderItem(row)->text(), -1);
        db->RemoveStudent(currentGroup, currentStudent);
        updateTable();
    }
    else if (ui->AvgMarksTable->selectionModel()->hasSelection()) {
        int currentGroup = groupsMap.key(ui->GroupNameComboBox->currentText());
        int row = ui->AvgMarksTable->currentIndex().row();
        int currentStudent = studentsMap.key(model->verticalHeaderItem(row)->text(), -1);
        db->RemoveStudent(currentGroup, currentStudent);
        updateTable();
    }
    else {
        QMessageBox::warning(this, "Ошибка удаления", "Не выбрана ячейка студента для удаления");
    }
}

void TeacherScreen::on_SubjectNameComboBox_currentIndexChanged(const QString &arg1)
{
    groupsMap.clear();
    db->FindSubjectsGroups(subjectsMap.key(arg1), TeacherID, groupsMap);
    SomethingChanged = true;
    ui->GroupNameComboBox->clear();
    SomethingChanged = true;
    ui->GroupNameComboBox->addItems(groupsMap.values());
    // ui->GroupNameComboBox->setCurrentIndex(0);
    updateTable();
}

void TeacherScreen::on_GroupNameComboBox_currentIndexChanged(const QString &arg1)
{
    if (SomethingChanged) {
        SomethingChanged = false;
        return;
    }
    updateTable();
}

void TeacherScreen::on_FromDateEdit_userDateChanged(const QDate &date)
{
    if (SomethingChanged) {
        SomethingChanged = false;
        return;
    }
    updateTable();
}

void TeacherScreen::on_ToDateEdit_userDateChanged(const QDate &date)
{
    if (SomethingChanged) {
        SomethingChanged = false;
        return;
    }
    updateTable();
}

void TeacherScreen::on_action_triggered()
{
    updateTable();
}
