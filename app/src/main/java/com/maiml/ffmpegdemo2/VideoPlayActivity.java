package com.maiml.ffmpegdemo2;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.maiml.ffmpeglibrary.FFmpeg;

public class VideoPlayActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;
    private String mPath;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video_play);

        mSurfaceView = (SurfaceView) findViewById(R.id.surface_view);

        mSurfaceHolder = mSurfaceView.getHolder();
        mSurfaceHolder.addCallback(this);

        mPath = "/storage/emulated/0/tencent/MicroMsg/WeiXin/wx_camera_1491139171727.mp4";
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
//        FFmpeg.getsInstance().play(mSurfaceHolder.getSurface(),mPath);
        new Thread(new Runnable() {
            @Override
            public void run() {
                FFmpeg.getsInstance().play(mSurfaceHolder.getSurface(),mPath);
            }
        }).start();

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }
}
