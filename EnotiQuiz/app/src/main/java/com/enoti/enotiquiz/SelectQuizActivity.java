// Временно не нужно!

package com.enoti.enotiquiz;

import android.content.pm.ActivityInfo;
import android.os.Environment;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

public class SelectQuizActivity extends AppCompatActivity {

    private Button cosmosBut;
    private TextView coinsCountText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_select_quiz);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        coinsCountText = (TextView)findViewById(R.id.CoinsCountText);
        /*if (getIntent().getBooleanExtra("firstRun", true)) {
            File externalAppDir = new File(Environment.getExternalStorageDirectory() + "/Android/data/" + getPackageName());
            if (!externalAppDir.exists()) {
                externalAppDir.mkdir();
            }
            File file = new File(externalAppDir, "save.txt");
            try {
                file.createNewFile();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
            try {
                FileOutputStream fileOutput = openFileOutput("save.txt", MODE_PRIVATE);
                fileOutput.write(0);
                fileOutput.close();
            } catch (FileNotFoundException e1) {
                e1.printStackTrace();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
            coinsCountText.setText("0");
        }
        else {
            try {
                FileInputStream fileInput = openFileInput("save.txt");
                InputStreamReader reader = new InputStreamReader(fileInput);
                BufferedReader buffer = new BufferedReader(reader);
                int str = buffer.read();
                coinsCountText.setText(String.valueOf(str));
                fileInput.close();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }*/
        addListenerOnButton();
    }

    public void addListenerOnButton () {
        cosmosBut = (Button)findViewById(R.id.CosmosBut);
        cosmosBut.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        Intent quizIntent = new Intent("com.enoti.enotiquiz.ModeSelectActivity");
                        quizIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        quizIntent.putExtra("coins", coinsCountText.getText().toString());
                        startActivity(quizIntent);
                        finish();
                    }
                }
        );
    }

    /*public void setText() {
        FragmentManager fm = getSupportFragmentManager();
        coinsFrag = (CoinsFragment)fm.findFragmentById(R.id.CoinsFrag);
        if (coinsFrag != null)
            coinsFrag.setCoinsText("999999");
    }*/
}
