package com.maiml.ffmpegdemo2;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.maiml.ffmpeglibrary.FFmpeg;

public class AvcodecTxtActivity extends AppCompatActivity {

    private TextView mTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_avcodec_txt);

        mTextView = (TextView) findViewById(R.id.tv);

        mTextView.setText(FFmpeg.getsInstance().getValue());

    }
}
