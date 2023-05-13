package com.byteflow.learnffmpeg;

import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;
import android.widget.SeekBar;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import com.byteflow.learnffmpeg.media.FFMediaPlayer;
import com.byteflow.learnffmpeg.media.MySurfaceView;

import static com.byteflow.learnffmpeg.media.FFMediaPlayer.MEDIA_PARAM_VIDEO_DURATION;
import static com.byteflow.learnffmpeg.media.FFMediaPlayer.MEDIA_PARAM_VIDEO_HEIGHT;
import static com.byteflow.learnffmpeg.media.FFMediaPlayer.MEDIA_PARAM_VIDEO_WIDTH;
import static com.byteflow.learnffmpeg.media.FFMediaPlayer.MSG_DECODER_DONE;
import static com.byteflow.learnffmpeg.media.FFMediaPlayer.MSG_DECODER_INIT_ERROR;
import static com.byteflow.learnffmpeg.media.FFMediaPlayer.MSG_DECODER_READY;
import static com.byteflow.learnffmpeg.media.FFMediaPlayer.MSG_DECODING_TIME;
import static com.byteflow.learnffmpeg.media.FFMediaPlayer.MSG_REQUEST_RENDER;

public class MediaPlayerActivity extends AppCompatActivity implements SurfaceHolder.Callback, FFMediaPlayer.EventCallback{
    private static final String TAG = "MediaPlayerActivity";
    private MySurfaceView mSurfaceView = null;
    private FFMediaPlayer mMediaPlayer = null;
    private SeekBar mSeekBar = null;
    private boolean mIsTouch = false;
    private String mPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/mvtest.mp4";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_media_player);
        mSurfaceView = findViewById(R.id.surface_view);
        mSurfaceView.getHolder().addCallback(this);

        mSeekBar = findViewById(R.id.seek_bar);
        mSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                mIsTouch = true;
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                Log.d(TAG, "onStopTrackingTouch() called with: progress = [" + seekBar.getProgress() + "]");
                if(mMediaPlayer != null) {
                    mMediaPlayer.seekToPosition(mSeekBar.getProgress());
                    mIsTouch = false;
                }

            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        if(mMediaPlayer != null)
            mMediaPlayer.play();
    }

    @Override
    protected void onPause() {
        super.onPause();
        if(mMediaPlayer != null)
            mMediaPlayer.pause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        Log.d(TAG, "surfaceCreated() called with: surfaceHolder = [" + surfaceHolder + "]");
        mMediaPlayer = new FFMediaPlayer();
        mMediaPlayer.addEventCallback(this);
        mMediaPlayer.init(mPath, surfaceHolder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int format, int w, int h) {
        Log.d(TAG, "surfaceChanged() called with: surfaceHolder = [" + surfaceHolder + "], format = [" + format + "], w = [" + w + "], h = [" + h + "]");
        mMediaPlayer.play();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        Log.d(TAG, "surfaceDestroyed() called with: surfaceHolder = [" + surfaceHolder + "]");
        mMediaPlayer.unInit();
    }

    @Override
    public void onPlayerEvent(final int msgType, final float msgValue) {
        Log.d(TAG, "onPlayerEvent() called with: msgType = [" + msgType + "], msgValue = [" + msgValue + "]");
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                switch (msgType) {
                    case MSG_DECODER_INIT_ERROR:
                        break;
                    case MSG_DECODER_READY:
                        onDecoderReady();
                        break;
                    case MSG_DECODER_DONE:
                        break;
                    case MSG_REQUEST_RENDER:
                        break;
                    case MSG_DECODING_TIME:
                        if(!mIsTouch)
                            mSeekBar.setProgress((int) msgValue);
                        break;
                    default:
                        break;
                }
            }
        });

    }

    private void onDecoderReady() {
        int videoWidth = (int) mMediaPlayer.getMediaParams(MEDIA_PARAM_VIDEO_WIDTH);
        int videoHeight = (int) mMediaPlayer.getMediaParams(MEDIA_PARAM_VIDEO_HEIGHT);
        if(videoHeight * videoWidth != 0)
            mSurfaceView.setAspectRatio(videoWidth, videoHeight);

        int duration = (int) mMediaPlayer.getMediaParams(MEDIA_PARAM_VIDEO_DURATION);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            mSeekBar.setMin(0);
        }
        mSeekBar.setMax(duration);
    }

}
