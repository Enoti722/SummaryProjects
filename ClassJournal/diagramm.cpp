#include "diagramm.h"
#include "ui_diagramm.h"

Diagramm::Diagramm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Diagramm)
{
    ui->setupUi(this);
}

Diagramm::~Diagramm()
{
    delete ui;
}

void Diagramm::receiveChartView(QChartView *chartView)
{
    ui->horizontalLayout->addWidget(chartView);
}
