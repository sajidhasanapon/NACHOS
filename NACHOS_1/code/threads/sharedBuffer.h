
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
    SharedBuffer(int bufferSize)
    {
        capacity = bufferSize;
    }

    bool IsFull()
    {
        return buffer.size() == capacity;
    }

    bool IsEmpty()
    {
        return buffer.size() == 0;
    }

    void put(int item)
    {
        buffer.push(item);
    }

    int get()
    {
        int item = buffer.front();
        buffer.pop();
        return item;
    }
};

//
//class SharedBuffer
//{
//private:
//    int capacity;
//    int foodNumber;
//    int tableSize;
//
//public:
//    SharedBuffer(int bufferSize)
//    {
//        capacity = bufferSize;
//        foodNumber = 1;
//        tableSize = 0;
//    }
//
//    bool IsFull()
//    {
//        return tableSize == capacity;
//    }
//
//    bool IsEmpty()
//    {
//        return tableSize == 0;
//    }
//
//    void put(int item)
//    {
//        foodNumber++;
//        tableSize++;
//    }
//
//    int get()
//    {
//        tableSize--;
//        return (foodNumber - 1);
//    }
//};




#endif // SHAREDBUFFER_H
