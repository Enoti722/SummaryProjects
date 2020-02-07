package com.enoti.enotisudoku;

import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class ResActivity extends AppCompatActivity {

    private Button nextLvl;
    private SharedPreferences SP;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_res);

        SP = getSharedPreferences("com.enoti.enotisudoku", MODE_PRIVATE);
        nextLvl = (Button)findViewById(R.id.NextLvlBut);
        if (SP.getInt("levelNum", 0) == 10) {
            nextLvl.setText("Вы прошли все уровни!");
        }
        else {
            addOnButtonClickListener();
        }
    }

    public void addOnButtonClickListener () {
        nextLvl.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent nextLvlIntent = new Intent("android.intent.action.MAIN");
                nextLvlIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                startActivity(nextLvlIntent);
                finish();
            }
        });
    }
}
