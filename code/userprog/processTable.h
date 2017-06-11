#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include "copyright.h"
#include "utility.h"

class ProcessTable
{
public:
    void** arrayOfThreadPointers; // the processes
    int nProcesses;
    int tableSize;

    ProcessTable(int size);

    ~ProcessTable();

    int Alloc(void* object);

    void* Get(int index);

    void Release(int index);

    int GetProcessCount();
};

#endif //PROCESSTABLE_H
