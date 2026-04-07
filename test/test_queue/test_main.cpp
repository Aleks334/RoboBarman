#include "unity.h"
#include "queue.h"

const uint8_t TEST_QUEUE_CAPACITY = 5;
Queue* queue;

namespace QueueTestUtils
{
    void fill(Queue &q, uint8_t count = TEST_QUEUE_CAPACITY, uint8_t start = 1)
    {
        for (uint8_t i = 0; i < count; i++)
            q.insert(start + i);
    }

    uint8_t popValue(Queue &q)
    {
        uint8_t val = 0;
        q.pop(val);
        return val;
    }
}

using namespace QueueTestUtils;

void setUp()
{
    queue = new Queue(TEST_QUEUE_CAPACITY);
}

void tearDown() 
{
    delete queue;
}

void test_should_be_empty_on_init()
{
    uint8_t val;

    TEST_ASSERT_TRUE(queue->isEmpty());
    TEST_ASSERT_EQUAL(0, queue->size());

    TEST_ASSERT_EQUAL(QueueStatus::EMPTY, queue->pop(val));
    TEST_ASSERT_EQUAL(QueueStatus::EMPTY, queue->peek(val));
}

void test_should_add_single_item()
{
    const uint8_t item = 42;

    TEST_ASSERT_EQUAL(QueueStatus::OK, queue->insert(item));
    TEST_ASSERT_EQUAL(1, queue->size());
    TEST_ASSERT_TRUE(queue->contains(item));
}

void test_should_return_without_removing_during_peek()
{
    queue->insert(42);
    uint8_t val;

    TEST_ASSERT_EQUAL(QueueStatus::OK, queue->peek(val));
    TEST_ASSERT_EQUAL(42, val);
    TEST_ASSERT_EQUAL(1, queue->size());
}

void test_should_remove_item_during_pop()
{
    queue->insert(42);
    uint8_t val;

    TEST_ASSERT_EQUAL(QueueStatus::OK, queue->pop(val));
    TEST_ASSERT_EQUAL(42, val);
    TEST_ASSERT_TRUE(queue->isEmpty());
}

void test_should_provide_items_in_fifo_order()
{
    queue->insert(10);
    queue->insert(20);
    queue->insert(30);

    TEST_ASSERT_EQUAL(10, popValue(*queue));
    TEST_ASSERT_EQUAL(20, popValue(*queue));
    TEST_ASSERT_EQUAL(30, popValue(*queue));
}

void test_should_signal_full_status()
{
    fill(*queue);

    TEST_ASSERT_TRUE(queue->isFull());
    TEST_ASSERT_EQUAL(QueueStatus::FULL, queue->insert(99));
}

void test_should_reuse_memory_circularly_after_wrapping()
{
    fill(*queue); // 1,2,3,4,5

    popValue(*queue);
    popValue(*queue);

    TEST_ASSERT_EQUAL(QueueStatus::OK, queue->insert(100));
    TEST_ASSERT_EQUAL(QueueStatus::OK, queue->insert(200));

    TEST_ASSERT_EQUAL(3, popValue(*queue));
    TEST_ASSERT_EQUAL(4, popValue(*queue));
    TEST_ASSERT_EQUAL(5, popValue(*queue));
    TEST_ASSERT_EQUAL(100, popValue(*queue));
    TEST_ASSERT_EQUAL(200, popValue(*queue));
    TEST_ASSERT_TRUE(queue->isEmpty());
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_should_be_empty_on_init);
    RUN_TEST(test_should_add_single_item);
    RUN_TEST(test_should_return_without_removing_during_peek);
    RUN_TEST(test_should_remove_item_during_pop);
    RUN_TEST(test_should_provide_items_in_fifo_order);
    RUN_TEST(test_should_signal_full_status);
    RUN_TEST(test_should_reuse_memory_circularly_after_wrapping);

    UNITY_END();
}

void loop() {}