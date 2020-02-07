package com.enoti.enotiquiz;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.os.CountDownTimer;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

public class TrFlsActivity extends AppCompatActivity {

    private TextView questionsText, coinsCounter, resText, quizCounter;
    private Button answBut1, answBut2;
    private TrFlsQuestion[] questions = new TrFlsQuestion[10];
    private Questions quizRandomizer;
    private ImageButton hintBut;
    private FrameLayout hintFrag;
    private int allcoins;
    byte quizNum;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tr_fls);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        questionsText = (TextView)findViewById(R.id.Question);
        questionsText.setMovementMethod(new ScrollingMovementMethod());
        coinsCounter = (TextView)findViewById(R.id.CoinsCountText);
        resText= (TextView)findViewById(R.id.ResText);
        quizCounter = (TextView)findViewById(R.id.QuizCounter);
        answBut1 = (Button)findViewById(R.id.TrueBut);
        answBut2 = (Button)findViewById(R.id.FalseBut);

        hintBut = (ImageButton)findViewById(R.id.HintBut);
        hintFrag = (FrameLayout)findViewById(R.id.HintFrag);
        hintFrag.setVisibility(View.INVISIBLE);

        quizRandomizer = new Questions();
        questions = quizRandomizer.trFlsRand((byte)10, getApplicationContext());
        quizCounter.setText("1/10");
        questionsText.setText(questions[0].getQuestion());
        quizNum = 0;
        allcoins = Integer.valueOf(getIntent().getStringExtra("coins"));
        addListenerOnButton();
    }

    @Override
    public void onBackPressed() {

    }

    public void addListenerOnButton() {
        answBut1.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        answBut1.setClickable(false);
                        answBut2.setClickable(false);
                        hintBut.setClickable(false);
                        if (quizNum == 9) {
                            if (questions[quizNum].getRightAnsw() == 1) {
                                coinsCounter.setText(String.valueOf(Integer.valueOf(coinsCounter.getText().toString()) + 1));
                                resText.setText(quizRandomizer.rightRand(getApplicationContext())); // сделать рандом
                                resText.setTextColor(Color.parseColor("#00d000"));
                            }
                            else {
                                resText.setText("Неправильно");
                                resText.setTextColor(Color.parseColor("#cc0000"));
                                // Подсветка верного варианта
                            }
                            resText.setVisibility(View.VISIBLE);
                        }
                        else if (quizNum != 9) {
                            if (questions[quizNum].getRightAnsw() == 1) {
                                coinsCounter.setText(String.valueOf(Integer.valueOf(coinsCounter.getText().toString()) + 1));
                                resText.setText(quizRandomizer.rightRand(getApplicationContext()));
                                resText.setTextColor(Color.parseColor("#00d000"));
                            }
                            else {
                                resText.setText("Неправильно");
                                resText.setTextColor(Color.parseColor("#cc0000"));
                                // Подсветка верного варианта
                            }
                            resText.setVisibility(View.VISIBLE);
                        }
                        new CountDownTimer(1000, 1) {
                            public void onTick(long millisUntilFinished) {

                            }

                            public void onFinish() {
                                if (quizNum == 9) {
                                    Intent resIntent = new Intent("com.enoti.enotiquiz.QuizResultActivity");
                                    resIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                                    resIntent.putExtra("coins", coinsCounter.getText());
                                    resIntent.putExtra("Allcoins", getIntent().getStringExtra("coins"));
                                    startActivity(resIntent);
                                    finish();
                                }
                                else if (quizNum != 9) {
                                    resText.setVisibility(View.INVISIBLE);
                                    if (quizNum != 9) {
                                        quizNum++;
                                        questionsText.setText(questions[quizNum].getQuestion()); // на 10-м вопросе ошибка
                                    }
                                    answBut1.setClickable(true);
                                    answBut2.setClickable(true);
                                    hintBut.setClickable(true);
                                }
                                answBut1.setBackground(getResources().getDrawable(R.drawable.buts));
                                answBut2.setBackground(getResources().getDrawable(R.drawable.buts));
                                quizCounter.setText(String.valueOf(quizNum+1) + "/10");
                            }
                        }.start();
                    }
                }
        );
        answBut2.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        answBut1.setClickable(false);
                        answBut2.setClickable(false);
                        hintBut.setClickable(false);
                        if (quizNum == 9) {
                            if (questions[quizNum].getRightAnsw() == 0) {
                                coinsCounter.setText(String.valueOf(Integer.valueOf(coinsCounter.getText().toString()) + 1));
                                resText.setText(quizRandomizer.rightRand(getApplicationContext())); // сделать рандом
                                resText.setTextColor(Color.parseColor("#00d000"));
                            }
                            else {
                                resText.setText("Неправильно");
                                resText.setTextColor(Color.parseColor("#cc0000"));
                                // Подсветка верного варианта
                            }
                            resText.setVisibility(View.VISIBLE);
                        }
                        else if (quizNum != 9) {
                            if (questions[quizNum].getRightAnsw() == 0) {
                                coinsCounter.setText(String.valueOf(Integer.valueOf(coinsCounter.getText().toString()) + 1));
                                resText.setText(quizRandomizer.rightRand(getApplicationContext()));
                                resText.setTextColor(Color.parseColor("#00d000"));
                            }
                            else {
                                resText.setText("Неправильно");
                                resText.setTextColor(Color.parseColor("#cc0000"));
                                // Подсветка верного варианта
                            }
                            resText.setVisibility(View.VISIBLE);
                        }
                        new CountDownTimer(1000, 1) {
                            public void onTick(long millisUntilFinished) {

                            }

                            public void onFinish() {
                                if (quizNum == 9) {
                                    Intent resIntent = new Intent("com.enoti.enotiquiz.QuizResultActivity");
                                    resIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                                    resIntent.putExtra("coins", coinsCounter.getText());
                                    resIntent.putExtra("Allcoins", getIntent().getStringExtra("coins"));
                                    startActivity(resIntent);
                                    finish();
                                }
                                else if (quizNum != 9) {
                                    resText.setVisibility(View.INVISIBLE);
                                    if (quizNum != 9) {
                                        quizNum++;
                                        questionsText.setText(questions[quizNum].getQuestion()); // на 10-м вопросе ошибка
                                    }
                                    answBut1.setClickable(true);
                                    answBut2.setClickable(true);
                                    hintBut.setClickable(true);
                                }
                                answBut1.setBackground(getResources().getDrawable(R.drawable.buts));
                                answBut2.setBackground(getResources().getDrawable(R.drawable.buts));
                                quizCounter.setText(String.valueOf(quizNum+1) + "/10");
                            }
                        }.start();
                    }
                }
        );
        hintBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        answBut1.setClickable(false);
                        answBut2.setClickable(false);
                        hintBut.setClickable(false);

                        android.support.v4.app.FragmentManager fm = getSupportFragmentManager();
                        android.support.v4.app.Fragment f = new HintFragment();

                        Bundle bundle = new Bundle();
                        bundle.putString("coins", String.valueOf(allcoins));
                        bundle.putByte("mode", (byte)3);
                        f.setArguments(bundle);

                        fm.beginTransaction().replace(R.id.HintFrag, f).commit();
                        hintFrag.setVisibility(View.VISIBLE);
                    }
                }
        );
    }

    public void getHint() {
        if (allcoins >= 5) {
            allcoins -= 5;
            if (questions[quizNum].getRightAnsw() == 1) {
                answBut1.setBackground(getResources().getDrawable(R.drawable.rightbuts));
            } else {
                answBut2.setBackground(getResources().getDrawable(R.drawable.rightbuts));
            }
            hintFrag.setVisibility(View.INVISIBLE);
            answBut1.setClickable(true);
            answBut2.setClickable(true);
        }
        else {
            hintFrag.setVisibility(View.INVISIBLE);
            answBut1.setClickable(true);
            answBut2.setClickable(true);
            hintBut.setClickable(true);
            Toast.makeText(getApplicationContext(), "У вас не хватает монет!", Toast.LENGTH_SHORT).show();
        }
    }

    public void cancelHint() {
        hintFrag.setVisibility(View.INVISIBLE);
        answBut1.setClickable(true);
        answBut2.setClickable(true);
        hintBut.setClickable(true);
    }
}
