#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QStandardItem>
#include <QStandardItemModel>
#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setupPointsModel(); // Функция для инициализации модели таблицы точек
    void setupLengthsModel(const QStringList &headers); // Функция для инициализации модели таблицы дистанций
    void Dijkstra(QVector<QVector<int>> GR, int st, int size, bool all); // Алгоритм Дейкстры

private slots:
    void modelChanged(QStandardItem *item);

    void on_CalcButton_clicked();

    void on_PointsCountSpin_valueChanged();

    void on_isOrientedCheck_toggled(bool checked);

    void on_SaveButton_clicked();

    void on_LoadButton_clicked();

    void on_PointsTable_entered();

    void on_LengthsTable_entered();

private:
    Ui::MainWindow *ui;

    // Модели таблиц
    QStandardItemModel* pointsModel;
    QStandardItemModel* lengthsModel;

    Database *db; // Объект нашего класса Database для работы с БД
};

#endif
