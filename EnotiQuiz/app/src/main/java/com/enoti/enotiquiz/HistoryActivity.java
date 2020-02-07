package com.enoti.enotiquiz;

import android.content.pm.ActivityInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

public class HistoryActivity extends AppCompatActivity {

    private TextView historyText, pageCounter;
    private Button toMainMenuBut;
    private ImageButton nextBut, prevBut;
    private Questions history;
    private int page;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_history);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        historyText = (TextView)findViewById(R.id.HistoryText);
        pageCounter = (TextView)findViewById(R.id.PageCounter);
        prevBut = (ImageButton)findViewById(R.id.PrevBut);
        nextBut = (ImageButton)findViewById(R.id.NextBut);
        toMainMenuBut = (Button)findViewById(R.id.ToMainMenuBut);
        history = new Questions();
        page = 1;
        historyText.setText(history.getHist(getApplicationContext(), page));
        historyText.setMovementMethod(new ScrollingMovementMethod());
        pageCounter.setText("1/48");
        addListenerOnButton();
    }

    public void addListenerOnButton () { // Перелистывание страниц кнопками
        prevBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        if (page > 1 && page <= 48) {
                            page-=1;
                            pageCounter.setText(page + "/48");
                            historyText.setText(history.getHist(getApplicationContext(), page));
                        }
                        else if (page == 1) { // Перелистывание с 1-й страницы на последнюю
                            page = 48;
                            pageCounter.setText(page + "/48");
                            historyText.setText(history.getHist(getApplicationContext(), page));
                        }
                    }
                }
        );
        nextBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        if (page >= 1 && page < 48) {
                            page+=1;
                            pageCounter.setText(page + "/48");
                            historyText.setText(history.getHist(getApplicationContext(), page));
                        }
                        else if (page == 48) { // Перелистывание с последней страницы на 1-ю
                            page = 1;
                            pageCounter.setText(page + "/48");
                            historyText.setText(history.getHist(getApplicationContext(), page));
                        }
                    }
                }
        );
        toMainMenuBut.setOnClickListener( // Переход обратно в главное меню
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        finish();
                    }
                }
        );
    }
}
