#ifndef SYNCHRONIZEDCONSOLE_H
#define SYNCHRONIZEDCONSOLE_H

#include "copyright.h"
#include "console.h"

class SynchronizedConsole
{
private:
    Console *console;
public:
    SynchronizedConsole();

    void GetLock();

    void ReleaseLock();

    char GetChar();

    void PutChar(char ch);
};

#endif //SYNCHRONIZEDCONSOLE_H
