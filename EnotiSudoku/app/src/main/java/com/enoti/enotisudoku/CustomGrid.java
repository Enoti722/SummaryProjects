package com.enoti.enotisudoku;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.GridView;
import java.util.Arrays;

public class CustomGrid extends BaseAdapter {
    Context mContext;
    String[][] mItems = new String[9][9];

    CustomGrid (Context context, String[][] strings) {
        mContext = context;
        mItems = strings;
    }

    @Override
    public int getCount() {
        // TODO Auto-generated method stub
        return mItems.length;
    }

    @Override
    public Object getItem(int position) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public long getItemId(int position) {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        // TODO Auto-generated method stub
        View grid;
        LayoutInflater inflater = (LayoutInflater) mContext
                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        if (convertView == null) {
            ArrayAdapter<String> adapter  =  new ArrayAdapter<String>(mContext, android.R.layout.simple_list_item_1, Arrays.asList(mItems[position]));
            grid = new View(mContext);
            grid = inflater.inflate(R.layout.grid_item, null);
            GridView g = grid.findViewById(R.id.Grid);
            g.setAdapter(adapter);
        } else {
            grid = (View) convertView;
        }

        return grid;
    }
}
