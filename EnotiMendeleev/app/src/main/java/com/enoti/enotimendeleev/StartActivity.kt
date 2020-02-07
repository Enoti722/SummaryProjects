package com.enoti.enotimendeleev

import android.content.Intent
import android.content.pm.ActivityInfo
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_start.*
import java.io.FileOutputStream

class StartActivity : AppCompatActivity() {

    // Константы кол-ва вопросов в каждом режиме
    private val questCountEasy = 91
    private val questCountGeography = 17
    private val questCountHard = 24

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_start)
        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT // Только портретная ориентация
        addListenerOnButton()
        var SP = getSharedPreferences("com.enoti.enotimendeleev", MODE_PRIVATE)
        // При первом запуске приложения выставляем переменные в SP
        if (SP.getBoolean("firstr", true)) {
            copyDataBase("Data")// Скопировать базу, если это первый запуск
            SP.edit().putBoolean("firstr", false).apply()
            SP.edit().putBoolean("easyModeCompleted", false).apply()
            SP.edit().putBoolean("geographyModeCompleted", false).apply()
            SP.edit().putBoolean("hardModeCompleted", false).apply()

            SP.edit().putBoolean("geographyModeUnlocked", true).apply() // Дебаг - true
            SP.edit().putBoolean("hardModeUnlocked", true).apply()

            for (i in (0..questCountEasy)) {
                SP.edit().putBoolean("easyQuest_$i", false).apply()
            }

            for (i in (0..questCountGeography)) {
                SP.edit().putBoolean("geographyQuest_$i", false).apply()
            }

            for (i in (0..questCountHard)) {
                SP.edit().putBoolean("hardQuest_$i", false).apply()
            }
        }
    }

    private fun copyDataBase(dbName: String) { // Простое копирование базы данных из папки assets во внетреннее хранилище баз данных приложений. Выполняем при первом запуске
        val myInput = applicationContext.assets.open("$dbName.db")
        val outFileName = getDatabasePath(dbName)
        val myOutput = FileOutputStream(outFileName)
        val buffer = ByteArray(1024)
        var length: Int
        length = myInput.read(buffer)
        while (length > 0) {
            myOutput.write(buffer, 0, length)
            length = myInput.read(buffer)
        }
        myOutput.flush()
        myOutput.close()
        myInput.close()
    }

    override fun onResume() {
        super.onResume()
        T_PlayBut.isClickable = true
        enotiBut.isClickable = true
    }

    fun addListenerOnButton() { // Назначение функции кнопки начала игры
        T_PlayBut.setOnClickListener {
            T_PlayBut.isClickable = false
            enotiBut.isClickable = false
            var selectModeIntent = Intent(this, GameModeSelectionActivity::class.java)
            startActivity(selectModeIntent)
        }
        enotiBut.setOnClickListener {
            T_PlayBut.isClickable = false
            enotiBut.isClickable = false
            var logoIntent = Intent(this, LogoActivity::class.java)
            startActivity(logoIntent)
        }
    }
}
