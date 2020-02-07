// Класс вопроса на "Правда/ложь"

package com.enoti.enotiquiz;

public class TrFlsQuestion {
    private short quizNum = 0;
    private String question = "Вопрос";
    private byte rightAnsw = 0;

    public TrFlsQuestion (short nQuizNum, String nQuestion, byte nRightAnsw) {
        quizNum = nQuizNum;
        question = nQuestion;
        rightAnsw = nRightAnsw;
    }

    public short getQuizNum() {
        return quizNum;
    }

    public String getQuestion() {
        return question;
    }

    public byte getRightAnsw() {
        return rightAnsw;
    }
}
