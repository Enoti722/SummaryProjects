package com.enoti.enotimendeleev

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import kotlinx.android.synthetic.main.activity_result.*

class ResultActivity : AppCompatActivity() {

    val questCountEasy = 91
    val questCountGeography = 17
    val questCountHard = 24

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_result)

        @Suppress("RECEIVER_NULLABILITY_MISMATCH_BASED_ON_JAVA_ANNOTATIONS")
        val Mode = intent.extras.getInt("mode")

        var SP = getSharedPreferences("com.enoti.enotimendeleev", MODE_PRIVATE)
        var unlockedMode = ""
        var mode = ""
        var count = 0
        var unlocked = ""

        when(Mode) {
            1 -> {
                if (!SP.getBoolean("geographyModeUnlocked", false)) {
                    unlockedMode = "География"
                    mode = "easy"
                    count = questCountEasy
                    unlocked = "geography"
                }
            }
            2 -> {
                if (!SP.getBoolean("hardModeUnlocked", false)) {
                    unlockedMode = "Сложный"
                    mode = "geography"
                    unlocked = "hard"
                    count = questCountGeography
                }
            }
            3 -> {
                count = questCountHard
                mode = "hard"
            }
        }

        if (unlockedMode.isNullOrEmpty()) {
            T_ModeUnlocked.visibility = View.INVISIBLE
            T_UnlockedMode.visibility = View.INVISIBLE
        }
        else {
            T_UnlockedMode.text = unlockedMode
        }

        // Очистка массива и Complete переменной в SP

        SP.edit().putBoolean("${mode}ModeCompleted", false).commit()
        SP.edit().putBoolean("${unlocked}ModeUnlocked", true).commit()

        for (i in (0..count)) {
            SP.edit().putBoolean("${mode}Quest_${i}", false).commit()
        }

        addListenerOnButton()
    }

    fun addListenerOnButton() {
        ModesBut.setOnClickListener {
            finish()
        }
    }
}
