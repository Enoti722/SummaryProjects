package com.enoti.enotimendeleev

import android.content.Intent
import android.database.Cursor
import android.database.sqlite.SQLiteDatabase
import android.graphics.Color
import android.graphics.Point
import android.graphics.Typeface
import android.os.Build
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.MotionEvent
import android.view.View
import android.view.animation.AnimationUtils
import android.widget.Button
import android.widget.TextView
import kotlinx.android.synthetic.main.activity_game.*
import java.util.*


class GameActivity : AppCompatActivity() {

    // Константы
    // Информация о таблице PeriodicTable в базе данных
    private val TABLE_PERIODIC = "PeriodicTable"
    private val KEY_ID_P = "ID"
    private val KEY_SHORTNAME = "ShortName"
    private val KEY_FULLNAME = "FullName"
    private val KEY_RUNAME = "RuName"

    // Информация о таблице Tasks в базе данных
    private val TABLE_TASKS = "Tasks"
    private val KEY_ID = "_id"
    private val KEY_TYPE = "Type"
    private val KEY_PICTURE = "Picture"
    private val KEY_HINT = "Hint"
    private val KEY_ANSWERS = "Answers"
    private val KEY_FULLANSW = "FullAnsw"
    private val KEY_ANSWRU = "AnswRu"

    // Кол-во вопросов разных режимов
    private val questCountEasy = 91
    private val questCountGeography = 17
    private val questCountHard = 24

    // Флаг - все вопросы закончились
    var allTasksEnded = false

    // База данных
    var db : SQLiteDatabase? = null

    // Массив Boolean всех заданий. true - задание было использовано
    var allTasks : BooleanArray = BooleanArray(0)

    // Текущие элементы и их номера
    var curElements = Vector<String?> (0)
    var curElementNums = Vector<Int> (0)

    // Информация о задании (Файл картинки, подсказка, ответы, полный ответ, ответ на русском)
    var Picture = ""
    var Hint = ""
    var Answers : List<String?> = listOf()
    var FullAnsw = ""
    var AnswRu = ""

    // Номер режима
    var Mode = 0

    // Главный вариант ответа. На случай, если можно ответить разными способами
    var mainAnswer : String? = ""

    // Флаг ответил ли пользователь на вопрос
    var answered = false

    // Текущее задание
    var curTaskPos = -1

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_game)
        // Получение выбранного ранее режима
        @Suppress("RECEIVER_NULLABILITY_MISMATCH_BASED_ON_JAVA_ANNOTATIONS")
        Mode = intent.extras.getInt("mode")

        var SP = getSharedPreferences("com.enoti.enotimendeleev", MODE_PRIVATE)

        // Получаем массив правильных ответов в зависимости от режима
        when(Mode) {
            1 -> {
                if (!SP.getBoolean("easyModeCompleted", false)) {
                    allTasks = BooleanArray(questCountEasy)
                    for (i in (0..questCountEasy)) {
                        if (SP.getBoolean("easyQuest_$i", false)) {
                            allTasks[i] = true
                        }
                    }
                }
            }
            2 -> {
                if (!SP.getBoolean("geographyModeCompleted", false)) {
                    allTasks = BooleanArray(questCountGeography)
                    for (i in (0..questCountGeography)) {
                        if (SP.getBoolean("geographyQuest_$i", false)) {
                            allTasks[i] = true
                        }
                    }
                }
            }
            3 -> {
                if (!SP.getBoolean("hardModeCompleted", false)) {
                    allTasks = BooleanArray(questCountHard)
                    for (i in (0..questCountHard)) {
                        if (SP.getBoolean("hardQuest_$i", false)) {
                            allTasks[i] = true
                        }
                    }
                }
            }
        }

        // Открытие базы
        db = SQLiteDatabase.openDatabase(getDatabasePath("Data").path, null, SQLiteDatabase.OPEN_READONLY)

        // Получение размеров экрана
        var size = Point(0, 0)
        windowManager.defaultDisplay.getSize(size)
        // Выставление ширины экрана для полного сдвижения после ответа
        QuestResConstraint.translationX = size.x.toFloat()

        getTask()

        addListenerOnButton()
    }

    override fun onBackPressed() { } // Отключение возвращения на предыдущий экран

    fun addListenerOnButton() { // Назначение функции, которая будет вызываться на созданной кнопке
        Var1.setOnClickListener {
            ElementButtonClicked(1, Var1)
        }
        Var2.setOnClickListener {
            ElementButtonClicked(2, Var2)
        }
        Var3.setOnClickListener {
            ElementButtonClicked(3, Var3)
        }
        Var4.setOnClickListener {
            ElementButtonClicked(4, Var4)
        }
        Var5.setOnClickListener {
            ElementButtonClicked(5, Var5)
        }
        Var6.setOnClickListener {
            ElementButtonClicked(6, Var6)
        }
        Var7.setOnClickListener {
            ElementButtonClicked(7, Var7)
        }
        Var8.setOnClickListener {
            ElementButtonClicked(8, Var8)
        }
        Var9.setOnClickListener {
            ElementButtonClicked(9, Var9)
        }
        Var10.setOnClickListener {
            ElementButtonClicked(10, Var10)
        }
        Var11.setOnClickListener {
            ElementButtonClicked(11, Var11)
        }
        Var12.setOnClickListener {
            ElementButtonClicked(12 , Var12)
        }
        HintBut.setOnClickListener {
            // Показать рекламу :)
            T_Hint.visibility = View.VISIBLE
        }
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean { // Событие нажатия на экран
        // Если верный ответ получен
        if (!answered) return false
        // Обнуляем переменную
        answered = false
        // Если вопросы не закончились - ищем новый вопрос и возвращаем все поля к стандартному виду
        if (!allTasksEnded) {
            T_Answ.text = ""
            curElements.clear()
            curElementNums.clear()

            getTask()

            Var1.isSelected = false
            Var2.isSelected = false
            Var3.isSelected = false
            Var4.isSelected = false
            Var5.isSelected = false
            Var6.isSelected = false
            Var7.isSelected = false
            Var8.isSelected = false
            Var9.isSelected = false
            Var10.isSelected = false
            Var11.isSelected = false
            Var12.isSelected = false

            // Обратная анимация (Возвращение к странице вопроса)
            var anim = AnimationUtils.loadAnimation(applicationContext, R.anim.toright)
            QuestConstraint.startAnimation(anim)
            QuestResConstraint.startAnimation(anim)
        }
        else { // Если вопросы закончились - показываем окно результатов
            var resultIntent = Intent(this, ResultActivity::class.java)
            resultIntent.putExtra("mode", Mode)
            resultIntent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP
            startActivity(resultIntent)
            finish()
        }

        return super.onTouchEvent(event)
    }

    fun getTask () { // Считывание задания из базы данных и создание кнопок

        T_Hint.visibility = View.INVISIBLE

        var selection = "${KEY_TYPE} = $Mode"

        // Запрос в базу данных
        var cursor : Cursor? = db!!.query(TABLE_TASKS, null, selection, null, null, null, null)

        // Получение кол-ва вопросов
        cursor!!.moveToLast()
        val lastPos : Int = cursor.position

        // Выбор задания
        var needRand = true
        var taskPos = 0
        while (needRand) {
            taskPos = (0..lastPos).random()
            needRand = allTasks[taskPos]
        }
        curTaskPos = taskPos

        cursor.moveToPosition(taskPos)

        // Получение задания
        Picture = cursor.getString(1)
        Hint = cursor.getString(2)
        Answers = cursor.getString(3).split('|') // List
        FullAnsw = cursor.getString(4)
        AnswRu = cursor.getString(5)

        T_Hint.text = Hint

        cursor.close()

        // Массив обязательных для ответа элементов
        mainAnswer = Answers[(1..Answers.count()).random()-1]
        var neededElements = IntArray(12) // Кол-во кнопок = 12
        var index = 0
        for (j in mainAnswer!!.split(" ")) {
            neededElements[index] = j.toInt()
            index++
        }

        // Рандом оставшихся элементов
        while (index < 12) {
            neededElements[index] = (1..118).random()
            index++
        }

        // Рандомное заполнение кнопок элементами
        // Перемешивание массива ответов
        shuffleArray(neededElements)

        // Получение названий элементов. Хранятся в базе с таблицей Менделеева
        var elements = arrayOfNulls<String?>(12)
        index = 0
        for (i in neededElements) {
            selection = "${KEY_ID_P} = ${i}"
            cursor = db!!.query(TABLE_PERIODIC, null, selection, null, null, null, null)
            cursor.moveToFirst()
            elements[index] = cursor.getString(cursor.getColumnIndex(KEY_SHORTNAME))
            index++
        }

        cursor!!.close()

        // Заполнение кнопок названиями элементов
        Var1.text = elements[0]
        Var2.text = elements[1]
        Var3.text = elements[2]
        Var4.text = elements[3]
        Var5.text = elements[4]
        Var6.text = elements[5]
        Var7.text = elements[6]
        Var8.text = elements[7]
        Var9.text = elements[8]
        Var10.text = elements[9]
        Var11.text = elements[10]
        Var12.text = elements[11]

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            I_Img.setImageDrawable(getDrawable(resources.getIdentifier(Picture, "drawable", packageName)))
        }
        else {
            @Suppress("DEPRECATION")
            I_Img.setImageDrawable(resources.getDrawable(resources.getIdentifier(Picture, "drawable", packageName)))
        }

        // Обновляем строку ответа для получения чёрточек (обозначение кол-ва букв в слове)
        UpdateString()
    }

    fun shuffleArray(ar: IntArray) {
        for (i in ar.size - 1 downTo 1) {
            val index = (0..11).random()
            // Simple swap
            val a = ar[index]
            ar[index] = ar[i]
            ar[i] = a
        }
    }

    fun ElementButtonClicked (element : Int, button : Button) { // Функция для всех кнопок. element - номер элемента таблицы
        // Добавление/Удаление из строки ответа
        if (!button.isSelected) {
            curElements.addElement("${button.text}")
            curElementNums.addElement(element)
            button.isSelected = true
            button.refreshDrawableState()
        }
        else {
            var index = curElementNums.indexOf(element)
            curElements.removeAt(index)
            curElementNums.removeAt(index)
            button.isSelected = false
            button.refreshDrawableState()
        }

        // Обновление строки ответа
        UpdateString()

        // Проверка на правильность ответа
        if (T_Answ.text.toString().replace("-", "").replace("_", "").toLowerCase() == FullAnsw.toLowerCase()) {
            // Заполнение страницы результата текущего задания данными об элементах и полном ответе
            T_ResAnsw.text = "$FullAnsw - $AnswRu"
            var Answer = T_Answ.text.toString().replace("_", "").split("-")
            LinearElems.removeAllViews()
            for (i in Answer) {
                val selection = "${KEY_SHORTNAME} = '${i}'"
                var cursor : Cursor? = db!!.query(TABLE_PERIODIC, null, selection, null, null, null, null)
                var newText = TextView(this)
                cursor!!.moveToFirst()
                newText.typeface = Typeface.SANS_SERIF
                newText.setBackgroundResource(R.drawable.tables)
                newText.setTextColor(Color.parseColor("#aaaa00"))
                newText.textSize = 22F
                newText.setShadowLayer(10.toFloat(), 0.toFloat(), 0.toFloat(), Color.BLACK)
                newText.text = "${cursor.getString(cursor.getColumnIndex(KEY_ID_P))} - ${i} - ${cursor.getString(cursor.getColumnIndex(KEY_FULLNAME))} - ${cursor.getString(cursor.getColumnIndex(KEY_RUNAME))}"
                LinearElems.addView(newText)
                cursor.close()
            }

            // Анимация перехода на страницу результата
            var anim = AnimationUtils.loadAnimation(applicationContext, R.anim.toleft)
            QuestConstraint.startAnimation(anim)
            QuestResConstraint.startAnimation(anim)
            answered = true
            allTasks[curTaskPos] = true

            // Проверка на полное прохождение режима, если он не был пройден ранее
            var SP = getSharedPreferences("com.enoti.enotimendeleev", MODE_PRIVATE)
            var tasksEnded = true
            var count = 0
            var mode = ""
            when(Mode) {
                1 -> {
                    count = questCountEasy-1
                    mode = "easy"
                }
                2 -> {
                    count = questCountGeography-1
                    mode = "geography"
                }
                3 -> {
                    count = questCountHard-1
                    mode = "hard"
                }
            }
            for (i in (0..count)) {
                tasksEnded = allTasks[i]
                if (!tasksEnded) {
                    break
                }
            }

            // Запись информации о том, что этот вопрос задавался
            SP.edit().putBoolean("${mode}Quest_$curTaskPos", true).apply() // TODO: Запись не работает!!!

            // Если режим был пройден - ставим флаг, что все вопросы закончились
            if (tasksEnded) {
                allTasksEnded = true
            }
        }
    }

    fun UpdateString () { // Обновление строки ответа для показа того, сколько осталось дописать элементов.
        var str = ""
        if (curElements.count() > 0) {
            var curLetters = 0
            for (i in curElements) {
                str += "-$i"
                curLetters++
            }
            while (curLetters < mainAnswer!!.split(" ").count()) {
                str += "-_"
                curLetters++
            }
        }
        else {
            for(i in (1..mainAnswer!!.split(" ").count())) {
                str += "-_"
            }
        }
        str = str.removeRange(0..0)
        T_Answ.text = str
    }
}