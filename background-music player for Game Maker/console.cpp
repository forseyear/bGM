#include "console.hpp"

CConsole::CConsole()
{

}

void CConsole::Create()
{
    ::AllocConsole();
    _tfreopen_s(&fp, _T("CONIN$"), _T("r"), stdin);
    _tfreopen_s(&fp, _T("CONOUT$"), _T("w"), stdout);
    _tfreopen_s(&fp, _T("CONOUT$"), _T("w"), stderr);
    tcout.imbue(std::locale("Japanese"));
}

CConsole::~CConsole()
{
    fclose(fp);
    ::FreeConsole();
}
