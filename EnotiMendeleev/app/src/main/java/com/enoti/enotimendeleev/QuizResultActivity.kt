package com.enoti.enotimendeleev

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_quiz_result.*

class QuizResultActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_quiz_result)
        @Suppress("RECEIVER_NULLABILITY_MISMATCH_BASED_ON_JAVA_ANNOTATIONS")
        RightQuizCounter.text = "${intent.extras.getInt("rightAnswersCount")}/10"

        addListenerOnButton()
    }

    fun addListenerOnButton() {
        MainMenuButton.setOnClickListener {
            finish()
        }
    }
}
