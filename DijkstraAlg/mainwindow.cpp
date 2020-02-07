#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Выставление начальных значений
    // Заполнение моделей таблиц
    pointsModel = new QStandardItemModel();

    pointsModel->appendRow(new QStandardItem(""));
    pointsModel->item(0, 0)->setEditable(false);

    this->setupPointsModel();

    this->setupLengthsModel(QStringList() << "От точки"
                                          << "До точки"
                                          << "Расстояние");

    // Подключение слота изменения модели к соответствующему слоту изменения item'а модели точек
    connect(pointsModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(modelChanged(QStandardItem*)));

    // Установка моделей для соответствующих таблиц
    // и ширины колонок таблиц по размеру контента (текста) в них
    ui->PointsTable->setModel(pointsModel);
    ui->PointsTable->resizeColumnsToContents();
    ui->LengthsTable->setModel(lengthsModel);
    ui->LengthsTable->resizeColumnsToContents();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Функция для инициализации таблицы расстояний
// headers - названия колонок таблицы
void MainWindow::setupLengthsModel(const QStringList &headers) {
    lengthsModel = new QStandardItemModel();
    lengthsModel->setHorizontalHeaderLabels(headers); // Установка заголовков колонок
}

// Функция для инициализации таблицы точек
void MainWindow::setupPointsModel()
{
    // Пока кол-во точек в таблице не равно кол-ву точек,
    // которое хочет пользователь - добваляем/удаляем строки и столбцы таблицы
    while (pointsModel->rowCount() != ui->PointsCountSpin->value()) {
        if (pointsModel->rowCount() < ui->PointsCountSpin->value()) {
            pointsModel->appendRow(new QStandardItem(""));
            for (int i = 0; i < pointsModel->columnCount(); i++) {
                pointsModel->setItem(pointsModel->rowCount()-1, i, new QStandardItem(""));
            }
            pointsModel->appendColumn(QList<QStandardItem*>());
            for (int i = 0; i < pointsModel->rowCount(); i++) {
                pointsModel->setItem(i, pointsModel->columnCount()-1, new QStandardItem(""));
                if (i == pointsModel->rowCount()-1) {
                    pointsModel->item(i, i)->setEditable(false);
                }
            }
        }
        else {
            pointsModel->removeRow(pointsModel->rowCount()-1);
            pointsModel->removeColumn(pointsModel->columnCount()-1);
        }
    }
}

// Слот изменения модели таблицы точек
void MainWindow::modelChanged(QStandardItem* item)
{
    // Если не ор. граф
    if (!ui->isOrientedCheck->isChecked()) {
        // И item не на диагонали
        if (item->row() != item->column()) {
            // Блокируем сигнал, меняем значение в зеркальной этой ячейке и разблокируем сигнал
            pointsModel->blockSignals(true);
            pointsModel->setItem(item->column(), item->row(), new QStandardItem(item->text()));
            pointsModel->blockSignals(false);
            // Снова выставляем ширину столбцов равной контенту (тексту) в них
            ui->PointsTable->resizeColumnsToContents();
        }
    }
}

// Слот клика на кнопку "Рассчитать расстояния"
void MainWindow::on_CalcButton_clicked()
{
    int size = ui->PointsCountSpin->value(); // размер матрицы (Кол-во точек)
    QVector<QVector<int>> items(size); // Матрица значений таблицы, которую нужно будет передать в функцию вычисления
    // Заполнение матрицы
    for (int i = 0; i < size; i++) {
        QVector<int> newVec(size);
        for (int j = 0; j < size; j++) {
            newVec[j] = pointsModel->item(i, j)->text().toInt();
        }
        items[i] = newVec;
    }

    // Очистка таблицы расстояний
    this->setupLengthsModel(QStringList() << "От точки"
                                          << "До точки"
                                          << "Расстояние");

    // Вычисление расстояний и заполнение таблицы
    Dijkstra(items, 0, size, ui->allPointsCheck->isChecked());
}

// Алгоритм Дейкстры
// Параметры: Исходная матрица связей, стратовая точка, размер матрицы, флаг "Все расстояния"
void MainWindow::Dijkstra(QVector<QVector<int>> Matrix, int start, int size, bool all)
{
    for (int j = 0; j < (all ? size : 1)*(size-1); j+=size-1) {
        // Массивы точек и их посещений
        QVector<int> distance(size, INT_MAX); // Вектор расстояний от стартовой точки до остальных
        int index = 0;
        QVector<bool> visited(size, false);
        distance[start]=0; // Точка старта - расстояниe = 0
        for (int count=0; count<size-1; count++) // Исключаем начальную точку!(?)
        {
            int min=INT_MAX; // Минимальное расстояниe
            for (int i=0; i<size; i++) // находим минимальное расстояние
                if (!visited[i] && distance[i]<=min) // Не посещалась и дистанция меньше - записываем // = поидее не нужно в <=
                {
                    min=distance[i];
                    index=i;
                }
            visited[index]=true;
            for (int i=0; i<size; i++) // Ищем минимум для соседа текущей точки
                // Если точка не посещалась, можно туда прийти отсюда, дистанция известна
                // и дистанция через другую точку меньше, чем текущая - записываем новую дистанцию
                if (!visited[i] && Matrix[index][i] && distance[index]!=INT_MAX && distance[index]+Matrix[index][i]<distance[i])
                    distance[i]=distance[index]+Matrix[index][i];
        }

        // Вывод в таблицу
        for (int i=0; i<size; i++) {
            if (i != start) {
                lengthsModel->setItem(i > start ? j+i-1 : j+i, 0, new QStandardItem(QString::number(start+1)));
                lengthsModel->setItem(i > start ? j+i-1 : j+i, 1, new QStandardItem(QString::number(i+1)));
                if (distance[i]!=INT_MAX && distance[i] != 0) {
                    lengthsModel->setItem(i > start ? j+i-1 : j+i, 2, new QStandardItem(QString::number(distance[i])));
                }
                else {
                    lengthsModel->setItem(i > start ? j+i-1 : j+i, 2, new QStandardItem("маршрут недоступен"));
                }
            }
        }

        // Если нужны все расстояния - инкрементируем начальную точку
        if (all) {
            start++;
        }
    }
    ui->LengthsTable->setModel(lengthsModel);
    ui->LengthsTable->resizeColumnsToContents();
}

// Слот изменения значения кол-ва точек
// Убраны параметры слотов - чтобы исчезли warning'и
void MainWindow::on_PointsCountSpin_valueChanged()
{
    setupPointsModel(); // Добавляем/удаляем строки и столбцы
    ui->PointsTable->resizeColumnsToContents();
}

// Слот переключения чекбокса ор. графа
void MainWindow::on_isOrientedCheck_toggled(bool checked)
{
    // Если не ор. граф
    if (!checked) {
        // Блокируем сигналы изменения модели таблицы точек
        // Чтобы не было лишних вызовов modelChanged
        pointsModel->blockSignals(true);
        // Копируем данные 1-й половины таблицы во вторую половину
        // и обратно, чтобы расстояния были зеркальными
        for (int i = 0; i < pointsModel->rowCount(); i++) {
            for (int j = 0; j < pointsModel->columnCount(); j++) {
                if (i != j && pointsModel->item(i, j)->text() != "") {
                    pointsModel->setItem(j, i, new QStandardItem(pointsModel->item(i, j)->text()));
                }
            }
        }
        // Разблокируем сигналы модели
        pointsModel->blockSignals(false);

        /// Единственный косяк - модель обновляется, а таблица не хочет.
        /// Таблица обновится только если на неё нажать. Ищу решение уже давно, но пока не нахожу.
    }
}

// Слот клика по кнопке сохранения в базу данных
void MainWindow::on_SaveButton_clicked()
{
    // Вызов файлового диалога для выбора нужного файла
    QFileDialog objFlDlg(this);
    objFlDlg.setOption(QFileDialog::ShowDirsOnly, true);
    objFlDlg.setAcceptMode(QFileDialog::AcceptSave);
    objFlDlg.setNameFilter("*.db");
    objFlDlg.selectFile("Data.db");
    if (objFlDlg.exec()) {
        qDebug() << objFlDlg.selectedFiles().at(0);

        db = new Database();
        /*  подключаемся к базе данных*/
        db->connectToDataBase(objFlDlg.selectedFiles().at(0));

        // Запрос на очистку таблицы точек
        QSqlQuery qry = QSqlQuery();
        qry.prepare("DELETE FROM Points");
        qry.exec();
        qDebug() << qry.lastError();
        qry.clear();

        // Запросы на вставку новых данных в таблицу точек
        int size = pointsModel->columnCount();
        if (ui->isOrientedCheck->isChecked()) { // Если это ор. граф - считываем все заполненные связи
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (pointsModel->item(i, j)->text() != "") {
                        qry.prepare("INSERT INTO Points VALUES (:Start, :End, :Length);");
                        qry.bindValue(":Start", i+1);
                        qry.bindValue(":End", j+1);
                        qry.bindValue(":Length", pointsModel->item(i, j)->text().toInt());
                        qry.exec();
                        qDebug() << qry.lastError();
                        qry.clear();
                    }
                }
            }
        }
        else { // Иначе - считываем половину, т. к. таблица зеркальна
            for (int i = 1; i < size; i++) {
                for (int j = i; j < size; j++) {
                    if (pointsModel->item(i, j)->text() != "") {
                        qry.prepare("INSERT INTO Points VALUES (:Start, :End, :Length);");
                        qry.bindValue(":Start", i+1);
                        qry.bindValue(":End", j+1);
                        qry.bindValue(":Length", pointsModel->item(i, j)->text().toInt());
                        qry.exec();
                        qDebug() << qry.lastError();
                        qry.clear();
                    }
                }
            }
        }

        // Запрос на очистку таблицы дистанций
        qry.prepare("DELETE FROM Lengths");
        qry.exec();
        qDebug() << qry.lastError();
        qry.clear();

        // Запросы на вставку новых данных в таблицу дистанций
        size = lengthsModel->columnCount();
        for (int i = 0; i < lengthsModel->rowCount(); i++) {
            qry.prepare("INSERT INTO Lengths VALUES (:Start, :End, :Length);");
            qry.bindValue(":Start", lengthsModel->item(i, 0)->text().toInt());
            qry.bindValue(":End", lengthsModel->item(i, 1)->text().toInt());
            qry.bindValue(":Length", lengthsModel->item(i, 2)->text().toInt());
            qry.exec();
            qDebug() << qry.lastError();
            qry.clear();
        }

        // Запрос на очистку таблицы настроек
        qry.prepare("DELETE FROM Settings");
        qry.exec();
        qDebug() << qry.lastError();
        qry.clear();

        // Запрос на вставку новых данных в таблицу настроек
        qry.prepare("INSERT INTO Settings VALUES (:IsOriented, :AllPoints, :PointsCount);");
        qry.bindValue(":IsOriented", ui->isOrientedCheck->isChecked());
        qry.bindValue(":AllPoints", ui->allPointsCheck->isChecked());
        qry.bindValue(":PointsCount", ui->PointsCountSpin->value());
        qry.exec();
        qDebug() << qry.lastError();
        qry.clear();

        db->disconnect();
    }
}

// Слот клика по кнопке загрузки из базы данных
void MainWindow::on_LoadButton_clicked()
{
    // Вызов файлового диалога для выбора нужного файла
    QFileDialog objFlDlg(this);
    objFlDlg.setOption(QFileDialog::ShowDirsOnly, true);
    objFlDlg.setAcceptMode(QFileDialog::AcceptOpen);
    objFlDlg.setNameFilter("*.db");
    objFlDlg.selectFile("Data.db");
    if (objFlDlg.exec()) {
        db = new Database();
        /*  подключаемся к базе данных*/
        db->connectToDataBase(objFlDlg.selectedFiles().at(0));

        // Очистка модели таблицы точек
        for (int i = 0; i < pointsModel->rowCount(); i++) {
            for (int j = 0; j < pointsModel->columnCount(); j++) {
                pointsModel->item(i, j)->setText("");
            }
        }

        QSqlQuery qry = QSqlQuery();
        // Считываение настроек из базы
        qry.exec("SELECT * FROM Settings");
        while (qry.next()) {
            ui->isOrientedCheck->setChecked(qry.value(0).toBool());
            ui->allPointsCheck->setChecked(qry.value(1).toBool());
            ui->PointsCountSpin->setValue(qry.value(2).toInt());
        }
        qry.clear();

        // Считываение точек из базы
        qry.prepare("SELECT * FROM Points");
        qry.exec();
        while (qry.next()) {
            pointsModel->item(qry.value(0).toInt()-1, qry.value(1).toInt()-1)->setText(qry.value(2).toString());
        }
        qry.clear();

        db->disconnect();
    }
}

// События наведения мыши на таблицы - показ подсказок в statusBar'е
// Убраны параметры слотов - чтобы исчезли warning'и
void MainWindow::on_PointsTable_entered()
{
    ui->statusBar->showMessage("В эту таблицу вы можете ввести данные точек");
}

void MainWindow::on_LengthsTable_entered()
{
    ui->statusBar->showMessage("В этой таблице будет отображён результат вычисления алгоритма Дейкстры");
}
