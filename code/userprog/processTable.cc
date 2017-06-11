#include "copyright.h"
#include "processTable.h"
#include "synch.h"

Lock* processTableLock = new Lock("processTableLock");

ProcessTable::ProcessTable(int size)
{
    arrayOfThreadPointers = new void*[size+1];

    for(int i = 1; i <= size; i++) arrayOfThreadPointers[i] = NULL;

    nProcesses = 0;
    tableSize = size;
}

ProcessTable::~ProcessTable()
{
    delete[] arrayOfThreadPointers;
}

int ProcessTable::Alloc(void* object)
{
    processTableLock->Acquire();
    int ret = -1;

    for(int i = 1; i <= tableSize; i++)
    {
        if(arrayOfThreadPointers[i] == NULL)
        {
            arrayOfThreadPointers[i] = object;
            nProcesses++;
            ret = i;
            break;
        }
    }
    processTableLock->Release();
    return ret;

}

void* ProcessTable::Get(int index)
{
    processTableLock->Acquire();

    if(arrayOfThreadPointers[index] != NULL)
    {
        processTableLock->Release();
        return arrayOfThreadPointers[index];
    }

    processTableLock->Release();
    return NULL;
}

void ProcessTable::Release(int index)
{
    processTableLock->Acquire();
    arrayOfThreadPointers[index] = NULL;
    nProcesses--;
    processTableLock->Release();
}

int ProcessTable::GetProcessCount()
{
    return nProcesses;
}
