#include "consumer.h"

Consumer::Consumer(const char* debugName, Lock* tableLock, Condition* tableCondition)
{
    name = debugName;
    tableAccessLock = tableLock;
    tableHandlingCondition = tableCondition;
}

void Consumer::consume()
{
    //TODO
}


void Consumer::startConsuming()
{
    while(true)
    {
        consume();
    }
}
