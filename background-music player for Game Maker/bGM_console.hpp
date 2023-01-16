#pragma once

#include "stdafx.hpp"
#include "console.hpp"
#include "timer.hpp"

class CBgmConsole : public CConsole
{
public:
    CBgmConsole(bool shouldPause);
    virtual ~CBgmConsole();
    void Init();
    void DoneOpenDevice();
    void FailedOpenDevice();
    void DoneLoadStream();
    void FailedLoadStream(const tstring &szDeviceName);
    void DoneStartPlayback(const tstring &szDeviceName, const tstring &szFileName);
    void FailedStartPlayback();
    void UpdateStatus(bool shouldLoop, bool isPausing, float fVolume, float fPan, float fPitch);
    void UpdateMIDIStatus(bool shouldLoop, bool isPausing);
    void Exit();
private:
    static BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);
    bool m_should_pause_;
    Timer openDeviceTimer;
    Timer loadStreamTimer;
    Timer startPlaybackTimer;
    double openDeviceTime;
    double loadStreamTime;
    double startPlaybackTime;
};
