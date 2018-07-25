#pragma once

#include "stdafx.hpp"
#include "bGM_console.hpp"
#include "sound_options.hpp"
#include "character_converter.hpp"
#include "tempfile_creator.hpp"
#include "audiere.h"
#pragma comment(lib, "audiere.lib")

class CBgmPlayer
{
public:
    CBgmPlayer();
    virtual ~CBgmPlayer();
    void init(sound_options_p const &so);
    void stopFade() { m_need_StopFade_ = true; };
    void fadeIn(int fadeFrame, float fadedVolume);
    void fadeOut(int fadeFrame, float fadedVolume);
    void executeFadeIn(std::chrono::milliseconds interval, float fadedVolume);
    void executeFadeOut(std::chrono::milliseconds interval, float fadedVolume);
    void firstPlay();
    void play();
    void pause();
    void reset();
    void stop();
    void setVolume(float volume);
    void setPan(float pan);
    void setPitch(float pitch);
    void addRepeat(int repeatPositionA, int repeatPositionB);
    void setRepeatCount(int repeatCount);
    void removeRepeat();
    void checkRepeat();
    void setLoop(bool isLooping);
    void setPosition(int position);
    bool getisLooping() const;
    void writePosition();
    void writeLength();
    void printSoundOptions();
private:
    bool m_need_StopFade_;
    sound_options_p m_sound_options_;
    int m_fade_count = 20;
    int m_repeat_positionA;
    int m_repeat_positionB;
    audiere::AudioDevicePtr m_audio_device_;
    audiere::SampleSourcePtr m_sample_source_;
    audiere::OutputStreamPtr m_output_stream_;
    audiere::LoopPointSourcePtr m_loop_point_;
    std::unique_ptr<CBgmConsole> m_pBgmConsole_;
    CTempFileCreator tempcreater;
};
