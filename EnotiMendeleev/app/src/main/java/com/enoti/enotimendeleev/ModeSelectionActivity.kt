package com.enoti.enotimendeleev

import android.content.Intent
import android.content.pm.ActivityInfo
import android.graphics.Color
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_mode_selection.*
import kotlinx.android.synthetic.main.activity_start.*

class ModeSelectionActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_mode_selection)
        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT
        addListenerOnButton()
    }

    override fun onResume() {
        super.onResume()
        changeButtonsStates()
    }

    fun changeButtonsStates () { // Блокирование кнопок, если предыдущий уровень не пройден
        val SP = getSharedPreferences("com.enoti.enotimendeleev", MODE_PRIVATE)
        if (!SP.getBoolean("geographyModeUnlocked", false)) {
            T_GeographyModeBut.isClickable = false
            T_GeographyModeBut.setTextColor(Color.parseColor("#aaaaaa"))
            T_HardcoreModeBut.isClickable = false
            T_HardcoreModeBut.setTextColor(Color.parseColor("#aaaaaa"))
        }
        else if (!SP.getBoolean("hardModeUnlocked", false)) {
            T_HardcoreModeBut.isClickable = false
            T_HardcoreModeBut.setTextColor(Color.parseColor("#aaaaaa"))
        }
        else {
            T_GeographyModeBut.isClickable = true
            T_GeographyModeBut.setTextColor(Color.parseColor("#00ff00"))
            T_HardcoreModeBut.isClickable = true
            T_HardcoreModeBut.setTextColor(Color.parseColor("#00ff00"))
        }
    }

    fun addListenerOnButton () { // Сделать очистку массива Boolean в SharedPreferences, если уровень уже пройден
        T_UsualModeBut.setOnClickListener {
            T_UsualModeBut.isClickable = false
            T_GeographyModeBut.isClickable = false
            T_HardcoreModeBut.isClickable = false
            var GameScreenIntent = Intent(this, GameActivity::class.java)
            GameScreenIntent.putExtra("mode", 1)
            GameScreenIntent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP
            startActivity(GameScreenIntent)
            onPause()
        }

        T_GeographyModeBut.setOnClickListener {
            T_UsualModeBut.isClickable = false
            T_GeographyModeBut.isClickable = false
            T_HardcoreModeBut.isClickable = false
            var GameScreenIntent = Intent(this, GameActivity::class.java)
            GameScreenIntent.putExtra("mode", 2)
            GameScreenIntent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP
            startActivity(GameScreenIntent)
            onPause()
        }

        T_HardcoreModeBut.setOnClickListener {
            T_UsualModeBut.isClickable = false
            T_GeographyModeBut.isClickable = false
            T_HardcoreModeBut.isClickable = false
            var GameScreenIntent = Intent(this, GameActivity::class.java)
            GameScreenIntent.putExtra("mode", 3)
            GameScreenIntent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP
            startActivity(GameScreenIntent)
            onPause()
        }
    }
}
