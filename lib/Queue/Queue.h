#pragma once
#include <Arduino.h>

enum class QueueStatus : uint8_t {
    OK,
    EMPTY,
    FULL
};

class Queue {
    private:
        static constexpr uint8_t CAPACITY = 5; 
        uint8_t arr[CAPACITY];
        uint8_t frontIndex;
        uint8_t currentSize;

    public:
        Queue();
        QueueStatus insert(uint8_t item);
        QueueStatus pop(uint8_t& item);
        QueueStatus peek(uint8_t& item) const;
        bool isFull() const;
        bool isEmpty() const;
        bool contains(uint8_t item) const;
        uint8_t size() const;
};