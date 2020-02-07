#ifndef ADDSTUDENTDIALOG_H
#define ADDSTUDENTDIALOG_H

#include <QMainWindow>

namespace Ui {
class AddStudentDialog;
}

class AddStudentDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddStudentDialog(QWidget *parent = nullptr);
    ~AddStudentDialog();

signals:
    void sendNewStudent(QString newStudentName);

private slots:
    void on_AddBtn_clicked();

private:
    Ui::AddStudentDialog *ui;
};

#endif // ADDSTUDENTDIALOG_H
