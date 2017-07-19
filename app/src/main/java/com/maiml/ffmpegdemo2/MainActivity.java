package com.maiml.ffmpegdemo2;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.maiml.ffmpeglibrary.FFmpeg;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    // Example of a call to a native method
    final TextView tv = (TextView) findViewById(R.id.sample_text);

        findViewById(R.id.btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                tv.setText(FFmpeg.getsInstance().getValue());
            }
        });


    }


}
