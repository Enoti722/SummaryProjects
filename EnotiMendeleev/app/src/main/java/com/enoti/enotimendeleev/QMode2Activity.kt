package com.enoti.enotimendeleev

import android.content.Intent
import android.database.Cursor
import android.database.sqlite.SQLiteDatabase
import android.graphics.Color
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.CountDownTimer
import android.widget.TextView
import kotlinx.android.synthetic.main.activity_qmode2.*

class QMode2Activity : AppCompatActivity() {

    // Константы
    // Информация о таблице QMode2 в базе данных
    private val TABLE_QMODE2 = "QMode2"

    var rightAnsw = ""
    var taskList = IntArray(10) {-1}
    var taskCount = 0
    var rightAnswCount = 0

    var db : SQLiteDatabase? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_qmode2)

        // Открываем базу данных
        db = SQLiteDatabase.openDatabase(getDatabasePath("Data").path, null, SQLiteDatabase.OPEN_READONLY)

        getTask()
    }

    fun addListenerOnButton(button: TextView) {
        button.setOnClickListener {
            // отключение кнопок
            for(i in 0 until ScrollLinear.childCount) {
                ScrollLinear.getChildAt(i).isClickable = false
            }
            // Показываем цветом верный ли ответ
            showRightAnsw()
            // Таймер, по окончании которого даётся новое задание, либо показываются результаты
            object: CountDownTimer(1000, 1) {
                override fun onTick(p0: Long) { }
                override fun onFinish () {
                    if (taskCount == 10) {
                        // Переход на окно результатов
                        var resultScreenIntent = Intent(this@QMode2Activity, QuizResultActivity::class.java)
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

            if (rightAnsw == button.text) { // Если ответ верный - прибавляем 1 к счётчику
                rightAnswCount++
            }
        }
    }

    fun showRightAnsw() { // Отображение цветом верного ответа
        for(i in 0 until ScrollLinear.childCount) {
            if ((ScrollLinear.getChildAt(i) as TextView).text == rightAnsw) {
                (ScrollLinear.getChildAt(i) as TextView).setTextColor(Color.parseColor("#ffff00"))
            }
            else {
                (ScrollLinear.getChildAt(i) as TextView).setTextColor(Color.parseColor("#ff0000"))
            }
        }
    }

    fun getTask() { // Считывание задания из базы данных и создание кнопок
        ScrollLinear.removeAllViews()

        // Подсказка должна стать невидимой, когда будет

        var cursor : Cursor? = db!!.query(TABLE_QMODE2, null, null, null, null, null, null)

        // Выбор задания

        cursor!!.moveToLast()
        val lastPos : Int = cursor.position

        var needRand = true
        while (needRand) {
            var taskPos = (0..lastPos).random()
            cursor.moveToPosition(taskPos).toString()
            if (taskPos !in taskList) { // По сути можно ведь заменить на сам taskPos !in taskList
                val answers = cursor.getString(1).split("|").toTypedArray()
                // Перемешивание массива строк для кнопок
                shuffleArray(answers)
                // Создание кнопок и применение к ним стилей
                for(i in answers) {
                    var textButton = TextView(this)
                    textButton.text = i
                    textButton.setTextColor(Color.parseColor("#00ff00"))
                    textButton.setShadowLayer(15f, 0f, 0f, Color.parseColor("#ff000000"))
                    textButton.textSize = 30f
                    @Suppress("DEPRECATION")
                    textButton.background = resources.getDrawable(R.drawable.tables)

                    addListenerOnButton(textButton)

                    ScrollLinear.addView(textButton)
                }
                // Получение строки верного ответа
                rightAnsw = cursor.getString(2)
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
            val index = (0 until ar.size).random() // ??? Было 11
            val a = ar[index]
            ar[index] = ar[i]
            ar[i] = a
        }
    }
}
