package com.von.ffmpeg.demo

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.von.ffmpeg.demo.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = FFmpegNative.getFFmpegVersion()
    }
}