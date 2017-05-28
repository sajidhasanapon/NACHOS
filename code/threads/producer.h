#ifndef PRODUCER_H
#define PRODUCER_H


#include "system.h"
#include "synch.h"
#include "sharedBuffer.h"

class Producer
{
private:
    const char* name;                      // debugging purpose
    Lock* tableAccessLock;                  // Permission for going to food table
    Condition* produceCondition;
    Condition* consumeCondition;
    SharedBuffer* foodTable;

    static int foodNumber;


    void Produce();


public:
    Producer(const char* debugName, Lock* tableLock,
             Condition* produceCondition, Condition* consumeCondition, SharedBuffer* foodTable);
    void StartProducing();
};


#endif // PRODUCER_H
