#pragma once

#include "stdafx.hpp"

class CWindow
{
public:
    CWindow();
    virtual ~CWindow();
    virtual bool Create(HINSTANCE hInstance, TCHAR* ptrClassName, TCHAR* ptrAppName, int width, int height);
    bool IsCreated();
    bool UpdateWindow();
    bool ShowWindow(int nCmdShow);
    void SetTitle(TCHAR* ptrTitle);
    virtual LRESULT WindowProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp);
protected:
    static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp);
    volatile HWND hWnd = nullptr;
    void SetPointer(HWND hWnd);
};
