#include "producer.h"

Producer::Producer(const char* debugName, Lock* tableLock, Condition* tableCondition)
{
    name = debugName;
    tableAccessLock = tableLock;
    tableHandlingCondition = tableCondition;
}

void Producer::produce()
{
    //TODO
}


void Producer::startProducing()
{
    while(true)
    {
        produce();
    }
}
