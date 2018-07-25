#include "bGM_console.hpp"

CBgmConsole::CBgmConsole(bool shouldPause)
{
    m_should_pause_ = shouldPause;
}

CBgmConsole::~CBgmConsole()
{
}

void CBgmConsole::Init()
{
    ::SetConsoleCtrlHandler(HandlerRoutine, TRUE);
    tcout << _T("+++++ Welcome to bGM Debug Console ;-) +++++") << std::endl;
    tcout << _T("Command: ") << ::GetCommandLine() << std::endl << std::endl;

    openDeviceTimer.Restart();
    tcout << _T("Opening sound device... ");
}

void CBgmConsole::DoneOpenDevice()
{
    openDeviceTime = openDeviceTimer.Elapsed();
    tcout << _T("done (took: ") << openDeviceTime << _T("sec)") << std::endl;

    loadStreamTimer.Restart();
    tcout << _T("Loading stream... ");
}

void CBgmConsole::FailedOpenDevice()
{
    tcout << std::endl << _T("Cannot open audiere device") << std::endl;
    Exit();
}

void CBgmConsole::DoneLoadStream()
{
    loadStreamTime = loadStreamTimer.Elapsed();
    tcout << _T("done (took: ") << loadStreamTime << _T("sec)") << std::endl;

    startPlaybackTimer.Restart();
    tcout << _T("Starting playback... ");
}

void CBgmConsole::FailedLoadStream(const tstring &szDeviceName)
{
    tcout << std::endl << _T("Cannot load file \"") << szDeviceName << _T("\"") << std::endl;
    Exit();
}

void CBgmConsole::DoneStartPlayback(const tstring &szDeviceName, const tstring &szFileName)
{
    startPlaybackTime = startPlaybackTimer.Elapsed();
    tcout << _T("done (took: ") << startPlaybackTime << _T("sec)") << std::endl;
    tcout << _T("(Total time taken: ") << (double)openDeviceTime + loadStreamTime + startPlaybackTime << _T("sec)") << std::endl << std::endl;
    tcout << _T("Device Name: ") << szDeviceName << std::endl;
    tcout << _T("Now Playing: ") << szFileName << std::endl << std::endl;
    tcout << _T("Status:") << std::endl;
}

void CBgmConsole::FailedStartPlayback()
{
    tcout << _T("failed!") << std::endl;
    Exit();
}

void CBgmConsole::UpdateStatus(bool shouldLoop, bool isPausing, float fVolume, float fPan, float fPitch)
{
    tcout << _T("\r<Loop: ");
    if (shouldLoop == true) tcout << _T("yes, ");
    else tcout << _T(" no, ");

    tcout << _T("Paused: ");
    if (isPausing == true) tcout << _T("yes, ");
    else tcout << _T(" no, ");

    tcout << _T("Volume: ") << std::showpos << std::showpoint << fVolume << _T(", ");
    tcout << _T("Pan: ") << std::showpos << std::showpoint << fPan << _T(", ");
    tcout << _T("Pitch: ") << std::showpos << std::showpoint << fPitch << _T(">");
}

void CBgmConsole::Exit()
{
    if (m_should_pause_)
    {
        system("pause");
    }
    exit(-1);
}

BOOL WINAPI CBgmConsole::HandlerRoutine(DWORD dwCtrlType)
{
    switch (dwCtrlType)
    {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
        return TRUE;

    case CTRL_CLOSE_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        return FALSE;
    }

    return FALSE;
}
