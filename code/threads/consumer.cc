#include "consumer.h"

Consumer::Consumer(const char* debugName, Lock* tableLock, Condition* produceCondition, Condition* consumeCondition, SharedBuffer* foodTable)
{
    name = debugName;
    tableAccessLock = tableLock;
    this->produceCondition = produceCondition;
    this->consumeCondition = consumeCondition;
    this->foodTable = foodTable;
}

void Consumer::Consume()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    for(long i = 0; i < CONSUMPTION_DELAY; i++) {}

    tableAccessLock->Acquire();

    if(foodTable->IsEmpty())
    {
        printf("\n\n---------------Table empty. %s is blocked---------------\n\n\n", currentThread->getName());
        consumeCondition->Wait();
    }

    else
    {
        int foodNumber = foodTable->GetItem();
        printf("[---]   %s consumed %d\n", name, foodNumber);
        produceCondition->Signal();
    }

    tableAccessLock->Release();

    interrupt->SetLevel(oldLevel);		// re-enable interrupts

    for(long i = 0; i < CONSUMPTION_DELAY; i++) {}
}


void Consumer::StartConsuming()
{
    while(true)
    {
        Consume();
    }

//    for(int i = 0; i<10; i++)
//    {
//        Consume();
//    }
}
