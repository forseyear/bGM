#pragma once

#include "stdafx.hpp"
#include "bGM_console.hpp"
#include "sound_options.hpp"
#include "character_converter.hpp"
#include "tempfile_creator.hpp"
#include "audiere.h"
#pragma comment(lib, "audiere.lib")

#undef max
#undef min

class CBgmPlayer
{
public:
    CBgmPlayer();
    virtual ~CBgmPlayer();
    virtual void init(sound_options_p const &so);
    virtual void stopFade()
    {
        m_need_StopFade_ = true;
    };
    virtual void fadeIn(int fadeFrame, float volume);
    virtual void fadeOut(int fadeFrame, float volume);
    virtual void executeFadeIn(std::chrono::milliseconds interval, float fadedVolume);
    virtual void executeFadeOut(std::chrono::milliseconds interval, float fadedVolume);
    virtual void firstPlay();
    virtual void play();
    virtual void pause();
    virtual void reset();
    virtual void stop();
    virtual void setVolume(float volume);
    virtual void setPan(float pan);
    virtual void setPitch(float pitch);
    virtual void addRepeat(int repeatPositionA, int repeatPositionB);
    virtual void setRepeatCount(int repeatCount);
    virtual void removeRepeat();
    virtual void checkRepeat();
    virtual void setLoop(bool isLooping);
    virtual void setPosition(int position);
    virtual bool getisLooping() const;
    virtual void writePosition();
    virtual void writeLength();
    virtual void printSoundOptions();
    virtual int soundRange(const int &number);
    virtual float volumeRange(const float &volume);
    virtual float panRange(const float &pan);
    virtual float pitchRange(const float &pitch);
    template <typename T>
    T maxmin(const T &max, const T &min, const T &number);
private:
    bool m_need_StopFade_;
    sound_options_p m_sound_options_;
    int m_fade_count_ = 20;
    int m_repeat_positionA_;
    int m_repeat_positionB_;
    int m_sound_start_;
    int m_sound_end_;
    audiere::AudioDevicePtr m_audio_device_;
    audiere::SampleSourcePtr m_sample_source_;
    audiere::OutputStreamPtr m_output_stream_;
    audiere::LoopPointSourcePtr m_loop_point_;
    std::unique_ptr<CBgmConsole> m_pBgmConsole_;
    CTempFileCreator tempcreater;
};
