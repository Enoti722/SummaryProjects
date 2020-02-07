#include "accountantscreen.h"
#include "ui_accountantscreen.h"

#include <QMessageBox>
#include <QFileDialog>

AccountantScreen::AccountantScreen(QWidget *parent) :
    WorkersWindowParent(parent),
    ui(new Ui::AccountantScreen)
{
    ui->setupUi(this);
    ui->ToDateDE->setDate(QDate::currentDate());
    page = 1;
}

AccountantScreen::~AccountantScreen()
{
    delete ui;
}

void AccountantScreen::receiveDataFunc(int WID, int PID, QString WName, Database* ndb)
{
    WorkersWindowParent::receiveDataFunc(WID, PID, WName, ndb);

    setWindowTitle("Окно бухгалтера. Бухгалтер: " + WorkerName);

    RefreshPage();
    RefreshTable();
}

void AccountantScreen::on_RefreshTableBtn_clicked()
{
    RefreshPage();
    RefreshTable();
}

void AccountantScreen::RefreshTable()
{
    model = new QStandardItemModel();
    if (db->FindChecks(ui->FromDateDE->date(), ui->ToDateDE->date(), 5, page, model)) {
        ui->ChecksTableView->setModel(model);
        QStringList strs;
        strs << "Квитанция №" << "ФИО фотографа" << "ФИО кассира" << "Дата выдачи" << "Стоимость" << "Статус";
        model->setHorizontalHeaderLabels(strs);
        ui->ChecksTableView->hideColumn(0);
        ui->ChecksTableView->hideColumn(5);
        qDebug() << "Таблица обновлена." << endl;
        ui->ChecksTableView->selectRow(0);
    }
    else {
        qDebug() << "Невозможно обновить таблицу!" << endl;
    }
}

void AccountantScreen::RefreshPage()
{
    ui->PrevPageBtn->setEnabled(true);
    ui->NextPageBtn->setEnabled(true);
    if (page <= 1) {
        page = 1;
        ui->PrevPageBtn->setEnabled(false);
    }
    int pagesCount;
    db->GetPagesCount(ui->FromDateDE->date(), ui->ToDateDE->date(), 5, pagesCount);
    if (page >= pagesCount) {
        page = pagesCount;
        ui->NextPageBtn->setEnabled(false);
    }
    RefreshTable();
    ui->PageNumberLabel->setText(QString::number(page) + " из " + QString::number(pagesCount));
}

void AccountantScreen::on_PrevPageBtn_clicked()
{
    page--;
    RefreshPage();
    RefreshTable();
}

void AccountantScreen::on_NextPageBtn_clicked()
{
    page++;
    RefreshPage();
    RefreshTable();
}

bool AccountantScreen::createHtmlTableFromModel() {

    // make a html-dump of table view
    if (ui->ChecksTableView) {
        QString filePath = QFileDialog::getSaveFileName(this, "Выберите файл для сохранения", QDir::currentPath(), "HTML-файлы(*.html)");

        if (filePath == "") {
            return false;
        }
        QFile file(filePath);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", QString("Can`t create file " + filePath));
            return false;
        }

        QTextStream out(&file);

        const long rowCount = model->rowCount();
        const long columnCount = model->columnCount();

        out << "<html>\n"
               "<head>\n"
               "<meta Content=\"Text/html; charset=Windows-1251\">\n"
            << QString("<title>Отчёт</title>\n") // Заголовок
            << "<style>\n"
               "table {\n"
                   "border-bottom-width:0px;\n"
                   "border-left-width: 0px;\n"
                   "frame: void;\n"
               "}\n"
               "th {\n"
                   "border-left-width: 0px;\n"
                   "border-bottom-width:0px;\n"
               "}\n"
               ".br {\n"
                   "border-left-width:2px;\n"
               "}\n"
               ".brd {\n"
                   "border-bottom-width:2px;\n"
               "}\n"
               "</style>"
               "</head>\n"
               "<body leftmargin=40 rightmargin=40 marginwidth=40>\n"
               "<table align=center border=1  bordercolor=black cellspacing=0 cellpadding=5 width=100%>\n";

        // headers
        out << "<tr bgcolor=#f0f0f0 align=center>";
        for (long column = 0; column < columnCount; column++)
            if (column != 5) {
                out << QString("<td " + (column == 0 ? QString("class=br width=7%") : "") + ">%1</td>").arg(model->headerData(column, Qt::Horizontal).toString());
            }
        out << "</tr>\n";
        file.flush();

        QSqlQuery* qry = new QSqlQuery;
        db->FindChecksForReport(ui->FromDateDE->date(), ui->ToDateDE->date(), 5, page, qry);

        // data table
        double sum = 0;

        while (qry->next()) {
            QStringList lst = db->QueryDataToString(qry);
            out << "<tr align=right>";
            for (long column = 0; column < lst.count(); column++) {
                out << QString("<td " + (column == 0 ? QString("class=br width=7%") : "") + ">%1</td>").arg(lst.at(column));
            }
            out << "</tr>\n";

            sum += lst.at(4).toDouble();
        }
        out << "<tr align=RIGHT>\n"
                   "<th colspan=4></th>\n"
                   "<td class=brd>" + QString::number(sum) + "</td >\n"
               "</tr>\n";
        out <<  "</table>\n"
            "</body>\n"
            "</html>\n";

        file.close();
    }

    return true;
}

void AccountantScreen::on_HTML_Action_triggered()
{
    createHtmlTableFromModel();
}
