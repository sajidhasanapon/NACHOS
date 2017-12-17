#ifndef SWAP_H
#define SWAP_H

#include "filesys.h"
#include <string>

class SwapFile
{
private:
    OpenFile* swapFile;
    std::string swapFileName;

    int numberOfPages;
    bool* map;
public:
    SwapFile(int fileNumber, int numberOfPages);
    ~SwapFile();

    void StoreInSwapSpaceFromMemory(int pageNumber, int frameNumber);
    void LoadIntoMemoryFromSwapSpace(int pageNumber, int frameNumber);
    bool IsStoredInSwapSpace(int pageNumber);
};

#endif // SWAP_H
