#include "Queue.h"

Queue::Queue() : frontIndex(0), currentSize(0) {}

QueueStatus Queue::insert(uint8_t item)
{
    if (isFull())
    {
        return QueueStatus::FULL;
    }

    uint8_t rearIndex = (frontIndex + currentSize) % CAPACITY;
    arr[rearIndex] = item;
    currentSize++;

    return QueueStatus::OK;
}

QueueStatus Queue::pop(uint8_t &item)
{
    if (isEmpty())
    {
        return QueueStatus::EMPTY;
    }

    item = arr[frontIndex];

    frontIndex = (frontIndex + 1) % CAPACITY;
    currentSize--;

    return QueueStatus::OK;
}

QueueStatus Queue::peek(uint8_t &item) const
{
    if (isEmpty())
    {
        return QueueStatus::EMPTY;
    }

    item = arr[frontIndex];
    return QueueStatus::OK;
}

bool Queue::isFull() const
{
    return currentSize == CAPACITY;
}

bool Queue::isEmpty() const
{
    return currentSize == 0;
}

uint8_t Queue::size() const
{
    return currentSize;
}

bool Queue::contains(uint8_t item) const
{
    for (uint8_t i = 0; i < currentSize; i++)
    {
        uint8_t currentIndex = (frontIndex + i) % CAPACITY;

        if (arr[currentIndex] == item)
        {
            return true;
        }
    }
    return false;
}
