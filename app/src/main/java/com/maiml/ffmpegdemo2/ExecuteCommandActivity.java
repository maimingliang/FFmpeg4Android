package com.maiml.ffmpegdemo2;

import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.VideoView;

import com.bumptech.glide.Glide;
import com.maiml.ffmpeglibrary.FFmpeg;

import static android.os.SystemClock.currentThreadTimeMillis;

public class ExecuteCommandActivity extends AppCompatActivity {

    private VideoView mVideoView;
    private Button mCrop;
    private ImageView mGif;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_execute_command);

        mVideoView = (VideoView) findViewById(R.id.video_view);
        mCrop = (Button) findViewById(R.id.cropBtn);
        mGif = (ImageView) findViewById(R.id.image);


        mCrop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                crop();
            }
        });
        init();
    }

    /**
     * 截取视频的前30帧 转化为gif
     * targetSdkVersion >=23 注意文件的读写权限，这里为了方便 targetSdkVersion = 22
     * 更多命令，可以看FFmpeg基础命令系列 http://www.jianshu.com/c/dc36e88ce0a6
     */
    private void crop() {

        final String[] commands = new String[]{
            "ffmpeg",
            "-y",
            "-i","/storage/emulated/0/tencent/MicroMsg/WeiXin/wx_camera_1491139171727.mp4",
              "-vframes","30","-y",
             "-f","gif",
             "/sdcard/crop_30.gif"

        };

        new Thread(new Runnable() {
            @Override
            public void run() {

                final long startTime = currentThreadTimeMillis();
                FFmpeg.getsInstance().executeCommand(commands);
                final long endTime = SystemClock.currentThreadTimeMillis();

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                       Log.e("tag", "使用时间："+(endTime - startTime)+"毫秒");
                        Glide.with(ExecuteCommandActivity.this).load("/sdcard/crop_30.gif").asGif().into(mGif);
                        mCrop.setEnabled(false);
                    }
                });
            }
        }).start();
    }

    private void init() {

        String path =  "/storage/emulated/0/tencent/MicroMsg/WeiXin/wx_camera_1491139171727.mp4";

        mVideoView.setVideoPath(path);
        mVideoView.start();
        mVideoView.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mp, int what, int extra) {
                Log.e("tag","---- what = " + what);
                return false;
            }
        });

        mVideoView.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                mVideoView.start();
                mp.setLooping(true);
            }
        });


    }
}
