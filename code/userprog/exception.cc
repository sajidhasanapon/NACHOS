// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "addrspace.h"
#include "synchronizedConsole.h"
#include "synch.h"

extern MemoryManager* memoryManager;
extern ProcessTable*  processTable;
extern SynchronizedConsole* synchronizedConsole;

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------

void ForwardPC()
{
    int pc;

    pc = machine->ReadRegister(PCReg);
    machine->WriteRegister(PrevPCReg, pc);

    pc = machine->ReadRegister(NextPCReg);
    machine->WriteRegister(PCReg, pc);

    pc += 4;
    machine->WriteRegister(NextPCReg, pc);
}

void DummyForFork(void* arg)
{
    (currentThread->space)->InitRegisters();
    (currentThread->space)->RestoreState();
    machine->Run();

    return;
}

void SysCallHaltHandler();
void SysCallExecHandler();
void SysCallExitHandler();
void SysCallReadHandler();
void SysCallWriteHandler();

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    switch (which)
    {
    case SyscallException:
        DEBUG('z', "\nInside SyscallException from thread %d.\n", currentThread->threadID);
        DEBUG('z',"### SyscallException Type: %d ###\n", type);

        switch (type)
        {
        case SC_Halt:
            SysCallHaltHandler();
            break;

        case SC_Exec:
            SysCallExecHandler();
            break;

        case SC_Exit:
            SysCallExitHandler();
            break;

        case SC_Read:
            SysCallReadHandler();
            break;

        case SC_Write:
            SysCallWriteHandler();
        default:
            break;
        }
        break;

    case PageFaultException:
        printf("Page Fault Exception\n");
        SysCallExitHandler();
        break;

    case ReadOnlyException:
        printf("Read Onle Exception\n");
        SysCallExitHandler();
        break;

    case BusErrorException:
        printf("Bus Error Exception\n");
        SysCallExitHandler();
        break;

    case OverflowException:
        printf("Overflow Exception\n");
        SysCallExitHandler();
        break;

    case IllegalInstrException:
        printf("Illegal Instruction Exception\n");
        SysCallExitHandler();
        break;

    case NumExceptionTypes:
        printf("Num Exception Types\n");
        SysCallExitHandler();
        break;

    default:
        printf("Unexpected user mode exception %d %d\n", which, type);
        SysCallExitHandler();
        break;
    }

    ForwardPC(); // this has to be done after every instruction
}

void
SysCallHaltHandler()
{
    DEBUG('a', "Shutdown, initiated by user program.\n");
    interrupt->Halt();

    return;
}


void
SysCallExecHandler()
{
    char* fileName;
    int fileNameLength = machine->ReadRegister(5);
    fileName = new char[fileNameLength + 1];
    // It is bad practice to create a fixed-size buffer.
    // For example, char* fileName = new char[100].
    // Should get the length first and then initialize the buffer, ...
    // ... ike I did here (like a boss :D ).

    int addr = machine->ReadRegister(4);
    int i = 0;

    do
    {
        machine->ReadMem(addr + i, 1, (int*)&fileName[i]);

    }
    while(fileName[i++] != '\0');


    OpenFile* executable = fileSystem->Open(fileName);
    if (executable == NULL)
    {
        printf("Unable to open file %s\n", fileName);
        machine->WriteRegister(2, -1);
        return;
    }

    AddrSpace* space = new AddrSpace(executable);

    Thread* thread = new Thread(fileName);
    thread->space=space;
    int returnValue = processTable->Alloc((void*)thread);
    thread->threadID = returnValue;
    if(returnValue == -1)
    {
        printf("Allocation failed\n" );
        return;
    }

    DEBUG('z', "%d inserted in processTable\n", thread->threadID);
    machine->WriteRegister(2, returnValue);

    delete executable; // close file
    delete fileName;   // free the buffer memory storing the name

    thread->Fork(DummyForFork, NULL);

    return;
}

void
SysCallExitHandler()
{
    DEBUG('z',"Inside Exit\n");
    int addr = machine->ReadRegister(4);

    currentThread->space->ReleaseMemory();
    delete currentThread->space;

    int index = currentThread->threadID;	// index in processTable
    processTable->Release(index);

    int processCount = processTable->GetProcessCount();
    if(processCount == 0)	// last process
    {
        interrupt->Halt();
    }
    // else
    currentThread->Finish();

    printf("Exit : %d\n", currentThread->threadID);
    return;
}


void
SysCallReadHandler()
{
    unsigned int addr = machine->ReadRegister(4);
    unsigned int size = machine->ReadRegister(5);

    synchronizedConsole->GetLock();

    char* buffer = new char[size];

    for(int i = 0; i < size; i++)
    {
        buffer[i] = synchronizedConsole->GetChar();
    }
    buffer[size] = '\0';
    for(int i = 0; i < size; i++)
    {
        machine->WriteMem(addr + i,1, (int)buffer[i]);
    }

    DEBUG('z', "Size of string = %d\n", size);
    machine->WriteRegister(2,size);


    DEBUG('z', "got :  %s\n", buffer);

    bzero(buffer, sizeof(char) * size);

    synchronizedConsole->ReleaseLock();

    return;
}


void
SysCallWriteHandler()
{
    unsigned int addr = machine->ReadRegister(4);
    unsigned int size = machine->ReadRegister(5);

    synchronizedConsole->GetLock();
    for(int i = 0; i < size; i++)
    {
        int c;
        machine->ReadMem(addr + i, 1, &c);
        synchronizedConsole->PutChar((char)c);
    }
    synchronizedConsole->PutChar('\n');

    synchronizedConsole->ReleaseLock();

    return;
}
