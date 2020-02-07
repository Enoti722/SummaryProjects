#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

signals:
    void sendData(int TeacherID, QString TeacherName, Database* db);

private slots:
    void on_ConnectBtn_clicked();

private:
    Ui::MainWindow *ui;

    Database* db;
};

#endif // MAINWINDOW_H
