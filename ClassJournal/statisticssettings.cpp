#include "statisticssettings.h"
#include "ui_statisticssettings.h"

#include <QtCharts/QChart>
#include <QtCharts/QLegend>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#include <QMessageBox>

#include "diagramm.h"

StatisticsSettings::StatisticsSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StatisticsSettings)
{
    ui->setupUi(this);
}

StatisticsSettings::~StatisticsSettings()
{
    delete ui;
}

void StatisticsSettings::receiveData(Database *db, QMap<int, QString> &subjectsMap, int currentSubject, QMap<int, QString> &groupsMap, int currentGroup, QMap<int, QString> &studentsMap, int currentStudent, QDate fromDate, QDate toDate, int TeacherID)
{
    this->db = db;

    this->subjectsMap = subjectsMap;
    ui->SubjectNameComboBox->addItems(subjectsMap.values());
    ui->SubjectNameComboBox->setCurrentText(subjectsMap.value(currentSubject));
    this->groupsMap = groupsMap;
    ui->GroupNameComboBox->addItems(groupsMap.values());
    ui->GroupNameComboBox->setCurrentText(groupsMap.value(currentGroup));
    this->studentsMap = studentsMap;
    ui->StudentNameComboBox->clear();
    ui->StudentNameComboBox->addItems(studentsMap.values());
    ui->StudentNameComboBox->setCurrentText(studentsMap.value(currentStudent));

    ui->FromDateEdit->setDate(fromDate);
    ui->ToDateEdit->setDate(toDate);

    ui->ToDateEdit->setMaximumDate(QDate::currentDate());

    this->TeacherID = TeacherID;
}

void StatisticsSettings::on_SubjectCheckBox_stateChanged(int arg1)
{
    if (ui->SubjectCheckBox->isChecked()) {
        ui->SubjectNameComboBox->setEnabled(true);
        ui->GroupCheckBox->setEnabled(true);
    }
    else {
        ui->SubjectNameComboBox->setEnabled(false);
        ui->GroupCheckBox->setEnabled(false);
    }
}

void StatisticsSettings::on_GroupCheckBox_stateChanged(int arg1)
{
    if (ui->GroupCheckBox->isChecked()) {
        ui->GroupNameComboBox->setEnabled(true);
        ui->StudentCheckBox->setEnabled(true);
    }
    else {
        ui->GroupNameComboBox->setEnabled(false);
        ui->StudentCheckBox->setEnabled(false);
    }
}

void StatisticsSettings::on_StudentCheckBox_stateChanged(int arg1)
{
    if (ui->StudentCheckBox->isChecked()) {
        ui->StudentNameComboBox->setEnabled(true);
    }
    else {
        ui->StudentNameComboBox->setEnabled(false);
    }
}

void StatisticsSettings::on_SubjectNameComboBox_currentIndexChanged(const QString &arg1)
{
    groupsMap.clear();
    db->FindSubjectsGroups(subjectsMap.key(arg1), TeacherID, groupsMap);
    ui->GroupNameComboBox->clear();
    ui->GroupNameComboBox->addItems(groupsMap.values());
    ui->GroupNameComboBox->setCurrentIndex(0);
}

void StatisticsSettings::on_GroupNameComboBox_currentIndexChanged(const QString &arg1)
{
    studentsMap.clear();
    db->FindGroupsStudents(groupsMap.key(arg1), studentsMap);
    ui->StudentNameComboBox->clear();
    ui->StudentNameComboBox->addItems(studentsMap.values());
    ui->StudentNameComboBox->setCurrentIndex(0);
}

void StatisticsSettings::on_StudentNameComboBox_currentIndexChanged(const QString &arg1)
{
    // Пока не надо.
}

void StatisticsSettings::on_BuildDiagrammBtn_clicked()
{
    QMap<int, int> MarksMap;

    int subjectID = ui->SubjectCheckBox->isChecked() ? subjectsMap.key(ui->SubjectNameComboBox->currentText()) : 0;
    int groupID = ui->GroupCheckBox->isChecked() ? groupsMap.key(ui->GroupNameComboBox->currentText()) : 0;
    int studentID = ui->StudentCheckBox->isChecked() ? studentsMap.key(ui->StudentNameComboBox->currentText()) : 0;
    QString fromDate = ui->FromDateEdit->date().toString("yyyy-MM-dd");
    QString toDate = ui->ToDateEdit->date().toString("yyyy-MM-dd");

    db->FindMarksForStatistics(subjectID, groupID, studentID, TeacherID, fromDate, toDate, MarksMap);

    int AllCount = 0;
    int AllSelectedCount = 0;
    for (int i : MarksMap.keys()) {
        AllCount += MarksMap.value(i);
        QList<QCheckBox*> CheckBoxList = ui->groupBox->findChildren<QCheckBox*>();
        if (CheckBoxList.at(i)->isChecked()) {
            AllSelectedCount += MarksMap.value(i);
        }
    }

    if (AllSelectedCount == 0) {
        QMessageBox::warning(this, "Ошибка ввода", "Нет оценок по выбранным параметрам");
        return;
    }


    QChart *chart;
    QFont font;
    font.setPixelSize(22);
    QFont lab;
    lab.setPixelSize(15);
    QPieSeries *series = new QPieSeries();

    if (ui->NotHereCheckBox->isChecked()) {
        QPieSlice *slice = series->append("Н - Неявка", qreal(float(MarksMap.value(0))/AllCount));
        slice->setLabelFont(lab);
        slice->setLabelBrush(QBrush(Qt::black));
        slice->setPen(QPen(Qt::black, 2));
        slice->setBrush(Qt::gray);
    }
    if (ui->HereCheckBox->isChecked()) {
        QPieSlice *slice = series->append("Присутствовал", qreal(float(MarksMap.value(1))/AllCount));
        slice->setLabelFont(lab);
        slice->setLabelBrush(QBrush(Qt::black));
        slice->setPen(QPen(Qt::black, 2));
        slice->setBrush(Qt::blue);
    }
    if (ui->Mark2CheckBox->isChecked()) {
        QPieSlice *slice = series->append("2 - Неудовлетворительно", qreal(float(MarksMap.value(2))/AllCount));
        slice->setLabelFont(lab);
        slice->setLabelBrush(QBrush(Qt::black));
        slice->setPen(QPen(Qt::black, 2));
        slice->setBrush(Qt::red);
    }
    if (ui->Mark3CheckBox->isChecked()) {
        QPieSlice *slice = series->append("3 - Удовлетворительно", qreal(float(MarksMap.value(3))/AllCount));
        slice->setLabelFont(lab);
        slice->setLabelBrush(QBrush(Qt::black));
        slice->setPen(QPen(Qt::black, 2));
        slice->setBrush(Qt::yellow);
    }
    if (ui->Mark4CheckBox->isChecked()) {
        QPieSlice *slice = series->append("4 - Хорошо", qreal(float(MarksMap.value(4))/AllCount));
        slice->setLabelFont(lab);
        slice->setLabelBrush(QBrush(Qt::black));
        slice->setPen(QPen(Qt::black, 2));
        slice->setBrush(Qt::cyan);
    }
    if (ui->Mark5CheckBox->isChecked()) {
        QPieSlice *slice = series->append("5 - Отлично", qreal(float(MarksMap.value(5))/AllCount));
        slice->setLabelFont(lab);
        slice->setLabelBrush(QBrush(Qt::black));
        slice->setPen(QPen(Qt::black, 2));
        slice->setBrush(Qt::green);
    }

    //Добавление чарта
    chart = new QChart();

    // Добавление в чарт данных,заголовка,а также легенд.
    chart->addSeries(series);
    chart->setTitleFont(font); // Шрифт заголовка
    chart->setTitleBrush(QBrush(Qt::black)); // Цвет заголовка
    chart->setTitle("Диаграмма успеваемости");
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->legend()->setFont(lab);
    chart->legend()->setBrush(QBrush(Qt::black));

    // Показ дисплея чарта
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    Diagramm* diagramm = new Diagramm(this);
    connect(this, SIGNAL(sendChartView(QChartView*)), diagramm, SLOT(receiveChartView(QChartView*)));
    emit sendChartView(chartView);
    diagramm->setAttribute(Qt::WA_ShowModal);
    diagramm->setAttribute(Qt::WA_DeleteOnClose);
    diagramm->show();
}
