#include "consumer.h"

Consumer::Consumer(const char* debugName, Lock* tableLock, Condition* produceCondition, Condition* consumeCondition, SharedBuffer* foodTable)
{
    name = debugName;
    tableAccessLock = tableLock;
    this->produceCondition = produceCondition;
    this->consumeCondition = consumeCondition;
    this->foodTable = foodTable;
}

void Consumer::consume()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    for(long i = 0; i <= 10000000; i++) {}

    tableAccessLock->Acquire();

    if(foodTable->IsEmpty())
    {
        consumeCondition->Wait();
    }

    else
    {
        int foodNumber = foodTable->get();
        printf("%s consumed %d\n", name, foodNumber);
        produceCondition->Signal();
    }

    tableAccessLock->Release();

    interrupt->SetLevel(oldLevel);		// re-enable interrupts

    for(long i = 0; i <= 10000000; i++) {}
}


void Consumer::startConsuming()
{
//    while(true)
//    {
//        consume();
//    }

    for(int i = 0; i<10; i++)
    {
        consume();
    }
}
