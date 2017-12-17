#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "system.h"

class MemoryManager
{
private:
    int timeStamp;  // increased whenever a memory location is accessed.
                    // whenever a frame is accessed, its...
                    // ... "lastAccessTime" field set equal to...
                    // the latest timeStamp.
                    // thus, recently accessed pages have greater...
                    // ... timeStamps than older pages.

                    // the timeStamp is incremented in
                    // "UpdateLastAccessTime()" method...
                    // ... which is defined in translate.cc

    int numberOfFrames;
    int numberOfFreeFrames;

    typedef struct
    {
        bool isOccupied;
        int processID;
        TranslationEntry* pageTableEntry;
        int lastAccessTime;
    } Frame;

    Frame* frames;

public:
    MemoryManager(int numberOfFrames);

    ~MemoryManager();

    int AllocateFrame(int spaceID, TranslationEntry* pageTableEntry);

    int AllocateFrameByForce();

    int GetLRUFrameNumber();

    void FreeFrame(int frameNumber);

    bool FrameIsAllocated(int frameNumber);

    int GetAvailableMemory();

    void UpdateLastAccessTime(int frameNumber);

};

#endif // MEMORYMANAGER_H
