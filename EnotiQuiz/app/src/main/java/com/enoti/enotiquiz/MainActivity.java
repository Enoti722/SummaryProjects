package com.enoti.enotiquiz;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    TextView histBut, quizBut;
    private ImageButton enotiBut, nejdanBut;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT); // Только портретная (вертикальная) ориентация экрана
        addListenerOnButton(); // Вызов метода опроса кнопок формы
    }

    @Override
    protected void onResume() {
        super.onResume();
        quizBut.setClickable(true);
        histBut.setClickable(true);
        enotiBut.setClickable(true);
        nejdanBut.setClickable(true);
    }

    public void addListenerOnButton () {
        quizBut = (TextView)findViewById(R.id.QuizBut); // Поиск кнопки игры
        histBut = (TextView)findViewById(R.id.HistBut); // Поиск кнопки истории
        enotiBut = (ImageButton)findViewById(R.id.enotiBut); // Поиск кнопки Енота
        nejdanBut = (ImageButton)findViewById(R.id.nejdanBut); // Поиск кнопки Неждана
        quizBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        quizBut.setClickable(false);
                        histBut.setClickable(false);
                        enotiBut.setClickable(false);
                        nejdanBut.setClickable(false);
                        Intent selectQuizIntent = new Intent("com.enoti.enotiquiz.ModeSelectActivity"); //
                        startActivity(selectQuizIntent); // Переход на Activity выбора режима
                    }
                }
        );
        histBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        quizBut.setClickable(false);
                        histBut.setClickable(false);
                        enotiBut.setClickable(false);
                        nejdanBut.setClickable(false);
                        Intent selectQuizIntent = new Intent("com.enoti.enotiquiz.HistoryActivity"); //
                        startActivity(selectQuizIntent); // Переход на Activity выбора режима
                    }
                }
        );
        enotiBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        quizBut.setClickable(false);
                        histBut.setClickable(false);
                        enotiBut.setClickable(false);
                        nejdanBut.setClickable(false);
                        Intent LogoIntent = new Intent("com.enoti.enotiquiz.LogoActivity"); //
                        LogoIntent.putExtra("logo", "enoti"); // Передача в Intent параметра logo = enoti (определит какую картинку показать)
                        startActivity(LogoIntent); // Переход на Activity выбора режима
                    }
                }
        );
        nejdanBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        quizBut.setClickable(false);
                        histBut.setClickable(false);
                        enotiBut.setClickable(false);
                        nejdanBut.setClickable(false);
                        Intent LogoIntent = new Intent("com.enoti.enotiquiz.LogoActivity"); //
                        LogoIntent.putExtra("logo", "nejdan"); // Передача в Intent параметра logo = nejdan (определит какую картинку показать)
                        startActivity(LogoIntent); // Переход на Activity выбора режима
                    }
                }
        );
    }
}
