package com.enoti.enotimendeleev

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_quiz_mode_selection.*

class QuizModeSelectionActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_quiz_mode_selection)
        addListenerOnButton()
    }

    override fun onResume() {
        super.onResume()
        T_QMode1.isClickable = true
        T_QMode2.isClickable = true
        T_QMode3.isClickable = true
    }

    fun addListenerOnButton() {
        T_QMode1.setOnClickListener {
            T_QMode1.isClickable = false
            T_QMode2.isClickable = false
            T_QMode3.isClickable = false
            var Mode1ScreenIntent = Intent(this, QMode1Activity::class.java)
            Mode1ScreenIntent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP
            startActivity(Mode1ScreenIntent)
            onPause()
        }
        T_QMode2.setOnClickListener {
            T_QMode1.isClickable = false
            T_QMode2.isClickable = false
            T_QMode3.isClickable = false
            var GameScreenIntent = Intent(this, QMode2Activity::class.java)
            GameScreenIntent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP
            startActivity(GameScreenIntent)
            onPause()
        }
        T_QMode3.setOnClickListener {
            T_QMode1.isClickable = false
            T_QMode2.isClickable = false
            T_QMode3.isClickable = false
            var GameScreenIntent = Intent(this, QMode3Activity::class.java)
            GameScreenIntent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP
            startActivity(GameScreenIntent)
            onPause()
        }
    }
}
