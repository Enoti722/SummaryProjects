// Работа с файлами вопросов, истории и т. д.

package com.enoti.enotiquiz;

import android.content.Context;
import android.graphics.drawable.Drawable;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.Random;

public class Questions {

    public String getHist (Context context, int page) { // Получение следующей страницы истории из файла
        // Считывание файла
        BufferedReader br;
        InputStream is;
        int n = page-1;
        is = context.getResources().openRawResource(R.raw.history);
        try {
            br = new BufferedReader(new InputStreamReader(is, "UTF-8"));
        } catch (UnsupportedEncodingException e) {
            br = new BufferedReader(new InputStreamReader(is));
            e.printStackTrace();
        }
        // Нахождение нужной строки
        for (int i = 0; i < n; i++) {
            try {
                br.readLine();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        // Её считывание
        char[] str;
        try {
            str = br.readLine().toCharArray();
            is.close();
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
            try {
                is.close();
                br.close();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
            return "";
        }
        // Её форматирование
        for (int i = 0; i < str.length; i++) {
            if (str[i] == '|') {
                str[i] = '\n';
            }
        }
        return String.valueOf(str);
    }

    public QuizQuestion[] quizRand(byte kol, Context context) { // Рандом вопроса Викторины из файла
        // Считывание файла
        QuizQuestion[] arr = new QuizQuestion[10];
        QuizQuestion nQuiz;
        BufferedReader br;
        InputStream is;
        int n;
        for (int j = 0; j < 10; j++) {
            n = new Random().nextInt(228);
            is = context.getResources().openRawResource(R.raw.quizquestions);
            try {
                br = new BufferedReader(new InputStreamReader(is, "UTF-8"));
            } catch (UnsupportedEncodingException e) {
                br = new BufferedReader(new InputStreamReader(is));
                e.printStackTrace();
            }
            // Нахождение нужной строки
            for (int i = 0; i < n * 6; i++) {
                try {
                    br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            // Проверка на вхождение вопроса в массив и запись в массив
            try {
                nQuiz = new QuizQuestion(Short.valueOf(br.readLine()), br.readLine(), new String[]{br.readLine(), br.readLine(), br.readLine()}, br.readLine());
                if (!QuizContains(arr, nQuiz, j)) {
                    arr[j] = nQuiz;
                }
                else {
                    j--;
                }
                is.close();
                br.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return arr;
    }

    public QuizQuestion[] wordRand(byte kol, Context context) { // Рандом вопроса Угадай термин из файла
        QuizQuestion[] arr = new QuizQuestion[10];
        QuizQuestion nQuiz;
        BufferedReader br;
        InputStream is;
        int n;
        for (int j = 0; j < 10; j++) {
            n = new Random().nextInt(96);
            is = context.getResources().openRawResource(R.raw.wordquestions);
            try {
                br = new BufferedReader(new InputStreamReader(is, "UTF-8"));
            } catch (UnsupportedEncodingException e) {
                br = new BufferedReader(new InputStreamReader(is));
                e.printStackTrace();
            }
            for (int i = 0; i < n * 6; i++) {
                try {
                    br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            try {
                nQuiz = new QuizQuestion(Short.valueOf(br.readLine()), br.readLine(), new String[]{br.readLine(), br.readLine(), br.readLine()}, br.readLine());
                if (!QuizContains(arr, nQuiz, j)) {
                    arr[j] = nQuiz;
                }
                else {
                    j--;
                }
                is.close();
                br.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return arr;
    }

    public PicQuestion[] picRand(byte kol, Context context) { // Рандом вопроса Угадай по картинке из файла
        PicQuestion[] arr = new PicQuestion[10];
        PicQuestion nQuiz;
        BufferedReader br;
        InputStream is;
        int n;
        for (int j = 0; j < 10; j++) {
            n = new Random().nextInt(30);
            is = context.getResources().openRawResource(R.raw.picquestions);
            try {
                br = new BufferedReader(new InputStreamReader(is, "UTF-8"));
            } catch (UnsupportedEncodingException e) {
                br = new BufferedReader(new InputStreamReader(is));
                e.printStackTrace();
            }
            for (int i = 0; i < n * 5; i++) {
                try {
                    br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            try {
                nQuiz = new PicQuestion(br.readLine(), new String[]{br.readLine(), br.readLine(), br.readLine()}, br.readLine(), getNewPic(n+1, context));
                if (!PicContains(arr, nQuiz, j)) {
                    arr[j] = nQuiz;
                }
                else {
                    j--;
                }
                is.close();
                br.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return arr;
    }

    public TrFlsQuestion[] trFlsRand(byte kol, Context context) { // Рандом вопроса Правда/Ложь из файла
        TrFlsQuestion[] arr = new TrFlsQuestion[10];
        TrFlsQuestion nQuiz;
        BufferedReader br;
        InputStream is;
        int n;
        for (int j = 0; j < 10; j++) {
            n = new Random().nextInt(135);
            is = context.getResources().openRawResource(R.raw.trflsquestions);
            try {
                br = new BufferedReader(new InputStreamReader(is, "UTF-8"));
            } catch (UnsupportedEncodingException e) {
                br = new BufferedReader(new InputStreamReader(is));
                e.printStackTrace();
            }
            for (int i = 0; i < n * 3; i++) {
                try {
                    br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            try {
                nQuiz = new TrFlsQuestion(Short.valueOf(br.readLine()), br.readLine(), Byte.valueOf(br.readLine()));
                if (!TrFlsContains(arr, nQuiz, j)) {
                    arr[j] = nQuiz;
                } else {
                    j--;
                }
                is.close();
                br.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return arr;
    }

    public String rightRand(Context context) { // Рандом надписи при верном ответе из файла
        InputStream is = context.getResources().openRawResource(R.raw.rightansw);
        BufferedReader br = new BufferedReader(new InputStreamReader(is));
        int n = new Random().nextInt(8);
        for (int i = 1; i < n; i++) {
            try {
                br.readLine();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        try {
            return br.readLine();
        } catch (IOException e) {
            e.printStackTrace();
            return "";
        }
    }

    public String wrongRand(Context context) { // Рандом надписи при неверном ответе из файла
        InputStream is = context.getResources().openRawResource(R.raw.wrongansw);
        BufferedReader br = new BufferedReader(new InputStreamReader(is));
        int n = new Random().nextInt(8);
        for (int i = 1; i < n; i++) {
            try {
                br.readLine();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        try {
            return br.readLine();
        } catch (IOException e) {
            e.printStackTrace();
            return "";
        }
    }

    public String resRand(byte kol, Context context) { // Рандом надписи на Activity результата игры из файла
        InputStream is = context.getResources().openRawResource(R.raw.resstr);
        BufferedReader br = new BufferedReader(new InputStreamReader(is));
        if (kol == 0) {
            for(int i = 0; i < new Random().nextInt(2) + 1; i++) {
                try {
                    br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            try {
                return br.readLine();
            } catch (IOException e) {
                e.printStackTrace();
                return "";
            }
        }
        else if (kol > 0 && kol < 4) {
            for(int i = 0; i < new Random().nextInt(3) + 4; i++) {
                try {
                    br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            try {
                return br.readLine();
            } catch (IOException e) {
                e.printStackTrace();
                return "";
            }
        }
        else if (kol > 3 && kol < 8) {
            for(int i = 0; i < new Random().nextInt(3) + 8; i++) {
                try {
                    br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            try {
                return br.readLine();
            } catch (IOException e) {
                e.printStackTrace();
                return "";
            }
        }
        else if (kol > 7 && kol < 11) {
            for(int i = 0; i < new Random().nextInt(2) + 12; i++) {
                try {
                    br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            try {
                return br.readLine();
            } catch (IOException e) {
                e.printStackTrace();
                return "";
            }
        }
        return "";
    }

    public boolean QuizContains (QuizQuestion[] arr, QuizQuestion targVal, int isnull) { // Определяет входит ли этот вопрос в массив вопросов Викторины и Угадай термин
        for (int i = 0; i < isnull; i++) {
            if (arr[i].getQuizNum() == targVal.getQuizNum()) {
                return true;
            }
        }
        return false;
    }

    public boolean PicContains (PicQuestion[] arr, PicQuestion targVal, int isnull) { // Определяет входит ли этот вопрос в массив вопросов Угадай по картинке
        for (int i = 0; i < isnull; i++) {
            if (arr[i].getQuestion().equals(targVal.getQuestion()) &
                    arr[i].getAnswers()[0].equals(targVal.getAnswers()[0]) &
                    arr[i].getAnswers()[1].equals(targVal.getAnswers()[1]) &
                    arr[i].getAnswers()[2].equals(targVal.getAnswers()[2]) &
                    (arr[i].getRightAnsw().equals(targVal.getRightAnsw()))) {
                return true;
            }
        }
        return false;
    }

    public boolean TrFlsContains (TrFlsQuestion[] arr, TrFlsQuestion targVal, int isnull) { // Определяет входит ли этот вопрос в массив вопросов Правда/Ложь
        for (int i = 0; i < isnull; i++) {
            if (arr[i].getQuizNum() == targVal.getQuizNum()) {
                return true;
            }
        }
        return false;
    }

    public Drawable getNewPic(int num, Context context) { // Получение картинки нужного вопроса  Угадай по картинке
        switch (num) {
            case 1: return context.getResources().getDrawable(R.drawable.quest1);
            case 2: return context.getResources().getDrawable(R.drawable.quest2);
            case 3: return context.getResources().getDrawable(R.drawable.quest3);
            case 4: return context.getResources().getDrawable(R.drawable.quest4);
            case 5: return context.getResources().getDrawable(R.drawable.quest5);
            case 6: return context.getResources().getDrawable(R.drawable.quest6);
            case 7: return context.getResources().getDrawable(R.drawable.quest7);
            case 8: return context.getResources().getDrawable(R.drawable.quest8);
            case 9: return context.getResources().getDrawable(R.drawable.quest9);
            case 10: return context.getResources().getDrawable(R.drawable.quest10);
            case 11: return context.getResources().getDrawable(R.drawable.quest11);
            case 12: return context.getResources().getDrawable(R.drawable.quest12);
            case 13: return context.getResources().getDrawable(R.drawable.quest13);
            case 14: return context.getResources().getDrawable(R.drawable.quest14);
            case 15: return context.getResources().getDrawable(R.drawable.quest15);
            case 16: return context.getResources().getDrawable(R.drawable.quest16);
            case 17: return context.getResources().getDrawable(R.drawable.quest17);
            case 18: return context.getResources().getDrawable(R.drawable.quest18);
            case 19: return context.getResources().getDrawable(R.drawable.quest19);
            case 20: return context.getResources().getDrawable(R.drawable.quest20);
            case 21: return context.getResources().getDrawable(R.drawable.quest21);
            case 22: return context.getResources().getDrawable(R.drawable.quest22);
            case 23: return context.getResources().getDrawable(R.drawable.quest23);
            case 24: return context.getResources().getDrawable(R.drawable.quest24);
            case 25: return context.getResources().getDrawable(R.drawable.quest25);
            case 26: return context.getResources().getDrawable(R.drawable.quest26);
            case 27: return context.getResources().getDrawable(R.drawable.quest27);
            case 28: return context.getResources().getDrawable(R.drawable.quest28);
            case 29: return context.getResources().getDrawable(R.drawable.quest29);
            case 30: return context.getResources().getDrawable(R.drawable.quest30);
        }
        return null;
    }
}
