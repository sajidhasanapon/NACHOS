#ifndef CONSUMER_H
#define CONSUMER_H


#include "system.h"
#include "synch.h"
#include "sharedBuffer.h"

class Consumer
{
private:
    const char* name;                      // debugging purpose
    Lock* tableAccessLock;                 // Permission for going to food table
    Condition* produceCondition;
    Condition* consumeCondition;
    SharedBuffer* foodTable;

    void Consume();

public:
    Consumer(const char* debugName, Lock* tableLock,
             Condition* produceCondition, Condition* consumeCondition, SharedBuffer* foodTable);
    void StartConsuming();
};


#endif // CONSUMER_H
