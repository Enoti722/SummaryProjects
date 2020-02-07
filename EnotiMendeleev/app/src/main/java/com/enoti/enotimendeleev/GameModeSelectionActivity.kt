package com.enoti.enotimendeleev

import android.content.Intent
import android.content.pm.ActivityInfo
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_game_mode_selection.*

class GameModeSelectionActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_game_mode_selection)
        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT // Только портретная ориентация
        addListenerOnButton()
    }

    override fun onResume() {
        super.onResume()
        T_Mode1.isClickable = true
        T_QuizMode.isClickable = true
    }

    fun addListenerOnButton() {
        T_Mode1.setOnClickListener {
            T_Mode1.isClickable = false
            T_QuizMode.isClickable = false
            var ModeSelectionIntent = Intent(this, ModeSelectionActivity::class.java)
            ModeSelectionIntent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP
            startActivity(ModeSelectionIntent)
            onPause()
        }
        T_QuizMode.setOnClickListener {
            T_Mode1.isClickable = false
            T_QuizMode.isClickable = false
            var ModeSelectionIntent = Intent(this, QuizModeSelectionActivity::class.java)
            ModeSelectionIntent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP
            startActivity(ModeSelectionIntent)
            onPause()
        }
    }
}
