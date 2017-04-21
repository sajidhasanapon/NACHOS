#include "synch.h"

class Consumer
{
private:
    const char* name;                      // debugging purpose
    Lock* tableAccessLock;                  // Permission for going to food table
    Condition* tableHandlingCondition;      // After going to food table, permission to consume food

    void consume();

public:
    Consumer(const char* debugName, Lock* tableLock, Condition* tableCondition);
    void startConsuming();
};
