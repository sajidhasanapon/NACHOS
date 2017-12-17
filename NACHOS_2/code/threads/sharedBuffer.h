#ifndef SHAREDBUFFER_H
#define SHAREDBUFFER_H

#define PRODUCTION_DELAY    100000000
#define CONSUMPTION_DELAY    60000000

#include <queue>
using namespace std;

class SharedBuffer
{
private:
    queue<int> buffer;
    int capacity;

public:
    SharedBuffer(int bufferSize);

    bool IsFull();

    bool IsEmpty();

    void PutItem(int item);

    int GetItem();
};

#endif // SHAREDBUFFER_H
