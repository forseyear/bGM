#pragma once

#include "stdafx.hpp"
#include "window.hpp"
#include "args_list.hpp"
#include "bGM_player.hpp"
#include "sound_options.hpp"
#include "tempfile_creator.hpp"
#include "resource.hpp"

class CBgmWindow : public CWindow
{
public:
    CBgmWindow(const std::shared_ptr<CArgsList> &pArgsList);
    virtual ~CBgmWindow();
    void InitPlayer();
    HWND FindTargetWindowHandle(tstring &szHandleName);
    void SetTargetWindowHandle(tstring &szHandleName);
    void PostWindowMessage(DWORD dwMessage, WPARAM wp, LPARAM lp);
    bool SendWindowMessage(DWORD dwMessage, WPARAM wp, LPARAM lp);
    void PassOption(TCHAR &param);
    bool Create(HINSTANCE hInstance, LPCTSTR szClassName, LPCTSTR szWindowName, int nWidth, int nHeight);
    bool Create(HINSTANCE hInstance, LPCTSTR szWindowName);
    LRESULT WindowProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp);
    void FetchGameMakerProcessId();
    int MsgWaitLoop();
public:
    static constexpr DWORD bWM_PAUSE = WM_APP + 1;
    static constexpr DWORD bWM_RESET = WM_APP + 2;
    static constexpr DWORD bWM_STOP = WM_APP + 3;
    static constexpr DWORD bWM_CLOSE = WM_APP + 4;
    static constexpr DWORD bWM_SET_VOLUME = WM_APP + 5;
    static constexpr DWORD bWM_SET_PAN = WM_APP + 6;
    static constexpr DWORD bWM_SET_LOOP = WM_APP + 7;
    static constexpr DWORD bWM_UNSET_LOOP = WM_APP + 8;
    static constexpr DWORD bWM_SET_PITCH = WM_APP + 9;
    static constexpr DWORD bWM_SWITCH_DEBUG = WM_APP + 10;
    static constexpr DWORD bWM_IS_PLAYING = WM_APP + 11;
    static constexpr DWORD bWM_IS_OPENING = WM_APP + 12;
    static constexpr DWORD bWM_SET_REPEAT_COUNT = WM_APP + 13;
    static constexpr DWORD bWM_ADD_REPEAT = WM_APP + 14;
    static constexpr DWORD bWM_REMOVE_REPEAT = WM_APP + 15;
    static constexpr DWORD bWM_LOAD = WM_APP + 16;
    static constexpr DWORD bWM_FADE_IN = WM_APP + 17;
    static constexpr DWORD bWM_FADE_OUT = WM_APP + 18;
    static constexpr DWORD bWM_CHECK_REPEAT = WM_APP + 19;
    static constexpr DWORD bWM_GET_POSITION = WM_APP + 20;
    static constexpr DWORD bWM_SET_POSITION = WM_APP + 21;
    static constexpr DWORD bWM_GET_LENGTH = WM_APP + 22;
private:
    HWND m_targetWindowHandle_;
    std::shared_ptr<CArgsList> m_pArgsList_;
    std::unique_ptr<CBgmPlayer> m_pBgmPlayer_;
    HANDLE m_GM_process_;
    DWORD m_GM_process_id_;
};
