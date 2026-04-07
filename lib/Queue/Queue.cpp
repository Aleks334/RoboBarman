#include "Queue.h"

Queue::Queue(uint8_t capacity) : capacity(capacity), frontIndex(0), currentSize(0) {
    arr = new uint8_t[capacity];
}

Queue::~Queue() {
    delete[] arr;
}

QueueStatus Queue::insert(uint8_t item)
{
    if (isFull())
    {
        return QueueStatus::FULL;
    }

    uint8_t rearIndex = (frontIndex + currentSize) % capacity;
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

    frontIndex = (frontIndex + 1) % capacity;
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
    return currentSize == capacity;
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
        uint8_t currentIndex = (frontIndex + i) % capacity;

        if (arr[currentIndex] == item)
        {
            return true;
        }
    }
    return false;
}
