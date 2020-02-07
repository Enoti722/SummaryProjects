// Класс вопроса на "Угадай по картинке"

package com.enoti.enotiquiz;

import android.graphics.drawable.Drawable;

public class PicQuestion {
    private String question = "Вопрос";
    private String[] answers = {"Ответ1", "Ответ2", "Ответ3"};
    private String rightAnsw = "Ответ1";
    private Drawable pic;

    public PicQuestion (String nQuestion, String[] nAnswers, String nRightAnsw, Drawable nPic) {
        question = nQuestion;
        answers = nAnswers;
        rightAnsw = nRightAnsw;
        pic = nPic;
    }

    public String getQuestion() {
        return question;
    }

    public String[] getAnswers() {
        return answers;
    }

    public String getRightAnsw() {
        return rightAnsw;
    }

    public Drawable getPic() {
        return pic;
    }
}
