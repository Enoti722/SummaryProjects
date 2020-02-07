#include "averagestatisticssettings.h"
#include "ui_averagestatisticssettings.h"

#include <QtCharts/QChart>
#include <QtCharts/QLegend>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#include <QMessageBox>

#include "diagramm.h"

AverageStatisticsSettings::AverageStatisticsSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AverageStatisticsSettings)
{
    ui->setupUi(this);
}

AverageStatisticsSettings::~AverageStatisticsSettings()
{
    delete ui;
}



void AverageStatisticsSettings::receiveData(Database *db, QMap<int, QString> &subjectsMap, int currentSubject, QMap<int, QString> &groupsMap, int currentGroup, QDate fromDate, QDate toDate, int TeacherID)
{
    this->db = db;

    this->subjectsMap = subjectsMap;
    ui->SubjectNameComboBox->addItems(subjectsMap.values());
    ui->SubjectNameComboBox->setCurrentText(subjectsMap.value(currentSubject));
    this->groupsMap = groupsMap;
    ui->GroupNameComboBox->addItems(groupsMap.values());
    ui->GroupNameComboBox->setCurrentText(groupsMap.value(currentGroup));

    ui->FromDateEdit->setDate(fromDate);
    ui->ToDateEdit->setDate(toDate);

    ui->ToDateEdit->setMaximumDate(QDate::currentDate());

    this->TeacherID = TeacherID;
}

void AverageStatisticsSettings::on_SubjectCheckBox_stateChanged(int arg1)
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

void AverageStatisticsSettings::on_GroupCheckBox_stateChanged(int arg1)
{
    if (ui->GroupCheckBox->isChecked()) {
        ui->GroupNameComboBox->setEnabled(true);
    }
    else {
        ui->GroupNameComboBox->setEnabled(false);
    }
}

void AverageStatisticsSettings::on_SubjectNameComboBox_currentIndexChanged(const QString &arg1)
{
    groupsMap.clear();
    db->FindSubjectsGroups(subjectsMap.key(arg1), TeacherID, groupsMap);
    ui->GroupNameComboBox->clear();
    ui->GroupNameComboBox->addItems(groupsMap.values());
    ui->GroupNameComboBox->setCurrentIndex(0);
}

void AverageStatisticsSettings::on_GroupNameComboBox_currentIndexChanged(const QString &arg1)
{
    // Пока не надо
}

void AverageStatisticsSettings::on_BuildDiagrammBtn_clicked()
{
    int lowCount;
    int midCount;
    int highCount;

    int subjectID = ui->SubjectCheckBox->isChecked() ? subjectsMap.key(ui->SubjectNameComboBox->currentText()) : 0;
    int groupID = ui->GroupCheckBox->isChecked() ? groupsMap.key(ui->GroupNameComboBox->currentText()) : 0;
    QString fromDate = ui->FromDateEdit->date().toString("yyyy-MM-dd");
    QString toDate = ui->ToDateEdit->date().toString("yyyy-MM-dd");

    db->FindPerformanceCount(subjectID, groupID, TeacherID, fromDate, toDate, lowCount, midCount, highCount);

    int AllCount = lowCount + midCount + highCount;

    int AllSelectedCount = 0;
    if (ui->LowPerformanceCheckBox->isChecked()) {
        AllSelectedCount += lowCount;
    }
    if (ui->MidPerformanceCheckBox->isChecked()) {
        AllSelectedCount += midCount;
    }
    if (ui->HighPerformanceCheckBox->isChecked()) {
        AllSelectedCount += highCount;
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

    if (ui->LowPerformanceCheckBox->isChecked()) {
        QPieSlice *slice = series->append("Низкая", qreal(float(lowCount)/AllCount*100));
        slice->setLabelFont(lab);
        slice->setLabelBrush(QBrush(Qt::black));
        slice->setPen(QPen(Qt::black, 2));
        slice->setBrush(Qt::red);
        slice->setLabel("Низкая - " + QString::number(qreal(float(lowCount)/AllCount*100), 'f', 2) + "%");
    }
    if (ui->MidPerformanceCheckBox->isChecked()) {
        QPieSlice *slice = series->append("Средняя", qreal(float(midCount)/AllCount*100));
        slice->setLabelFont(lab);
        slice->setLabelBrush(QBrush(Qt::black));
        slice->setPen(QPen(Qt::black, 2));
        slice->setBrush(Qt::yellow);
        slice->setLabel("Средняя - " + QString::number(qreal(float(midCount)/AllCount*100), 'f', 2) + "%");
    }
    if (ui->HighPerformanceCheckBox->isChecked()) {
        QPieSlice *slice = series->append("Высокая", qreal(float(highCount)/AllCount*100));
        slice->setLabelFont(lab);
        slice->setLabelBrush(QBrush(Qt::black));
        slice->setPen(QPen(Qt::black, 2));
        slice->setBrush(Qt::green);
        slice->setLabel("Высокая - " + QString::number(qreal(float(highCount)/AllCount*100), 'f', 2) + "%");
    }

    //Добавление чарта
    chart = new QChart();

    // Добавление в чарт данных,заголовка,а также легенд.
    chart->addSeries(series);
    chart->setTitleFont(font); // Шрифт заголовка
    chart->setTitleBrush(QBrush(Qt::black)); // Цвет заголовка
    chart->setTitle("Успеваемость");
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

//
// TODO:
//

// Добавить проценты мб?
