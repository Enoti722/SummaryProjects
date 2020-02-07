package com.enoti.enotiquiz;

import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class QuizResultActivity extends AppCompatActivity {

    private TextView rightQuizCounter, resText, coinsCountText;
    private Button toMainMenuBut;
    private SharedPreferences SP = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_quiz_result);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        rightQuizCounter = (TextView)findViewById(R.id.RightQuizCounter);
        resText = (TextView)findViewById(R.id.ResText);
        coinsCountText= (TextView)findViewById(R.id.CoinsCountText);
        toMainMenuBut = (Button)findViewById(R.id.ToMainMenuBut);
        rightQuizCounter.setText(getIntent().getStringExtra("coins") + "/10");
        SP = getSharedPreferences("com.enoti.enotiquiz", MODE_PRIVATE); // Предопределённые переменные
        coinsCountText.setText("Всего монет: " + String.valueOf(Integer.parseInt(getIntent().getStringExtra("Allcoins")) + Integer.parseInt(getIntent().getStringExtra("coins"))));
        SP.edit().putInt("coins", Integer.parseInt(getIntent().getStringExtra("Allcoins")) + Integer.parseInt(getIntent().getStringExtra("coins"))).apply();
        resText.setText(new Questions().resRand((byte)(Integer.parseInt(getIntent().getStringExtra("coins"))), getApplicationContext()));
        addListenerOnButton();
    }

    public void addListenerOnButton () {
        toMainMenuBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        finish();
                    }
                }
        );
    }
}
