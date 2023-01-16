#include "window.hpp"

CWindow::CWindow()
{
}

CWindow::~CWindow()
{
}

bool CWindow::Create(HINSTANCE hInstance, TCHAR* szClassName, TCHAR* szAppName, int nWidth, int nHeight)
{
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.lpfnWndProc = CWindow::StaticWindowProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = szClassName;
    wcex.lpszMenuName = nullptr;
    wcex.style = CS_VREDRAW | CS_HREDRAW;
    wcex.hCursor = ::LoadCursor(hInstance, IDC_ARROW);
    wcex.hIcon = ::LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hIconSm = ::LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);

    if (!::RegisterClassEx(&wcex))
    {
        return false;
    }

    this->hWnd = ::CreateWindow(
        szClassName,
        szAppName,
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
    if (this->hWnd == nullptr)
    {
        return false;
    }
    return true;
}

bool CWindow::IsCreated()
{
    if (hWnd == nullptr)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool CWindow::UpdateWindow()
{
    if (hWnd == nullptr)
    {
        return false;
    }
    ::UpdateWindow(hWnd);
    return true;
}

bool CWindow::ShowWindow(int nCmdShow)
{
    if (hWnd == nullptr)
    {
        return false;
    }
    ::ShowWindow(hWnd, nCmdShow);
    return true;
}

void CWindow::SetTitle(TCHAR* szTitle)
{
    if (hWnd != nullptr)
    {
        ::SetWindowText(hWnd, szTitle);
    }
}

LRESULT CWindow::WindowProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
{
    switch (Msg)
    {
    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;
    default:
        return ::DefWindowProc(hWnd, Msg, wp, lp);
    }
    return 0;
}

LRESULT CALLBACK CWindow::StaticWindowProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
{
    CWindow *pWindow = (CWindow*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (pWindow == nullptr)
    {
        if (Msg == WM_CREATE)
        {
            pWindow = (CWindow*)((LPCREATESTRUCT)lp)->lpCreateParams;
        }
    }
    if (pWindow != nullptr)
    {
        pWindow->SetPointer(hWnd);
        return pWindow->WindowProc(hWnd, Msg, wp, lp);
    } else {
        return ::DefWindowProc(hWnd, Msg, wp, lp);
    }
}

void CWindow::SetPointer(HWND hWnd)
{
    ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    hWnd = hWnd;
}
