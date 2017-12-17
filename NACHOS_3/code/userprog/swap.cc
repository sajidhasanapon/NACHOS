#include "swap.h"
#include "system.h"
#include <sstream>

SwapFile::SwapFile(int fileNumber, int numberOfPages)
{
    this->numberOfPages = numberOfPages;

    map = new bool[numberOfPages];
    for(unsigned i = 0; i < numberOfPages; i++)
    {
        map[i] = false;
    }

	std::stringstream nameStream;
  	nameStream << "Swap." << fileNumber;
  	swapFileName = nameStream.str();

    int fileSize = numberOfPages * PageSize;
  	fileSystem->Create(swapFileName.c_str(), fileSize);

	swapFile = fileSystem->Open(swapFileName.c_str());
}

SwapFile::~SwapFile()
{
  delete swapFile;
  delete [] map;
  fileSystem->Remove(swapFileName.c_str());
}


void
SwapFile::StoreInSwapSpaceFromMemory(int pageNumber, int frameNumber)
{
    char* from = machine->mainMemory + frameNumber * PageSize;
    int into = pageNumber * PageSize;

    swapFile->WriteAt(from, PageSize, into);
    map[pageNumber] = true; // mark page as stored
}

void
SwapFile::LoadIntoMemoryFromSwapSpace(int pageNumber, int frameNumber)
{
    char* into = machine->mainMemory + frameNumber * PageSize;
    int from = pageNumber * PageSize;

    swapFile->ReadAt(into, PageSize, from);
}

bool
SwapFile::IsStoredInSwapSpace(int pageNumber)
{
    return map[pageNumber];
}
