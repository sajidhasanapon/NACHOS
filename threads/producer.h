#include "synch.h"

class Producer
{
private:
    const char* name;                      // debugging purpose
    Lock* tableAccessLock;                  // Permission for going to food table
    Condition* tableHandlingCondition;      // After going to food table, permission to put food there

    void produce();


public:
    Producer(const char* debugName, Lock* tableLock, Condition* tableCondition);
    void startProducing();
};
