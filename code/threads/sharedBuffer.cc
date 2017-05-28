
#include "sharedBuffer.h"

SharedBuffer::SharedBuffer(int bufferSize)
{
    capacity = bufferSize;
}

bool SharedBuffer::IsFull()
{
    return buffer.size() == capacity;
}

bool SharedBuffer::IsEmpty()
{
    return buffer.size() == 0;
}

void SharedBuffer::put(int item)
{
    buffer.push(item);
}

int SharedBuffer::get()
{
    int item = buffer.front();
    buffer.pop();
    return item;
}
