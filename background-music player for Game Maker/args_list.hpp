#pragma once

#include "stdafx.hpp"

class CArgsList
{
public:
    using argument_list = std::map<TCHAR, tstring>;
    CArgsList(int &argc, TCHAR** &argv);
    virtual ~CArgsList();
    inline bool hasTargetWindowClassName() const
    {
        return hasOption(_T('t'));
    }

    inline tstring getTargetWindowClassName() const
    {
        return getTstringOption(_T('t'));
    }

    inline bool hasMyWindowClassName() const
    {
        return hasOption(_T('i'));
    }

    inline tstring getMyWindowClassName() const
    {
        return getTstringOption(_T('i'));
    }
    
    inline float getVolume() const
    {
        return getFloatOption(_T('v'));
    }

    inline float getPan() const
    {
        return getFloatOption(_T('p'));
    }

    inline float getPitch() const
    {
        return getFloatOption(_T('s'));
    }

    inline int getRepeatCount() const
    {
        return getIntOption(_T('g'));
    }

    inline int getRepeatPointA() const
    {
        return getIntOption(_T('a'));
    }

    inline int getRepeatPointB() const
    {
        return getIntOption(_T('b'));
    }

    inline int getFadeInFrame() const
    {
        return getIntOption(_T('h'));
    }

    inline int getFadeOutFrame() const
    {
        return getIntOption(_T('q'));
    }

    inline float getFadedVolume() const
    {
        return getFloatOption(_T('V'));
    }

    inline int getMovedPosition() const
    {
        return getIntOption(_T('k'));
    }

    inline long getLongGameMakerWindowHWND() const
    {
        return getLongOption(_T('w'));
    }
    
    inline void removeTargetWindowClassName()
    {
        return removeOption(_T('t'));
    }

    inline void removeMyWindowClassName()
    {
        return removeOption(_T('i'));
    }

    inline tstring getFileName() const
    {
        return getTstringOption(_T('f'));
    }

    inline bool getIsLooping() const
    {
        return hasOption(_T('L'));
    }

    inline bool getIsDebuging() const
    {
        return hasOption(_T('e'));
    }

    inline bool getIsLoadOnly() const
    {
        return hasOption(_T('z'));
    }
    
    std::vector<TCHAR> getArgumentList() const;
private:
    bool hasOption(TCHAR szParam) const;
    void removeOption(TCHAR szParam);
    int getIntOption(TCHAR szParam) const;
    long getLongOption(TCHAR szParam) const;
    tstring getTstringOption(TCHAR szParam) const;
    float getFloatOption(TCHAR szParam) const;
    bool existsOption(TCHAR szParam) const;
    argument_list m_args_list_;
};
