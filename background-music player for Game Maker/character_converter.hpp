#pragma once

#include "stdafx.hpp"

class CCharacterConverter
{
public:
    CCharacterConverter();
    std::string ToString(const std::wstring &src);
    std::string ToString(const std::string &src);
    std::wstring ToWstring(const std::wstring &src);
    std::wstring ToWstring(const std::string &src);
    tstring ToTstring(const char *szChar);
    tstring ToTstring(const wchar_t *szWchar);
    virtual ~CCharacterConverter();
};
