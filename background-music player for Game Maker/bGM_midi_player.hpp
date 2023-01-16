#pragma once

#include "stdafx.hpp"
#include "bGM_player.hpp"
#include "bGM_console.hpp"
#include "sound_options.hpp"
#include "character_converter.hpp"
#include "tempfile_creator.hpp"
#include "audiere.h"
#pragma comment(lib, "audiere.lib")

#undef max
#undef min

class CBgmidiPlayer : public CBgmPlayer
{
public:
    CBgmidiPlayer();
    virtual ~CBgmidiPlayer();
    void init(sound_options_p const &so) override;
    void stopFade() override
    {
    }
    void fadeIn(int fadeFrame, float volume) override
    {
    }
    void fadeOut(int fadeFrame, float volume) override
    {
    }
    void executeFadeIn(std::chrono::milliseconds interval, float fadedVolume) override
    {
    }
    void executeFadeOut(std::chrono::milliseconds interval, float fadedVolume) override
    {
    }
    void firstPlay() override;
    void play() override;
    void pause() override;
    void reset() override;
    void stop() override;
    void setVolume(float volume) override
    {
    }
    void setPan(float pan) override
    {
    }
    void setPitch(float pitch) override
    {
    }
    void addRepeat(int repeatPositionA, int repeatPositionB) override
    {
    }
    void setRepeatCount(int repeatCount) override
    {
    }
    void removeRepeat() override
    {
    }
    void checkRepeat() override
    {
    }
    void setLoop(bool isLooping) override;
    void setPosition(int position) override;
    bool getisLooping() const override;
    void writePosition() override;
    void writeLength() override;
    void printSoundOptions() override;
    int soundRange(const int &number) override;
private:
    sound_options_p m_sound_options_;
    int m_sound_start_;
    int m_sound_end_;
    audiere::MIDIDevicePtr m_midi_device_;
    audiere::MIDIStreamPtr m_midi_stream_;
    std::unique_ptr<CBgmConsole> m_pBgmConsole_;
    CTempFileCreator tempcreater;
};
