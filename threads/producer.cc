#include "producer.h"

Producer::Producer(const char* debugName, Lock* tableLock, Condition* produceCondition, Condition* consumeCondition, SharedBuffer* foodTable)
{
    name = debugName;
    tableAccessLock = tableLock;
    this->produceCondition = produceCondition;
    this->consumeCondition = consumeCondition;
    this->foodTable = foodTable;
}

int Producer::foodNumber = 0;

void Producer::Produce()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    for(long i = 0; i < PRODUCTION_DELAY; i++) {}

    tableAccessLock->Acquire();

    if(foodTable->IsFull())
    {
        printf("\n\n+++++++++++++++Table full. %s is blocked+++++++++++++++\n\n\n", currentThread->getName());
        produceCondition->Wait();
    }

    else
    {
        foodTable->put(foodNumber);
        printf("[+++]   %s produced %d\n", name, foodNumber);
        consumeCondition->Signal();
        foodNumber++;
    }

    tableAccessLock->Release();

    interrupt->SetLevel(oldLevel);		// re-enable interrupts

    for(long i = 0; i < PRODUCTION_DELAY; i++) {}
}


void Producer::StartProducing()
{
    while(true)
    {
        Produce();
    }

//    for(int i = 0; i < 10; i++)
//    {
//        Produce();
//    }
}
