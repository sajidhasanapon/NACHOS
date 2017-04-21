#include "producer.h"

Producer::Producer(const char* debugName, Lock* tableLock, Condition* produceCondition, Condition* consumeCondition, SharedBuffer* foodTable)
{
    name = debugName;
    tableAccessLock = tableLock;
    this->produceCondition = produceCondition;
    this->consumeCondition = consumeCondition;
    this->foodTable = foodTable;
}

void Producer::produce()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    for(long i = 0; i <= 90000000; i++) {}

    tableAccessLock->Acquire();

    if(foodTable->IsFull())
    {
        produceCondition->Wait();
    }

    else
    {
        foodTable->put(foodNumber);
        printf("%s produced %d\n", name, foodNumber);
        consumeCondition->Signal();
        foodNumber++;
    }

    tableAccessLock->Release();

    interrupt->SetLevel(oldLevel);		// re-enable interrupts

    for(long i = 0; i <= 90000000; i++) {}
}


void Producer::startProducing()
{
//    while(true)
//    {
//        produce();
//    }

    for(int i = 0; i < 10; i++)
    {
        produce();
    }
}
