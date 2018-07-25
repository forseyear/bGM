#pragma once

#include "stdafx.hpp"

class CConsole
{
public:
    CConsole();
    void Create();
    virtual ~CConsole();
private:
    FILE *fp;
};
