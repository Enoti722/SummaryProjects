package com.enoti.enotimendeleev

import android.content.Intent
import android.database.Cursor
import android.database.sqlite.SQLiteDatabase
import android.graphics.Color
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.CountDownTimer
import kotlinx.android.synthetic.main.activity_qmode1.*

class QMode1Activity : AppCompatActivity() {

    // Константы
    // Информация о таблице QMode1 в базе данных
    private val TABLE_QMODE1 = "QMode1"

    var rightAnsw = ""
    var taskList = IntArray(10) {-1}
    var taskCount = 0
    var rightAnswCount = 0

    var db : SQLiteDatabase? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_qmode1)
        addListenerOnButton()

        // Открываем базу данных
        db = SQLiteDatabase.openDatabase(getDatabasePath("Data").path, null, SQLiteDatabase.OPEN_READONLY)

        getTask()
    }

    override fun onBackPressed() { }

    fun getTask() {// Считывание задания из базы данных и заполнение кнопок
        // Включаем кнопки
        T_QuizAnsw1.isClickable = true
        T_QuizAnsw2.isClickable = true
        T_QuizAnsw3.isClickable = true

        // Назначаем кнопкам текст
        T_QuizAnsw1.setTextColor(Color.parseColor("#00ff00"))
        T_QuizAnsw2.setTextColor(Color.parseColor("#00ff00"))
        T_QuizAnsw3.setTextColor(Color.parseColor("#00ff00"))

        // TODO: Подсказка должна стать невидимой, когда будет

        // Делаем запрос в базу данных
        var cursor : Cursor? = db!!.query(TABLE_QMODE1, null, null, null, null, null, null)

        // Узнаём сколько вопросов есть
        cursor!!.moveToLast()
        val lastPos : Int = cursor.position

        // Выбор задания
        var needRand = true
        while (needRand) {
            var taskPos = (0..lastPos).random()
            cursor.moveToPosition(taskPos)
            if (taskPos !in taskList) {
                // Получение задания
                T_QuizTask.text = cursor.getString(1)
                // Получение ответов
                val answList = cursor.getString(2).split("|").toTypedArray()
                shuffleArray(answList) // Перемешивание массива
                // Заполнение кнопок текстами ответов
                T_QuizAnsw1.text = answList[0]
                T_QuizAnsw2.text = answList[1]
                T_QuizAnsw3.text = answList[2]
                // Получение строки верного ответа
                rightAnsw = cursor.getString(3)
                // Запись номеров вопросов, которые уже использованы
                taskList[taskCount] = taskPos
                taskCount++
                needRand = false
            }
        }
    }

    fun addListenerOnButton() { // Назначение функций, которые будут вызываться на кнопках
        T_QuizAnsw1.setOnClickListener {
            onAnswButtonClick()
            if (rightAnsw == T_QuizAnsw1.text.toString()) { // Если ответ верный - прибавляем 1 к счётчику
                rightAnswCount++
            }
        }
        T_QuizAnsw2.setOnClickListener {
            onAnswButtonClick()
            if (rightAnsw == T_QuizAnsw2.text.toString()) { // Если ответ верный - прибавляем 1 к счётчику
                rightAnswCount++
            }
        }
        T_QuizAnsw3.setOnClickListener {
            onAnswButtonClick()
            if (rightAnsw == T_QuizAnsw3.text.toString()) { // Если ответ верный - прибавляем 1 к счётчику
                rightAnswCount++
            }
        }
    }

    fun onAnswButtonClick() { // Общая часть для всех кнопок
        // Отключаем кнопки, чтобы нельзя было нажать их дважды
        T_QuizAnsw1.isClickable = false
        T_QuizAnsw2.isClickable = false
        T_QuizAnsw3.isClickable = false
        // Показываем цветом верный ли ответ
        showRightAnsw()
        // Таймер, по окончании которого даётся новое задание, либо показываются результаты
        object: CountDownTimer(1000, 1) {
            override fun onTick(p0: Long) { }
            override fun onFinish () {
                if (taskCount == 10) {
                    // Переход на окно результатов
                    var ResultScreenIntent = Intent(this@QMode1Activity, QuizResultActivity::class.java)
                    ResultScreenIntent.putExtra("rightAnswersCount", rightAnswCount)
                    ResultScreenIntent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP
                    startActivity(ResultScreenIntent)
                    finish()
                }
                else {
                    // Поиск нового вопроса
                    getTask()
                }
            }
        }.start()
    }

    fun showRightAnsw() { // Отображение цветом верного ответа
        if(rightAnsw == T_QuizAnsw1.text.toString()) {
            T_QuizAnsw1.setTextColor(Color.parseColor("#ffff00"))
            T_QuizAnsw2.setTextColor(Color.parseColor("#ff0000"))
            T_QuizAnsw3.setTextColor(Color.parseColor("#ff0000"))
        }
        else if (rightAnsw == T_QuizAnsw2.text.toString()) {
            T_QuizAnsw1.setTextColor(Color.parseColor("#ff0000"))
            T_QuizAnsw2.setTextColor(Color.parseColor("#ffff00"))
            T_QuizAnsw3.setTextColor(Color.parseColor("#ff0000"))
        }
        else {
            T_QuizAnsw1.setTextColor(Color.parseColor("#ff0000"))
            T_QuizAnsw2.setTextColor(Color.parseColor("#ff0000"))
            T_QuizAnsw3.setTextColor(Color.parseColor("#ffff00"))
        }

    }

    fun shuffleArray(ar: Array<String>) { // Функция перемешивания массива
        for (i in ar.size - 1 downTo 1) {
            val index = (0 until ar.size).random() // ??? Было 11
            val a = ar[index]
            ar[index] = ar[i]
            ar[i] = a
        }
    }
}
