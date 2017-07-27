package com.maiml.ffmpegdemo2;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.maiml.ffmpeglibrary.FFmpeg;

public class AvcodecTxtActivity extends AppCompatActivity {

    private TextView mTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_avcodec_txt);

        mTextView = (TextView) findViewById(R.id.tv);

//        mTextView.setText(FFmpeg.getsInstance().getValue());

//        init();

        findViewById(R.id.contact).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                contact();
            }
        });

        findViewById(R.id.merge).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                merge();
            }
        });
    }

    private void contact() {

        String fileList = "/sdcard/MCut/20170727_095642_fileList.txt";
        String path = "/sdcard/MCut/addddddd.mp4";

        StringBuilder builder = new StringBuilder();

        builder.append("ffmpeg -f concat -safe 0 -i ");
        builder.append(fileList);
        builder.append(" -c copy ");
        //        builder.append("-profile:v baseline -preset ultrafast -b:v 3000k -g 25 ");
        builder.append("-f mp4 ");
        builder.append(path);

        Log.e("tag", "command = " + builder.toString());

        int i = FFmpeg.getsInstance().executeCommand(builder.toString().split(" "));

        Log.e("tag", "----- res = " + i);

    }

    private void merge() {

//        ffmpeg -y -i demo.mp4 -i cc.png -filter_complex overlay demo_overlay1.mp4
        String srcPath = "/sdcard/MCut/20170726_143358.mp4";
        String imgPath = "/sdcard/mind_sa.png";
        String dstPath = "/sdcard/MCut/eeeedd.mp4";

        StringBuilder builder = new StringBuilder();

        builder.append("ffmpeg -y -i ");
        builder.append(srcPath);
        builder.append(" -i ");
        builder.append(imgPath);
        builder.append(" -filter_complex overlay -avoid_negative_ts make_zero ");
        builder.append(dstPath);


        String[] cmds = builder.toString().split(" ");
        Log.e("tag", "  cmds = " +builder.toString());
        int i = FFmpeg.getsInstance().executeCommand(cmds);

        Log.e("tag", "  res = " + i);

    }
}
