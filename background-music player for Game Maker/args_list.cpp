#include "args_list.hpp"

CArgsList::CArgsList(int &argc, TCHAR** &argv)
{
    std::vector<tstring> args(argv, argv + argc);
    for (auto iterator = args.begin(); iterator != args.end(); ++iterator)
    {
        tstring arg = *iterator;
        if (arg[0] != _T('-')) continue;

        if (existsOption(arg[1]))
        {
            ++iterator;
            tstring next_arg = *iterator;
            m_args_list_[arg[1]] = next_arg;
        }
        else
        {
            m_args_list_[arg[1]] = tstring(_T("Enabled"));
        }
    }
}

CArgsList::~CArgsList()
{
}

bool CArgsList::hasOption(TCHAR szParam) const
{
    if (m_args_list_.find(szParam) == m_args_list_.end())
    {
        return false;
    }
    return true;
}

int CArgsList::getIntOption(TCHAR szParam) const
{
    if (m_args_list_.find(szParam) == m_args_list_.end())
    {
        return 0;
    }
    return _ttoi(m_args_list_.at(szParam).data());
}

long CArgsList::getLongOption(TCHAR szParam) const
{
    if (m_args_list_.find(szParam) == m_args_list_.end())
    {
        return 0;
    }
    return _ttol(m_args_list_.at(szParam).data());
}

tstring CArgsList::getTstringOption(TCHAR szParam) const
{
    if (m_args_list_.find(szParam) == m_args_list_.end())
    {
        return tstring(_T(""));
    }
    return m_args_list_.at(szParam);
}

float CArgsList::getFloatOption(TCHAR szParam) const
{
    if (m_args_list_.find(szParam) == m_args_list_.end())
    {
        return 0.0;
    }
    return (float)_ttof(m_args_list_.at(szParam).data());
}

bool CArgsList::existsOption(TCHAR szParam) const
{
    switch (szParam)
    {
    case _T('f'):
    case _T('v'):
    case _T('p'):
    case _T('s'):
    case _T('w'):
    case _T('i'):
    case _T('t'):
    case _T('g'):
    case _T('a'):
    case _T('b'):
    case _T('V'):
    case _T('h'):
    case _T('q'):
    case _T('k'):
        return true;
    case _T('L'):
    case _T('e'):
    case _T('o'):
    case _T('r'):
    case _T('m'):
    case _T('d'):
    case _T('u'):
    case _T('n'):
    case _T('y'):
    case _T('x'):
    case _T('z'):
    case _T('c'):
    case _T('j'):
    case _T('l'):
    case _T('M'):
        return false;
    default:
        // undefined parameter
        // A, B, C, D, E, F, G, H, I, J, K, N, O, P, Q, R, S, T, U, W, X, Y, Z
        return false;
    }
}

void CArgsList::removeOption(TCHAR szParam)
{
    this->m_args_list_.erase(szParam);
}

std::vector<TCHAR> CArgsList::getArgumentList() const
{
    std::vector<TCHAR> keys;
    for (const auto &pair : m_args_list_) {
        keys.push_back(pair.first);
    }
    return keys;
}
