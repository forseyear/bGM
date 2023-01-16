#include "stdafx.hpp"
#include "args_list.hpp"
#include "bGM_window.hpp"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    std::shared_ptr<CArgsList> pArgsList(new CArgsList(__argc, __targv));
    std::unique_ptr<CBgmWindow> pBgmWindow(new CBgmWindow(pArgsList));

    if (pArgsList->hasMyWindowClassName())
    {
        tstring szWindowName = pArgsList->getMyWindowClassName();
        pBgmWindow->Create(hInstance, szWindowName.data());
        pBgmWindow->InitPlayer();
        pBgmWindow->MsgWaitLoop();
    }

    return 0;
}
