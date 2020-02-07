#ifndef STATISTICSSETTINGS_H
#define STATISTICSSETTINGS_H

#include <QMainWindow>
#include "database.h"

#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class StatisticsSettings;
}

class StatisticsSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit StatisticsSettings(QWidget *parent = nullptr);
    ~StatisticsSettings();

signals:
    void sendChartView(QChartView* chartView);

public slots:
    void receiveData(Database* db, QMap<int, QString>& subjectsMap, int currentSubject, QMap<int, QString>& groupsMap, int currentGroup, QMap<int, QString>& studentsMap, int currentStudent, QDate fromDate, QDate toDate, int TeacherID);

private slots:
    void on_SubjectCheckBox_stateChanged(int arg1);

    void on_GroupCheckBox_stateChanged(int arg1);

    void on_StudentCheckBox_stateChanged(int arg1);

    void on_SubjectNameComboBox_currentIndexChanged(const QString &arg1);

    void on_GroupNameComboBox_currentIndexChanged(const QString &arg1);

    void on_StudentNameComboBox_currentIndexChanged(const QString &arg1);

    void on_BuildDiagrammBtn_clicked();

private:
    Ui::StatisticsSettings *ui;

    QMap<int, QString> subjectsMap;
    QMap<int, QString> groupsMap;
    QMap<int, QString> studentsMap;

    int TeacherID;

    Database* db;
};

#endif // STATISTICSSETTINGS_H
