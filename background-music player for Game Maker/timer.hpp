#pragma once

#include "stdafx.hpp"

class Timer
{
public:
    Timer();
    virtual ~Timer();
    void Restart();
    double Elapsed();
private:
    DWORD m_start;
};
