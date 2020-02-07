#ifndef DIAGRAMM_H
#define DIAGRAMM_H

#include <QMainWindow>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Diagramm;
}

class Diagramm : public QMainWindow
{
    Q_OBJECT

public:
    explicit Diagramm(QWidget *parent = nullptr);
    ~Diagramm();

public slots:
    void receiveChartView(QChartView* chartView);

private:
    Ui::Diagramm *ui;
};

#endif // DIAGRAMM_H
