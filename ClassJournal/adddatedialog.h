#ifndef ADDDATEDIALOG_H
#define ADDDATEDIALOG_H

#include <QMainWindow>
#include "database.h"

namespace Ui {
class AddDateDialog;
}

class AddDateDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddDateDialog(QWidget *parent = nullptr);
    ~AddDateDialog();

signals:
    void sendNewDate(QDate newDate);

private slots:
    void on_AddBtn_clicked();

private:
    Ui::AddDateDialog *ui;
};

#endif // ADDDATEDIALOG_H
