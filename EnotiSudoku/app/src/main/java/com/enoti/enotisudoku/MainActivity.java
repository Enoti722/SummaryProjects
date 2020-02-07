package com.enoti.enotisudoku;

import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.GridView;
import android.widget.ListAdapter;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.Arrays;
import java.util.Random;

public class MainActivity extends AppCompatActivity {

    private static final int[][] ids = new int[][] {
            { R.id.But_1_1, R.id.But_1_2, R.id.But_1_3, R.id.But_1_4, R.id.But_1_5, R.id.But_1_6, R.id.But_1_7, R.id.But_1_8, R.id.But_1_9 },
            { R.id.But_2_1, R.id.But_2_2, R.id.But_2_3, R.id.But_2_4, R.id.But_2_5, R.id.But_2_6, R.id.But_2_7, R.id.But_2_8, R.id.But_2_9 },
            { R.id.But_3_1, R.id.But_3_2, R.id.But_3_3, R.id.But_3_4, R.id.But_3_5, R.id.But_3_6, R.id.But_3_7, R.id.But_3_8, R.id.But_3_9 },
            { R.id.But_4_1, R.id.But_4_2, R.id.But_4_3, R.id.But_4_4, R.id.But_4_5, R.id.But_4_6, R.id.But_4_7, R.id.But_4_8, R.id.But_4_9 },
            { R.id.But_5_1, R.id.But_5_2, R.id.But_5_3, R.id.But_5_4, R.id.But_5_5, R.id.But_5_6, R.id.But_5_7, R.id.But_5_8, R.id.But_5_9 },
            { R.id.But_6_1, R.id.But_6_2, R.id.But_6_3, R.id.But_6_4, R.id.But_6_5, R.id.But_6_6, R.id.But_6_7, R.id.But_6_8, R.id.But_6_9 },
            { R.id.But_7_1, R.id.But_7_2, R.id.But_7_3, R.id.But_7_4, R.id.But_7_5, R.id.But_7_6, R.id.But_7_7, R.id.But_7_8, R.id.But_7_9 },
            { R.id.But_8_1, R.id.But_8_2, R.id.But_8_3, R.id.But_8_4, R.id.But_8_5, R.id.But_8_6, R.id.But_8_7, R.id.But_8_8, R.id.But_8_9 },
            { R.id.But_9_1, R.id.But_9_2, R.id.But_9_3, R.id.But_9_4, R.id.But_9_5, R.id.But_9_6, R.id.But_9_7, R.id.But_9_8, R.id.But_9_9 }
    };

    private char[][] str = new char[][]{
            {' ', ' ', ' ', '5', ' ', ' ', '3', ' ', ' ',},
            {' ', '7', ' ', ' ', '3', '2', ' ', ' ', '5',},
            {' ', '3', ' ', '7', '6', ' ', ' ', ' ', '9',},
            {' ', ' ', ' ', '4', ' ', '7', ' ', ' ', '8',},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', '3', ' ',},
            {'2', '5', ' ', ' ', ' ', ' ', '9', ' ', '7',},
            {'7', '2', ' ', '3', ' ', '9', '5', ' ', ' ',},
            {'8', '9', ' ', '2', ' ', ' ', ' ', ' ', ' ',},
            {' ', ' ', '5', ' ', ' ', ' ', ' ', ' ', '6',}
    };

    private char[][] curStr = new char[][]{
            {' ', ' ', ' ', '5', ' ', ' ', '3', ' ', ' ',},
            {' ', '7', ' ', ' ', '3', '2', ' ', ' ', '5',},
            {' ', '3', ' ', '7', '6', ' ', ' ', ' ', '9',},
            {' ', ' ', ' ', '4', ' ', '7', ' ', ' ', '8',},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', '3', ' ',},
            {'2', '5', ' ', ' ', ' ', ' ', '9', ' ', '7',},
            {'7', '2', ' ', '3', ' ', '9', '5', ' ', ' ',},
            {'8', '9', ' ', '2', ' ', ' ', ' ', ' ', ' ',},
            {' ', ' ', '5', ' ', ' ', ' ', ' ', ' ', '6',}
    };

    char curNum = ' ';
    private Button but;
    private SharedPreferences SP;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        SP = getSharedPreferences("com.enoti.enotisudoku", MODE_PRIVATE);
        if (SP.getInt("levelNum", 0) == 10) {
            SP.edit().putInt("levelNum", 1).apply();
        }
        int n = SP.getInt("levelNum", 0);
        InputStream is = getResources().openRawResource(R.raw.levels);
        BufferedReader br = new BufferedReader(new InputStreamReader(is));
        // Нахождение нужной строки
        for (int i = 0; i < n * 10; i++) {
            try {
                br.readLine();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        for (int i = 0; i < 9; i++) {
            try {
                String s = br.readLine();
                str[i] = s.toCharArray();
                curStr[i] = s.toCharArray();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                but = (Button)findViewById(ids[i][j]);
                but.setText(String.valueOf(str[i][j]));
            }
        }
        but = (Button)findViewById(R.id.But_1);
        but.setOnClickListener(addOnButtonClickListener());
        but = (Button)findViewById(R.id.But_2);
        but.setOnClickListener(addOnButtonClickListener());
        but = (Button)findViewById(R.id.But_3);
        but.setOnClickListener(addOnButtonClickListener());
        but = (Button)findViewById(R.id.But_4);
        but.setOnClickListener(addOnButtonClickListener());
        but = (Button)findViewById(R.id.But_5);
        but.setOnClickListener(addOnButtonClickListener());
        but = (Button)findViewById(R.id.But_6);
        but.setOnClickListener(addOnButtonClickListener());
        but = (Button)findViewById(R.id.But_7);
        but.setOnClickListener(addOnButtonClickListener());
        but = (Button)findViewById(R.id.But_8);
        but.setOnClickListener(addOnButtonClickListener());
        but = (Button)findViewById(R.id.But_9);
        but.setOnClickListener(addOnButtonClickListener());
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                but = (Button) findViewById(ids[i][j]);
                but.setOnClickListener(addOnButtonClickListener());
            }
        }
    }

    public View.OnClickListener addOnButtonClickListener () {
        return new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                switch (v.getId()) {
                    case R.id.But_1:
                        but = (Button)findViewById(R.id.But_1);
                        if(but.getCurrentTextColor() == Color.BLACK) {
                            but.setTextColor(Color.BLUE);
                            but = (Button)findViewById(R.id.But_2);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_3);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_4);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_5);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_6);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_7);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_8);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_9);
                            but.setTextColor(Color.BLACK);
                            curNum = '1';
                        }
                        else {
                            but.setTextColor(Color.BLACK);
                            curNum = ' ';
                        }
                        break;
                    case R.id.But_2:
                        but = (Button)findViewById(R.id.But_2);
                        if(but.getCurrentTextColor() == Color.BLACK) {
                            but.setTextColor(Color.BLUE);
                            but = (Button)findViewById(R.id.But_1);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_3);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_4);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_5);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_6);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_7);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_8);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_9);
                            but.setTextColor(Color.BLACK);
                            curNum = '2';
                        }
                        else {
                            but.setTextColor(Color.BLACK);
                            curNum = ' ';
                        }
                        break;
                    case R.id.But_3:
                        but = (Button)findViewById(R.id.But_3);
                        if(but.getCurrentTextColor() == Color.BLACK) {
                            but.setTextColor(Color.BLUE);
                            but = (Button)findViewById(R.id.But_1);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_2);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_4);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_5);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_6);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_7);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_8);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_9);
                            but.setTextColor(Color.BLACK);
                            curNum = '3';
                        }
                        else {
                            but.setTextColor(Color.BLACK);
                            curNum = ' ';
                        }
                        break;
                    case R.id.But_4:
                        but = (Button)findViewById(R.id.But_4);
                        if(but.getCurrentTextColor() == Color.BLACK) {
                            but.setTextColor(Color.BLUE);
                            but = (Button)findViewById(R.id.But_1);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_2);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_3);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_5);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_6);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_7);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_8);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_9);
                            but.setTextColor(Color.BLACK);
                            curNum = '4';
                        }
                        else {
                            but.setTextColor(Color.BLACK);
                            curNum = ' ';
                        }
                        break;
                    case R.id.But_5:
                        but = (Button)findViewById(R.id.But_5);
                        if(but.getCurrentTextColor() == Color.BLACK) {
                            but.setTextColor(Color.BLUE);
                            but = (Button)findViewById(R.id.But_1);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_2);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_3);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_4);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_6);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_7);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_8);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_9);
                            but.setTextColor(Color.BLACK);
                            curNum = '5';
                        }
                        else {
                            but.setTextColor(Color.BLACK);
                            curNum = ' ';
                        }
                        break;
                    case R.id.But_6:
                        but = (Button)findViewById(R.id.But_6);
                        if(but.getCurrentTextColor() == Color.BLACK) {
                            but.setTextColor(Color.BLUE);
                            but = (Button)findViewById(R.id.But_1);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_2);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_3);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_4);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_5);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_7);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_8);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_9);
                            but.setTextColor(Color.BLACK);
                            curNum = '6';
                        }
                        else {
                            but.setTextColor(Color.BLACK);
                            curNum = ' ';
                        }
                        break;
                    case R.id.But_7:
                        but = (Button)findViewById(R.id.But_7);
                        if(but.getCurrentTextColor() == Color.BLACK) {
                            but.setTextColor(Color.BLUE);
                            but = (Button)findViewById(R.id.But_1);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_2);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_3);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_4);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_5);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_6);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_8);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_9);
                            but.setTextColor(Color.BLACK);
                            curNum = '7';
                        }
                        else {
                            but.setTextColor(Color.BLACK);
                            curNum = ' ';
                        }
                        break;
                    case R.id.But_8:
                        but = (Button)findViewById(R.id.But_8);
                        if(but.getCurrentTextColor() == Color.BLACK) {
                            but.setTextColor(Color.BLUE);
                            but = (Button)findViewById(R.id.But_1);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_2);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_3);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_4);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_5);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_6);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_7);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_9);
                            but.setTextColor(Color.BLACK);
                            curNum = '8';
                        }
                        else {
                            but.setTextColor(Color.BLACK);
                            curNum = ' ';
                        }
                        break;
                    case R.id.But_9:
                        but = (Button)findViewById(R.id.But_9);
                        if(but.getCurrentTextColor() == Color.BLACK) {
                            but.setTextColor(Color.BLUE);
                            but = (Button)findViewById(R.id.But_1);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_2);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_3);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_4);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_5);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_6);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_7);
                            but.setTextColor(Color.BLACK);
                            but = (Button)findViewById(R.id.But_8);
                            but.setTextColor(Color.BLACK);
                            curNum = '9';
                        }
                        else {
                            but.setTextColor(Color.BLACK);
                            curNum = ' ';
                        }
                        break;
                    default:
                        for (int i = 0; i < 9; i++) {
                            for (int j = 0; j < 9; j++) {
                                if (v.getId() == ids[i][j]) {
                                    if (checkFields(i, j)) {
                                        Intent quitgame = new Intent("android.intent.action.ResActivity");
                                        quitgame.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                                        SP.edit().putInt("levelNum", SP.getInt("levelNum", 0)+1).apply();
                                        startActivity(quitgame);
                                        finish();
                                        i = 9;
                                        break;
                                    }
                                }
                            }
                        }
                        break;
                }
            }
        };
    }

    public boolean checkFields (int a, int b) {
        if (str[a][b] == ' ') {
            but = (Button)findViewById(ids[a][b]);
            if (but.getText().toString().equals(String.valueOf(curNum))) {
                but.setText(" ");
                curStr[a][b] = ' ';
            } else {
                but.setText(String.valueOf(curNum));
                curStr[a][b] = curNum;
            }
        }
        else
        {
            return false;
        }
        for (int i = 0; i < 9; i++) { // Есть ли пустые
            for (int j = 0; j < 9; j++) {
                if (curStr[i][j] == ' ')
                {
                    return false;
                }
            }
        }
        for (int i = 0; i < 9; i+=3) { // Проверка по квадратам
            for (int j = i; j < 9; j+=3) {
                char[] s = new char[] {curStr[i][j], curStr[i][j+1], curStr[i][j+2], curStr[i+1][j], curStr[i+1][j+1], curStr[i+1][j+2], curStr[i+2][j], curStr[i+2][j+1], curStr[i+2][j+2] };
                for (int k = 0; k < 9; k++) {
                    for (int n = k+1; n < 9; n++) {
                        if (s[k] == s[n]) {
                            Toast.makeText(getApplicationContext(), "Ошибка решения", Toast.LENGTH_SHORT).show();
                            return false;
                        }
                    }
                }
            }
        }
        for (int i = 0; i < 9; i++) { // Проверка по строкам
            for (int j = 0; j < 9; j++) {
                for (int k = j+1; k < 9; k++) {
                    if (curStr[i][j] == curStr[i][k]) {
                        Toast.makeText(getApplicationContext(), "Ошибка решения", Toast.LENGTH_SHORT).show();
                        return false;
                    }
                }
            }
        }
        for (int i = 0; i < 9; i++) { // Проверка по столбцам
            for (int j = 0; j < 9; j++) {
                for (int k = j+1; k < 9; k++) {
                    if (curStr[j][i] == curStr[k][i]) {
                        Toast.makeText(getApplicationContext(), "Ошибка решения", Toast.LENGTH_SHORT).show();
                        return false;
                    }
                }
            }
        }
        return true;
    }
}
