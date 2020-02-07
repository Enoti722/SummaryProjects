package com.enoti.enotiquiz;

import android.content.pm.ActivityInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.widget.ImageView;

public class LogoActivity extends AppCompatActivity {

    private ImageView logo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_logo);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        logo = (ImageView)findViewById(R.id.Logo);
        if (getIntent().getStringExtra("logo").equals("enoti")) { // Получение и высвет нужного логотипа
            logo.setImageDrawable(getResources().getDrawable(R.drawable.enotioriginal));
        }
        else if (getIntent().getStringExtra("logo").equals("nejdan")) {
            logo.setImageDrawable(getResources().getDrawable(R.drawable.nejdanoriginal));
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) { // Нажатие на экран - выход из Activity
        finish();
        return true;
    }
}
