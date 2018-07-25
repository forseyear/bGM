#include "tempfile_creator.hpp"

CTempFileCreator::CTempFileCreator()
{
}

CTempFileCreator::~CTempFileCreator()
{
}

void CTempFileCreator::Create(tstring &szFileName, bool bValue)
{
    std::ofstream outputfile(szFileName);
    outputfile << bValue;
    outputfile.close();
}

void CTempFileCreator::Create(tstring &szFileName, int iNumber)
{
    std::ofstream outputfile(szFileName);
    outputfile << iNumber;
    outputfile.close();
}

