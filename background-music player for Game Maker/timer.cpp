#include "timer.hpp"

Timer::Timer()
{
    Restart();
}

Timer::~Timer()
{
}

void Timer::Restart()
{
    m_start = ::GetTickCount();
}

double Timer::Elapsed()
{
    DWORD end = ::GetTickCount();
    return (double)(end - m_start) / 1000;
}
