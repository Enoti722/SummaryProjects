#ifndef AVERAGESTATISTICSSETTINGS_H
#define AVERAGESTATISTICSSETTINGS_H

#include <QMainWindow>
#include "database.h"

#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class AverageStatisticsSettings;
}

class AverageStatisticsSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit AverageStatisticsSettings(QWidget *parent = nullptr);
    ~AverageStatisticsSettings();

signals:
    void sendChartView(QChartView* chartView);

public slots:
    void receiveData(Database* db, QMap<int, QString>& subjectsMap, int currentSubject, QMap<int, QString>& groupsMap, int currentGroup, QDate fromDate, QDate toDate, int TeacherID);

private slots:
    void on_SubjectCheckBox_stateChanged(int arg1);

    void on_GroupCheckBox_stateChanged(int arg1);

    void on_SubjectNameComboBox_currentIndexChanged(const QString &arg1);

    void on_GroupNameComboBox_currentIndexChanged(const QString &arg1);

    void on_BuildDiagrammBtn_clicked();

private:
    Ui::AverageStatisticsSettings *ui;

    QMap<int, QString> subjectsMap;
    QMap<int, QString> groupsMap;

    int TeacherID;

    Database* db;
};

#endif // AVERAGESTATISTICSSETTINGS_H
