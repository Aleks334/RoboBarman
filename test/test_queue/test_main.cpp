#include <unity.h>
#include "Queue.h"

const uint8_t QUEUE_CAPACITY = 5;

namespace TestUtils {
    void fillQueue(Queue& queue, uint8_t startVal, uint8_t count) {
        for (uint8_t i = 0; i < count; i++) {
            queue.insert(startVal + i);
        }
    }

    uint8_t popValue(Queue& queue) {
        uint8_t val = 0;
        queue.pop(val);
        return val;
    }
}

Queue queue;

void setUp() {
    queue = Queue();
}

void tearDown() {}

void test_should_be_empty_on_initialization() {
    bool empty = queue.isEmpty();
    
    TEST_ASSERT_TRUE(empty);
}

void test_should_return_empty_status_when_popping_empty_queue() {
    uint8_t val = 99;

    QueueStatus status = queue.pop(val);

    TEST_ASSERT_EQUAL(QueueStatus::EMPTY, status);
}

void test_should_return_empty_status_when_peeking_empty_queue() {
    uint8_t val = 99;

    QueueStatus status = queue.peek(val);

    TEST_ASSERT_EQUAL(QueueStatus::EMPTY, status);
}

void test_should_add_element_when_queue_has_space() {
    QueueStatus status = queue.insert(42);

    TEST_ASSERT_EQUAL(QueueStatus::OK, status);
    TEST_ASSERT_FALSE(queue.isEmpty());
}

void test_should_not_remove_element_on_peek() {
    queue.insert(42);
    uint8_t val = 0;

    QueueStatus status = queue.peek(val);

    TEST_ASSERT_EQUAL(QueueStatus::OK, status);
    TEST_ASSERT_EQUAL(42, val);
    TEST_ASSERT_FALSE(queue.isEmpty());
}

void test_should_remove_element_on_pop() {
    queue.insert(42);
    uint8_t val = 0;

    QueueStatus status = queue.pop(val);

    TEST_ASSERT_EQUAL(QueueStatus::OK, status);
    TEST_ASSERT_EQUAL(42, val);
    TEST_ASSERT_TRUE(queue.isEmpty());
}

void test_should_output_items_in_fifo_order() {
    queue.insert(10);
    queue.insert(20);
    queue.insert(30);

    TEST_ASSERT_EQUAL(10, TestUtils::popValue(queue));
    TEST_ASSERT_EQUAL(20, TestUtils::popValue(queue));
    TEST_ASSERT_EQUAL(30, TestUtils::popValue(queue));
}

void test_should_refuse_insertion_when_queue_is_full() {
    TestUtils::fillQueue(queue, 0, QUEUE_CAPACITY);

    QueueStatus status = queue.insert(99);

    TEST_ASSERT_EQUAL(QueueStatus::FULL, status);
}

void test_should_wrap_correctly_when_capacity_is_exceeded() {
    TestUtils::fillQueue(queue, 0, QUEUE_CAPACITY); 
    TestUtils::popValue(queue);                   
    TestUtils::popValue(queue);                   
    
    queue.insert(100);             
    queue.insert(200);             
    
    TEST_ASSERT_EQUAL(2, TestUtils::popValue(queue));
    TEST_ASSERT_EQUAL(3, TestUtils::popValue(queue));
    TEST_ASSERT_EQUAL(4, TestUtils::popValue(queue));
    TEST_ASSERT_EQUAL(100, TestUtils::popValue(queue));
    TEST_ASSERT_EQUAL(200, TestUtils::popValue(queue));
    TEST_ASSERT_TRUE(queue.isEmpty());
}

void setup() {
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_should_be_empty_on_initialization);
    RUN_TEST(test_should_return_empty_status_when_popping_empty_queue);
    RUN_TEST(test_should_return_empty_status_when_peeking_empty_queue);
    RUN_TEST(test_should_add_element_when_queue_has_space);
    RUN_TEST(test_should_not_remove_element_on_peek);
    RUN_TEST(test_should_remove_element_on_pop);
    RUN_TEST(test_should_output_items_in_fifo_order);
    RUN_TEST(test_should_refuse_insertion_when_queue_is_full);
    RUN_TEST(test_should_wrap_correctly_when_capacity_is_exceeded);

    UNITY_END();
}

void loop() {}
