#include "character_converter.hpp"

CCharacterConverter::CCharacterConverter()
{
}

CCharacterConverter::~CCharacterConverter()
{
}

std::string CCharacterConverter::ToString(const std::wstring &src)
{
    std::size_t converted{};
    std::vector<char> dest(src.size() * sizeof(wchar_t) + 1, '\0');
    if (::_wcstombs_s_l(&converted, dest.data(), dest.size(), src.data(), _TRUNCATE, ::_create_locale(LC_ALL, "jpn")) != 0)
    {
        tcerr << std::endl << _T("ToString ERR:") << errno << std::endl << std::endl;
        throw std::system_error{ errno, std::system_category() };
    }
    dest.resize(std::char_traits<char>::length(dest.data()));
    dest.shrink_to_fit();
    return std::string(dest.begin(), dest.end());
}

std::string CCharacterConverter::ToString(const std::string &src)
{ 
    return src;
}

std::wstring CCharacterConverter::ToWstring(const std::wstring &src)
{
    return src;
}

std::wstring CCharacterConverter::ToWstring(const std::string &src)
{
    std::size_t converted{};
    std::vector<wchar_t> dest(src.size() + 1, L'\0');
    if (::_mbstowcs_s_l(&converted, dest.data(), dest.size(), src.data(), _TRUNCATE, ::_create_locale(LC_ALL, "jpn")) != 0)
    {
        tcerr << std::endl << _T("ToWstring ERR:") << errno << std::endl << std::endl;
        throw std::system_error{ errno, std::system_category() };
    }
    dest.resize(std::char_traits<wchar_t>::length(dest.data()));
    dest.shrink_to_fit();
    return std::wstring(dest.begin(), dest.end());
}

tstring CCharacterConverter::ToTstring(const char *szChar)
{
    std::string str = szChar;
#if defined(UNICODE) || defined(_UNICODE)
    return ToWstring(str);
#else
    return ToString(str);
#endif
}

tstring CCharacterConverter::ToTstring(const wchar_t *szWchar)
{
    std::wstring wstr = szWchar;
#if defined(UNICODE) || defined(_UNICODE)
    return ToWstring(wstr);
#else
    return ToString(wstr);
#endif
}
