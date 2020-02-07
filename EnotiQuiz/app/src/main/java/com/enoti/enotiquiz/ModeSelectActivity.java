package com.enoti.enotiquiz;


// Сделать функцию списывания монет при покупке режима


import android.animation.ValueAnimator;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.os.CountDownTimer;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

public class ModeSelectActivity extends AppCompatActivity {

    private TextView coinsCountText;
    private Button quizModeBut, wordModeBut, picModeBut, trFlsModeBut, wordBuyBut, picBuyBut, trFlsBuyBut;
    private SharedPreferences SP = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mode_select);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        coinsCountText = (TextView)findViewById(R.id.CoinsCountText); // Поиск текста с монетками
        quizModeBut = (Button)findViewById(R.id.QuizModeBut); // Поиск кнопки викторины
        wordModeBut = (Button)findViewById(R.id.WordModeBut); // Поиск кнопки угадай слово
        picModeBut = (Button)findViewById(R.id.PicModeBut); // Поиск кнопки угадай по картинке
        trFlsModeBut = (Button)findViewById(R.id.TrFlsModeBut); // Поиск кнопки правда/ложь
        wordBuyBut = (Button)findViewById(R.id.WordBuyBut); // Поиск кнопки покупки 2-го режима
        picBuyBut = (Button)findViewById(R.id.PicBuyBut); // Поиск кнопки покупки 3-го режима
        trFlsBuyBut = (Button)findViewById(R.id.TrFlsBuyBut); // Поиск кнопки покупки 4-го режима
        SP = getSharedPreferences("com.enoti.enotiquiz", MODE_PRIVATE); // Предопределённые переменные
        if (SP.getBoolean("firstrun", true)) { // запихнуть коины в SP или вывести существующие
            // SP.edit().putInt("coins", 180).apply(); // добавление монет для теста
            // coinsCountText.setText("180");
            SP.edit().putBoolean("firstrun", false).apply();
        }
        else {
            coinsCountText.setText(String.valueOf(SP.getInt("coins", 0)));
        }
        if (SP.getBoolean("wordbought", false)) {
            wordBuyBut.setVisibility(View.INVISIBLE); // Если 2-й режим куплен - сделать кнопку покупки невидимой
        }
        if (SP.getBoolean("picbought", false)) {
            picBuyBut.setVisibility(View.INVISIBLE); // Если 3-й режим куплен - сделать кнопку покупки невидимой
        }
        if (SP.getBoolean("trflsbought", false)) {
            trFlsBuyBut.setVisibility(View.INVISIBLE); // Если 4-й режим куплен - сделать кнопку покупки невидимой
        }
        addListenerOnButton(); // Вызов метода опроса кнопок формы
    }

    public void addListenerOnButton () {
        quizModeBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        quizModeBut.setClickable(false);
                        wordModeBut.setClickable(false);
                        picModeBut.setClickable(false);
                        trFlsModeBut.setClickable(false);
                        wordBuyBut.setClickable(false);
                        picBuyBut.setClickable(false);
                        trFlsBuyBut.setClickable(false);
                        Intent quizIntent = new Intent("com.enoti.enotiquiz.QuizActivity");
                        quizIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        quizIntent.putExtra("mode", "1"); // Передача в Intent параметра mode = 1 (определяет какой будет режим: викторина или угадай слово)
                        quizIntent.putExtra("coins", coinsCountText.getText().toString()); // Передача в Intent параметра coins (кол-во монет)
                        startActivity(quizIntent); // Переход на Activity викторины
                        finish(); // завершение работы этой Activity
                    }
                }
        );
        wordModeBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        if (wordBuyBut.getVisibility() == View.INVISIBLE) { // Если режим куплен
                            quizModeBut.setClickable(false);
                            wordModeBut.setClickable(false);
                            picModeBut.setClickable(false);
                            trFlsModeBut.setClickable(false);
                            wordBuyBut.setClickable(false);
                            picBuyBut.setClickable(false);
                            trFlsBuyBut.setClickable(false);

                            Intent wordIntent = new Intent("com.enoti.enotiquiz.QuizActivity");
                            wordIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                            wordIntent.putExtra("mode", "2"); // Передача в Intent параметра mode = 2 (определяет какой будет режим: викторина или угадай слово)
                            wordIntent.putExtra("coins", coinsCountText.getText().toString());
                            startActivity(wordIntent);
                            finish(); // Переход на Activity угадай термин
                        }
                    }
                }
        );
        wordBuyBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        if (Integer.valueOf(coinsCountText.getText().toString()) >= 30) { // Если хватает денег - купить режим и списать коины
                            SP.edit().putBoolean("wordbought", true).apply();
                            modeBought((byte)1);
                            SP.edit().putInt("coins", Integer.valueOf(coinsCountText.getText().toString()));
                            wordBuyBut.setVisibility(View.INVISIBLE);
                        }
                        else {
                            Toast.makeText(
                                    getApplicationContext(),
                                    "Вам не хватает монет!",
                                    Toast.LENGTH_SHORT
                            ).show();
                        }
                    }
                }
        );
        picModeBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        if (picBuyBut.getVisibility() == View.INVISIBLE) { // Если режим куплен
                            quizModeBut.setClickable(false);
                            wordModeBut.setClickable(false);
                            picModeBut.setClickable(false);
                            trFlsModeBut.setClickable(false);
                            wordBuyBut.setClickable(false);
                            picBuyBut.setClickable(false);
                            trFlsBuyBut.setClickable(false);

                            Intent picIntent = new Intent("com.enoti.enotiquiz.PicActivity");
                            picIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                            picIntent.putExtra("coins", coinsCountText.getText().toString()); // Передача в Intent параметра coins (кол-во монет)
                            startActivity(picIntent); // Переход на Activity угадай по картинке
                            finish(); // Переход на Activity угадай термин
                        }
                    }
                }
        );
        picBuyBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        if (Integer.valueOf(coinsCountText.getText().toString()) >= 60) { // Если хватает денег - купить режим и списать коины
                            SP.edit().putBoolean("picbought", true).apply();
                            modeBought((byte)2);
                            SP.edit().putInt("coins", Integer.valueOf(coinsCountText.getText().toString()));
                            picBuyBut.setVisibility(View.INVISIBLE);
                        }
                        else {
                            Toast.makeText(
                                    getApplicationContext(),
                                    "Вам не хватает монет!",
                                    Toast.LENGTH_SHORT
                            ).show();
                        }
                    }
                }
        );
        trFlsModeBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        if (trFlsBuyBut.getVisibility() == View.INVISIBLE) { // Если режим куплен
                            quizModeBut.setClickable(false);
                            wordModeBut.setClickable(false);
                            picModeBut.setClickable(false);
                            trFlsModeBut.setClickable(false);
                            wordBuyBut.setClickable(false);
                            picBuyBut.setClickable(false);
                            trFlsBuyBut.setClickable(false);

                            Intent trFlsIntent = new Intent("com.enoti.enotiquiz.TrFlsActivity");
                            trFlsIntent .setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                            trFlsIntent .putExtra("coins", coinsCountText.getText().toString()); // Передача в Intent параметра coins (кол-во монет)
                            startActivity(trFlsIntent); // Переход на Activity правда/ложь
                            finish(); // Переход на Activity угадай термин
                        }
                    }
                }
        );
        trFlsBuyBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        if (Integer.valueOf(coinsCountText.getText().toString()) >= 90) { // Если хватает денег - купить режим и списать коины
                            SP.edit().putBoolean("trflsbought", true).apply();
//                            coinsCountText.setText(String.valueOf(Integer.valueOf(coinsCountText.getText().toString())-90));
                            modeBought((byte)3);
                            SP.edit().putInt("coins", Integer.valueOf(coinsCountText.getText().toString()));
                            trFlsBuyBut.setVisibility(View.INVISIBLE);
                        }
                        else {
                            Toast.makeText(
                                    getApplicationContext(),
                                    "Вам не хватает монет!",
                                    Toast.LENGTH_SHORT
                            ).show();
                        }
                    }
                }
        );
    }

    // Списывание монет
    public void modeBought(byte modeNum) {
        quizModeBut.setClickable(false);
        wordModeBut.setClickable(false);
        picModeBut.setClickable(false);
        trFlsModeBut.setClickable(false);
        wordBuyBut.setClickable(false);
        picBuyBut.setClickable(false);
        trFlsBuyBut.setClickable(false);
        final int startCoins = Integer.valueOf(coinsCountText.getText().toString());
        ValueAnimator animator;
        if (modeNum == 1) {
            animator = ValueAnimator.ofInt(startCoins, startCoins-30);
            animator.setDuration(300);
            animator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
                @Override
                public void onAnimationUpdate(ValueAnimator updatedAnimation) {
                    coinsCountText.setText(String.valueOf(updatedAnimation.getAnimatedValue()));
                    if (Integer.valueOf(coinsCountText.getText().toString()) == startCoins-30) {
                        quizModeBut.setClickable(true);
                        wordModeBut.setClickable(true);
                        picModeBut.setClickable(true);
                        trFlsModeBut.setClickable(true);
                        wordBuyBut.setClickable(true);
                        picBuyBut.setClickable(true);
                        trFlsBuyBut.setClickable(true);
                    }
                }
            });
            animator.start();
        }
        else if (modeNum == 2) {
            animator = ValueAnimator.ofInt(startCoins, startCoins-60);
            animator.setDuration(600);
            animator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
                @Override
                public void onAnimationUpdate(ValueAnimator updatedAnimation) {
                    coinsCountText.setText(String.valueOf(updatedAnimation.getAnimatedValue()));
                    if (Integer.valueOf(coinsCountText.getText().toString()) == startCoins-60) {
                        quizModeBut.setClickable(true);
                        wordModeBut.setClickable(true);
                        picModeBut.setClickable(true);
                        trFlsModeBut.setClickable(true);
                        wordBuyBut.setClickable(true);
                        picBuyBut.setClickable(true);
                        trFlsBuyBut.setClickable(true);
                    }
                }
            });
            animator.start();
        }
        else {
            animator = ValueAnimator.ofInt(startCoins, startCoins-90);
            animator.setDuration(900);
            animator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
                @Override
                public void onAnimationUpdate(ValueAnimator updatedAnimation) {
                    coinsCountText.setText(String.valueOf(updatedAnimation.getAnimatedValue()));
                    if (Integer.valueOf(coinsCountText.getText().toString()) == startCoins-90) {
                        quizModeBut.setClickable(true);
                        wordModeBut.setClickable(true);
                        picModeBut.setClickable(true);
                        trFlsModeBut.setClickable(true);
                        wordBuyBut.setClickable(true);
                        picBuyBut.setClickable(true);
                        trFlsBuyBut.setClickable(true);
                    }
                }
            });
            animator.start();
        }
    }
}
