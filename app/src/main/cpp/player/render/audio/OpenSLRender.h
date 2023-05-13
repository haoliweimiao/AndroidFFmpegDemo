//
// Created by Von on 2023/5/13.
//

#ifndef FFMPEGDEMO_OPENSLRENDER_H
#define FFMPEGDEMO_OPENSLRENDER_H

#include <cstdint>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <queue>
#include <string>
#include <thread>
#include "AudioRender.h"

class OpenSLRender : public AudioRender {
public:
    OpenSLRender() {}

    virtual ~OpenSLRender() {}

    virtual void Init();

    virtual void RenderAudioFrame(uint8_t *pData, int dataSize);

    virtual void UnInit();

private:
    int CreateEngine();

    int CreateOutputMixer();

    int CreateAudioPlayer();

    void StartRender();

    void HandleAudioFrameQueue();

    static void CreateSLWaitingThread(OpenSLRender *openSlRender);

    static void AudioPlayerCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context);

    SLObjectItf m_EngineObj = nullptr;
    SLEngineItf m_EngineEngine = nullptr;
    SLObjectItf m_OutputMixObj = nullptr;
    SLObjectItf m_AudioPlayerObj = nullptr;
    SLPlayItf m_AudioPlayerPlay = nullptr;
    SLVolumeItf m_AudioPlayerVolume = nullptr;
    SLAndroidSimpleBufferQueueItf m_BufferQueue;

    std::queue<AudioFrame *> m_AudioFrameQueue;

    std::thread *m_thread = nullptr;
    std::mutex m_Mutex;
    std::condition_variable m_Cond;
    volatile bool m_Exit = false;

};

#endif //FFMPEGDEMO_OPENSLRENDER_H
