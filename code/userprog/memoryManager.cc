
#include "memoryManager.h"
#include "processTable.h"
#include "synch.h"

#define RANDOM_REPLACEMENT 0
#define LRU_REPLACEMENT    1

const int REPLACEMENT_POLICY = RANDOM_REPLACEMENT;

extern ProcessTable*  processTable;
Lock* memoryLock = new Lock("memoryLock");


MemoryManager::MemoryManager(int numberOfFrames)
{
    timeStamp = 0;

    this->numberOfFrames = numberOfFrames;
    numberOfFreeFrames = numberOfFrames;
    frames = new Frame[numberOfFrames];

    for(unsigned i = 0; i < numberOfFrames; i++)
    {
        frames[i].isOccupied     = false;
        frames[i].processID      = -1;
        frames[i].pageTableEntry = NULL;
        frames[i].lastAccessTime = 0;
    }
}

MemoryManager::~MemoryManager()
{
    delete[] frames;
}

int MemoryManager::AllocateFrame(int spaceID, TranslationEntry* pageTableEntry)
{
    memoryLock->Acquire();

    int allocatedFrameNumber;

    if(numberOfFreeFrames > 0)
    {
        for(unsigned i = 0; i < numberOfFrames; i++)
        {
            if(frames[i].isOccupied == false)
            {
                DEBUG('x', "From memoryManager.cc :\n");
                DEBUG('x', "Free frame available. Frame to allocate: %d\n", i);
                DEBUG('x', "Allocating...\n");

                allocatedFrameNumber = i;
                numberOfFreeFrames--;

                DEBUG('x', "Number of free frames decremented. Current number of free frames : %d\n\n", numberOfFreeFrames);

                break;
            }
        }
    }

    else allocatedFrameNumber = AllocateFrameByForce();
    // no empty frame; must evict an existing page

    frames[allocatedFrameNumber].isOccupied     = true;
    frames[allocatedFrameNumber].processID      = spaceID;
    frames[allocatedFrameNumber].pageTableEntry = pageTableEntry;
    // "frames[i].lastAccessTime" will be set from translate.cc

    memoryLock->Release();

    return allocatedFrameNumber;
}

int MemoryManager::AllocateFrameByForce()
{
    DEBUG('x', "From memoryManager.cc :\n");
    DEBUG('x', "No unoccupied frame availabe. ");

    int victim;
    if(REPLACEMENT_POLICY == RANDOM_REPLACEMENT)
    {
        DEBUG('x', "Randomly selecting a frame to evict...\n");
        victim = rand() % numberOfFrames;
    }
    else if(REPLACEMENT_POLICY == LRU_REPLACEMENT)
    {
        DEBUG('x', "Searching for LRU frame to evict...\n");
        victim = GetLRUFrameNumber();
    }
    int victimID = frames[victim].processID;

    Thread* thread = (Thread*) processTable->Get(victimID);
    AddrSpace* space = thread->space;

    TranslationEntry* pageTableEntry = frames[victim].pageTableEntry;
    int victimPageNumber = pageTableEntry->virtualPage;

    DEBUG('x', "Frame to evict: %d\n" ,victim);
    DEBUG('x', "victim proces: %d, victim vpn: %d\n", victimID, victimPageNumber);
    DEBUG('x', "Evicting...\n\n");

    space->ForcedEvict(victimPageNumber);

    DEBUG('x', "From memoryManager.cc :\n");
    DEBUG('x', "Allocating...\n\n");

    return victim;
}

int MemoryManager::GetLRUFrameNumber()
{
    // simplest LRU replacement policy
    // should try to avoid replacing a dirty page, at the very least
    // well... who cares?

    int LRUFrameNumber, minTimeStamp;

    LRUFrameNumber = 0;
    minTimeStamp   = frames[0].lastAccessTime;
    // initializing the first frame as the LRU frame;

    for(unsigned i = 1; i < numberOfFrames; i++)
    {
        if(frames[i].lastAccessTime < minTimeStamp)
        {
            LRUFrameNumber = i;
            minTimeStamp   = frames[i].lastAccessTime;
        }
    }

    return LRUFrameNumber;
}

void 
MemoryManager::UpdateLastAccessTime(int frameNumber)
{
    timeStamp++;
    frames[frameNumber].lastAccessTime = timeStamp;
}

void MemoryManager::FreeFrame(int frameNumber)
{
    memoryLock->Acquire();

    if(frames[frameNumber].isOccupied)
    {
        DEBUG('x', "From memoryManager.cc :\n");
        DEBUG('x', "Freeing page frame...\n");
        frames[frameNumber].isOccupied = false;
        numberOfFreeFrames++;

        DEBUG('x', "Freed page frame number: %d\n", frameNumber);
        DEBUG('x', "Number of free frames incremented. Current number of free frames : %d\n\n", numberOfFreeFrames);
    }

    memoryLock->Release();

    return;
}

bool MemoryManager::FrameIsAllocated(int frameNumber)
{
    memoryLock->Acquire();
    bool ret =  frames[frameNumber].isOccupied;
    memoryLock->Release();

    return ret;
}

int MemoryManager::GetAvailableMemory()
{
    memoryLock->Acquire();
    int ret = numberOfFreeFrames;
    memoryLock->Release();

    return ret;
}
