// Класс вопроса на викторине и "Угадай термин"

package com.enoti.enotiquiz;

public class QuizQuestion {
    private short quizNum = 0;
    private String question = "Вопрос";
    private String[] answers = {"Ответ1", "Ответ2", "Ответ3"};
    private String rightAnsw = "Ответ1";

    public QuizQuestion (short nQuizNum, String nQuestion, String[] nAnswers, String nRightAnsw) {
        quizNum = nQuizNum;
        question = nQuestion;
        answers = nAnswers;
        rightAnsw = nRightAnsw;
    }

    public short getQuizNum () {
        return quizNum;
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
}
