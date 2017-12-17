#include "copyright.h"
#include "memoryManager.h"
#include "synch.h"

Lock* memoryLock = new Lock("memoryLock");

MemoryManager::MemoryManager(int nPages)
{
    bitMap = new BitMap(nPages);
}

int MemoryManager::AllocPage()
{
    memoryLock->Acquire();
    int page = bitMap->Find();
    memoryLock->Release();
    return page;
}

void MemoryManager::FreePage(int physPageNumber)
{
    memoryLock->Acquire();
    bitMap->Clear(physPageNumber);
    memoryLock->Release();

    return;
}

bool MemoryManager::PageIsAllocated(int physPageNum)
{
    memoryLock->Acquire();
    bool ret =  bitMap->Test(physPageNum);
    memoryLock->Release();

    return ret;
}

int MemoryManager::GetAvailableMemory()
{
    memoryLock->Acquire();
    int ret = bitMap->NumClear();
    memoryLock->Release();

    return ret;
}
