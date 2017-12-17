
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

void SharedBuffer::PutItem(int item)
{
    buffer.push(item);
}

int SharedBuffer::GetItem()
{
    int item = buffer.front();
    buffer.pop();
    return item;
}
