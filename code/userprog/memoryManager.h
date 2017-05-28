#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "copyright.h"
#include "bitmap.h"


class MemoryManager
{
private:
    BitMap *bitMap;

public:
    MemoryManager(int nPages);

    int AllocPage();

    void FreePage(int physPageNumber);

    bool PageIsAllocated(int physPageNum);

    int GetAvailableMemory();

};

#endif // MEMORYMANAGER_H
