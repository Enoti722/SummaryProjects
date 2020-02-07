package com.enoti.enotimendeleev

import android.content.Intent
import android.database.Cursor
import android.database.sqlite.SQLiteDatabase
import android.graphics.Color
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.CountDownTimer
import android.widget.TextView
import kotlinx.android.synthetic.main.activity_qmode3.*
import java.util.*

class QMode3Activity : AppCompatActivity() {

    // Константы
    // Информация о таблице QMode3 в базе данных
    private val TABLE_QMODE3 = "QMode3"

    // Переменные
    var taskList = IntArray(10) {-1}
    var taskCount = 0
    var rightAnswCount = 0
    var answersList = Vector<String?>()

    var db : SQLiteDatabase? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_qmode3)

        // Открытие базы
        db = SQLiteDatabase.openDatabase(getDatabasePath("Data").path, null, SQLiteDatabase.OPEN_READONLY)

        getTask()
    }

    fun addListenerOnButton(button: TextView) { // Назначение функции, которая будет вызываться на созданной кнопке
        button.setOnClickListener {
            if (button.isSelected) {
                // Удаление цифры из ответа
                T_UserAnswer.text = T_UserAnswer.text.toString().removeRange(T_UserAnswer.text.toString().indexOf(button.text[0]), T_UserAnswer.text.toString().indexOf(button.text[0])+1)
                button.isSelected = false // Посмотреть как делали до этого
            }
            else {
                // Добавление цифры к ответу
                T_UserAnswer.text = "${T_UserAnswer.text}${button.text.toString()[0]}"
                button.isSelected = true // Посмотреть как делали до этого
            }

            // Если поле ответов заполнено
            if (T_UserAnswer.text.toString().length == answersList.count()) {
                // отключение кнопок
                for(i in 0 until ScrollLinear.childCount) {
                    ScrollLinear.getChildAt(i).isClickable = false // Блокирование кнопок (если делать как тест - 10 вопросов)
                }
                // Проверка правильности ответа
                var rightAnsw = true
                for (i in 0 until answersList.count()) {
                    if (answersList[i] != (ScrollLinear.getChildAt(i) as TextView).text.toString().removeRange(0..2)) { // Для облегчения сравнений можно предварительно сделать строку rightAnsw с правильной последовательностью
                        rightAnsw = false
                        break
                    }
                }
                // Показываем цветом верный ли ответ
                if (rightAnsw) {
                    T_UserAnswer.setTextColor(Color.parseColor("#ffff00"))
                    rightAnswCount++
                }
                else {
                    T_UserAnswer.setTextColor(Color.parseColor("#ff0000"))
                }
                // Таймер, по окончании которого даётся новое задание, либо показываются результаты
                object: CountDownTimer(1000, 1) {
                    override fun onTick(p0: Long) { }
                    override fun onFinish () {
                        if (taskCount == 6) { // Пока у нас 1 вопрос в базе - ставим 1 // Всего в базе 6 штук
                            // Переход на окно результатов
                            var resultScreenIntent = Intent(this@QMode3Activity, QuizResultActivity::class.java)
                            resultScreenIntent.putExtra("rightAnswersCount", rightAnswCount)
                            resultScreenIntent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP
                            startActivity(resultScreenIntent)
                            finish()
                        }
                        else {
                            // Поиск нового вопроса
                            getTask()
                        }
                    }
                }.start()
            }
        }
    }

    fun getTask() { // Считывание задания из базы данных и создание кнопок
        ScrollLinear.removeAllViews()

        T_UserAnswer.text = ""
        T_UserAnswer.setTextColor(Color.parseColor("#ffffff"))
        answersList.clear()

        // TODO: Подсказка должна стать невидимой, когда будет

        // Запрос в базу данных
        var cursor : Cursor? = db!!.query(TABLE_QMODE3, null, null, null, null, null, null)

        // Получение кол-ва вопросов
        cursor!!.moveToLast()
        val lastPos : Int = cursor.position

        // Рандом задания (которого ещё не было)
        var needRand = true
        while (needRand) {
            var taskPos = (0..lastPos).random()
            cursor.moveToPosition(taskPos).toString()
            if (taskPos !in taskList) {
                // Получение ответов
                val answers = cursor.getString(0).split("|").toTypedArray()
                // Запись правильного порядка в массив
                for (i in 0 until answers.count()) {
                    answersList.add(answers[i])
                }
                // Перемешивание массива строк для кнопок
                shuffleArray(answers)
                // Создание кнопок и применение к ним стилей
                for(i in 0 until answers.count()) {
                    var textButton = TextView(this)
                    textButton.text = "${i+1}. ${answers[i]}"
                    //textButton.setTextColor(Color.parseColor("#00ff00"))
                    textButton.setShadowLayer(15f, 0f, 0f, Color.parseColor("#ff000000"))
                    textButton.textSize = 30f
                    @Suppress("DEPRECATION")
                    textButton.background = resources.getDrawable(R.drawable.tables)

                    addListenerOnButton(textButton)

                    ScrollLinear.addView(textButton)
                }
                // Запись номеров вопросов, которые уже использованы
                taskList[taskCount] = taskPos
                taskCount++
                needRand = false
            }
        }
        cursor.close()
    }

    fun shuffleArray(ar: Array<String>) { // Функция перемешивания массива
        for (i in ar.size - 1 downTo 1) {
            val index = (0 until ar.size).random()
            val a = ar[index]
            ar[index] = ar[i]
            ar[i] = a
        }
    }
}
