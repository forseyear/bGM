#pragma once

#include "stdafx.hpp"

class CTempFileCreator
{
public:
    CTempFileCreator();
    virtual ~CTempFileCreator();
    void Create(tstring &szFileName, bool bValue);
    void Create(tstring &szFileName, int iNumber);
};
