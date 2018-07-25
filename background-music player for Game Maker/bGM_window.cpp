#include "bGM_window.hpp"

CBgmWindow::CBgmWindow(const std::shared_ptr<CArgsList> &pArgsList) : m_pArgsList_(pArgsList)
{
    
    if (m_pArgsList_->hasTargetWindowClassName())
    {
        tstring targetWindowClassName = m_pArgsList_->getTargetWindowClassName();

        SetTargetWindowHandle(targetWindowClassName);
        m_pArgsList_->removeTargetWindowClassName();

        std::vector<TCHAR> args_list = m_pArgsList_->getArgumentList();
        for (TCHAR arg : args_list)
        {
            PassOption(arg);
        }
    }
}

CBgmWindow::~CBgmWindow()
{
}

void CBgmWindow::InitPlayer()
{
    sound_options_p so = {
        m_pArgsList_->getFileName(),
        m_pArgsList_->getVolume(),
        m_pArgsList_->getPan(),
        m_pArgsList_->getPitch(),
        false, // isPlaying
        m_pArgsList_->getIsLooping(),
        m_pArgsList_->getIsDebuging(),
        m_pArgsList_->getIsLoadOnly(),
    };

    m_pBgmPlayer_.reset(new CBgmPlayer());
    m_pBgmPlayer_->init(so);
}

HWND CBgmWindow::FindTargetWindowHandle(tstring &szHandleName)
{
    HWND window = ::FindWindow(_T("wnd_class_bGM_hidden"), (LPTSTR)szHandleName.data());
    if (!window)
    {
        tcerr << std::endl << _T("Target ") << szHandleName.data() << _T(" not found.") << std::endl << std::endl;
        exit(-1);
    }
    return window;
}

void CBgmWindow::SetTargetWindowHandle(tstring &szHandleName)
{
    m_targetWindowHandle_ = FindTargetWindowHandle(szHandleName);
}

void CBgmWindow::PostWindowMessage(DWORD dwMessage, WPARAM wp, LPARAM lp)
{
    if (m_targetWindowHandle_)
    {
        ::PostMessage(m_targetWindowHandle_, dwMessage, wp, lp);
    }
}

bool CBgmWindow::SendWindowMessage(DWORD dwMessage, WPARAM wp, LPARAM lp)
{
    if (m_targetWindowHandle_)
    {
        ::SendMessage(m_targetWindowHandle_, dwMessage, wp, lp);
        return true;
    }
    else
    {
        return false;
    }
}

void CBgmWindow::PassOption(TCHAR &param)
{
    CTempFileCreator tempcreater;
    switch (param)
    {
    case _T('v'):
        PostWindowMessage(bWM_SET_VOLUME, 0, (LPARAM)m_pArgsList_->getVolume());
        break;
    case _T('p'):
        PostWindowMessage(bWM_SET_PAN, 0, (LPARAM)m_pArgsList_->getPan());
        break;
    case _T('s'):
        PostWindowMessage(bWM_SET_PITCH, 0, (LPARAM)m_pArgsList_->getPitch());
        break;
    case _T('L'):
        PostWindowMessage(bWM_SET_LOOP, 0, 0);
        break;
    case _T('o'):
        PostWindowMessage(bWM_PAUSE, 0, 0);
        break;
    case _T('r'):
        PostWindowMessage(bWM_RESET, 0, 0);
        break;
    case _T('m'):
        PostWindowMessage(bWM_STOP, 0, 0);
        break;
    case _T('d'):
        PostWindowMessage(bWM_CLOSE, 0, 0);
        break;
    case _T('u'):
        PostWindowMessage(bWM_UNSET_LOOP, 0, 0);
        break;
    case _T('n'):
        if (!SendWindowMessage(bWM_IS_OPENING, 0, 0))
        {
            tempcreater.Create((tstring)_T("isOpenTemp"), false);
        }
        break;
    case _T('y'):
        if (!SendWindowMessage(bWM_IS_PLAYING, 0, 0))
        {
            tempcreater.Create((tstring)_T("isPlayTemp"), false);
        }
        break;
    case _T('g'):
        PostWindowMessage(bWM_SET_REPEAT_COUNT, 0, (WPARAM)m_pArgsList_->getRepeatCount());
    case _T('a'):
        PostWindowMessage(bWM_ADD_REPEAT, (WPARAM)m_pArgsList_->getRepeatPointA(), (LPARAM)m_pArgsList_->getRepeatPointB());
        break;
    case _T('b'):
        break;
    case _T('x'):
        PostWindowMessage(bWM_REMOVE_REPEAT, 0, 0);
        break;
    case _T('V'):
        break;
    case _T('h'):
        PostWindowMessage(bWM_FADE_IN, (WPARAM)m_pArgsList_->getFadeInFrame(), (LPARAM)m_pArgsList_->getFadedVolume());
        break;
    case _T('q'):
        PostWindowMessage(bWM_FADE_OUT, (WPARAM)m_pArgsList_->getFadeOutFrame(), (LPARAM)m_pArgsList_->getFadedVolume());
        break;
    case _T('c'):
        PostWindowMessage(bWM_CHECK_REPEAT, 0, 0);
        break;
    case _T('j'):
        SendWindowMessage(bWM_GET_POSITION, 0, 0);
        break;
    case _T('k'):
        PostWindowMessage(bWM_SET_POSITION, 0, (LPARAM)m_pArgsList_->getMovedPosition());
        break;
    case _T('l'):
        SendWindowMessage(bWM_GET_LENGTH, 0, 0);
        break;
    default:
        break;
    }
}

bool CBgmWindow::Create(HINSTANCE hInstance, LPCTSTR szClassName, LPCTSTR szWindowName, int nWidth, int nHeight)
{
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.lpfnWndProc = StaticWindowProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = szClassName;
    wcex.lpszMenuName = nullptr;
    wcex.style = CS_VREDRAW | CS_HREDRAW;
    wcex.hCursor = ::LoadCursor(hInstance, IDC_ARROW);
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hIconSm = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hbrBackground = (HBRUSH)::GetStockObject(LTGRAY_BRUSH);

    if (!::RegisterClassEx(&wcex))
    {
        return false;
    }

    hWnd = ::CreateWindow(
        szClassName,
        szWindowName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nWidth,
        nHeight,
        nullptr,
        nullptr,
        hInstance,
        this
    );
    if (hWnd == nullptr)
    {
        return false;
    }

    ShowWindow(SW_HIDE);
    UpdateWindow();

    return true;
}

bool CBgmWindow::Create(HINSTANCE hInstance, LPCTSTR szWindowName)
{
    return Create(hInstance, _T("wnd_class_bGM_hidden"), szWindowName, 280, 280);
}

LRESULT CBgmWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp)
{
    CTempFileCreator tempcreater;
    switch (uMsg)
    {
    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;
 
    case bWM_PAUSE:
        m_pBgmPlayer_->pause();
        break;

    case bWM_RESET:
        m_pBgmPlayer_->reset();
        break;

    case bWM_STOP:
        m_pBgmPlayer_->stop();
        break;

    case bWM_CLOSE:
        m_pBgmPlayer_->stop();
        DestroyWindow(hWnd);
        break;

    case bWM_SET_VOLUME:
        m_pBgmPlayer_->setVolume((float)lp);
        break;

    case bWM_SET_PAN:;
        m_pBgmPlayer_->setPan((float)lp);
        break;

    case bWM_SET_LOOP:
        m_pBgmPlayer_->setLoop(true);
        break;

    case bWM_UNSET_LOOP:
        m_pBgmPlayer_->setLoop(false);
        break;

    case bWM_SET_PITCH:
        m_pBgmPlayer_->setPitch((float)lp);
        break;

    case bWM_SWITCH_DEBUG:
        break;

    case bWM_IS_PLAYING:
        tempcreater.Create((tstring)_T("isPlayTemp"), m_pBgmPlayer_->getisLooping());
        break;

    case bWM_IS_OPENING:
        tempcreater.Create((tstring)_T("isOpenTemp"), true);
        break;

    case bWM_ADD_REPEAT:
        m_pBgmPlayer_->addRepeat((int)wp, (int)lp);
        break;

    case bWM_SET_REPEAT_COUNT:
        m_pBgmPlayer_->setRepeatCount((int)lp);
        break;

    case bWM_REMOVE_REPEAT:
        m_pBgmPlayer_->removeRepeat();
        break;

    case bWM_LOAD:
        break;

    case bWM_FADE_IN:
        m_pBgmPlayer_->fadeIn((int)wp, (float)lp);
        break;

    case bWM_FADE_OUT:
        m_pBgmPlayer_->fadeOut((int)wp, (float)lp);
        break;

    case bWM_CHECK_REPEAT:
        m_pBgmPlayer_->checkRepeat();
        break;

    case bWM_GET_POSITION:
        m_pBgmPlayer_->writePosition();
        break;

    case bWM_SET_POSITION:
        m_pBgmPlayer_->setPosition((int)lp);
        break;

    case bWM_GET_LENGTH:
        m_pBgmPlayer_->writeLength();
        break;

    default:
        return ::DefWindowProc(hWnd, uMsg, wp, lp);
    }
    return 0;
}

void CBgmWindow::FetchGameMakerProcessId()
{
    long GM_handle_number = m_pArgsList_->getLongGameMakerWindowHWND();
    ::GetWindowThreadProcessId((HWND)GM_handle_number, &m_GM_process_id_);

    if (!m_GM_process_id_)
    {
        tcerr << std::endl << _T("Window (") << std::showbase << std::hex << GM_handle_number << _T(") not found.") << std::endl << std::endl;
        exit(-1);
    }
}

int CBgmWindow::MsgWaitLoop()
{
    MSG msg;
    this->FetchGameMakerProcessId();
    m_GM_process_ = ::OpenProcess(PROCESS_QUERY_INFORMATION | SYNCHRONIZE, FALSE, m_GM_process_id_);

    while (::MsgWaitForMultipleObjects(1, &m_GM_process_, FALSE, INFINITE, QS_POSTMESSAGE | QS_SENDMESSAGE) == WAIT_OBJECT_0 + 1)
    {
        while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                ::CloseHandle(m_GM_process_);
                return msg.wParam;
            }
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }
    return 0;
}
