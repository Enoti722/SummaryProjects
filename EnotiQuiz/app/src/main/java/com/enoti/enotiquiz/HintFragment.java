// Всплывающее окно "Пропустить вопрос"

package com.enoti.enotiquiz;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

public class HintFragment extends Fragment {

    private TextView text;
    private Button yesBut, noBut;
    private byte mode = 0;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_hint, container, false);
        text = (TextView)view.findViewById(R.id.Text); // Текст вопроса
        yesBut = (Button)view.findViewById(R.id.Yes); // Кнопка Да
        noBut = (Button)view.findViewById(R.id.No); // Кнопка Нет
        text.setText("\nУзнать ответ за 5 монет?\n\nУ вас есть " + getArguments().getString("coins") + " монет");
        if (getArguments().getByte("mode") == 1) {
            mode = 1;
        }
        else if (getArguments().getByte("mode") == 2) {
            mode = 2;
        }
        else {
            mode = 3;
        }
        addListenerOnButton();
        return view;
    }

    public void addListenerOnButton () { // Списание монет и пропуск вопроса через определённую Activity по нажатию кнопки
        yesBut.setOnClickListener(
            new View.OnClickListener(){
                @Override
                public void onClick (View view) {
                    if (mode == 1) {
                        QuizActivity qa = (QuizActivity) getActivity();
                        qa.getHint();
                    }
                    else if (mode == 2) {
                        PicActivity pa = (PicActivity) getActivity();
                        pa.getHint();
                    }
                    else {
                        TrFlsActivity tfa = (TrFlsActivity) getActivity();
                        tfa.getHint();
                    }
                }
            }
        );
        noBut.setOnClickListener( // Отказ от подскащки через определённую Activity по нажатию кнопки
            new View.OnClickListener(){
                @Override
                public void onClick (View view) {
                    if (mode == 1) {
                        QuizActivity qa = (QuizActivity) getActivity();
                        qa.cancelHint();
                    }
                    else if (mode == 2) {
                        PicActivity pa = (PicActivity) getActivity();
                        pa.cancelHint();
                    }
                    else {
                        TrFlsActivity tfa = (TrFlsActivity) getActivity();
                        tfa.cancelHint();
                    }
                }
            }
        );
    }
}
