#include "unity.h"
#include "Queue.h"

const uint8_t QUEUE_CAPACITY = 5;
Queue queue;

namespace TestUtils
{
    void fill(Queue &q, uint8_t count, uint8_t start = 1)
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

void setUp()
{
    queue = Queue();
}

void tearDown() {}

void test_should_be_empty_on_init()
{
    uint8_t val;

    TEST_ASSERT_TRUE(queue.isEmpty());
    TEST_ASSERT_EQUAL(0, queue.size());

    TEST_ASSERT_EQUAL(QueueStatus::EMPTY, queue.pop(val));
    TEST_ASSERT_EQUAL(QueueStatus::EMPTY, queue.peek(val));
}

void test_should_add_single_item()
{
    const uint8_t item = 42;

    TEST_ASSERT_EQUAL(QueueStatus::OK, queue.insert(item));
    TEST_ASSERT_EQUAL(1, queue.size());
    TEST_ASSERT_TRUE(queue.contains(item));
}

void test_should_return_without_removing_during_peek()
{
    queue.insert(42);
    uint8_t val;

    TEST_ASSERT_EQUAL(QueueStatus::OK, queue.peek(val));
    TEST_ASSERT_EQUAL(42, val);
    TEST_ASSERT_EQUAL(1, queue.size());
}

void test_should_remove_item_during_pop()
{
    queue.insert(42);
    uint8_t val;

    TEST_ASSERT_EQUAL(QueueStatus::OK, queue.pop(val));
    TEST_ASSERT_EQUAL(42, val);
    TEST_ASSERT_TRUE(queue.isEmpty());
}

void test_should_provide_items_in_fifo_order()
{
    queue.insert(10);
    queue.insert(20);
    queue.insert(30);

    TEST_ASSERT_EQUAL(10, TestUtils::popValue(queue));
    TEST_ASSERT_EQUAL(20, TestUtils::popValue(queue));
    TEST_ASSERT_EQUAL(30, TestUtils::popValue(queue));
}

void test_should_signal_full_status()
{
    TestUtils::fill(queue, QUEUE_CAPACITY);

    TEST_ASSERT_TRUE(queue.isFull());
    TEST_ASSERT_EQUAL(QueueStatus::FULL, queue.insert(99));
}

void test_should_reuse_memory_circularly_after_wrapping()
{
    TestUtils::fill(queue, QUEUE_CAPACITY); // 1, 2, 3, 4, 5

    TestUtils::popValue(queue);
    TestUtils::popValue(queue);

    TEST_ASSERT_EQUAL(QueueStatus::OK, queue.insert(100));
    TEST_ASSERT_EQUAL(QueueStatus::OK, queue.insert(200));

    TEST_ASSERT_EQUAL(3, TestUtils::popValue(queue));
    TEST_ASSERT_EQUAL(4, TestUtils::popValue(queue));
    TEST_ASSERT_EQUAL(5, TestUtils::popValue(queue));
    TEST_ASSERT_EQUAL(100, TestUtils::popValue(queue));
    TEST_ASSERT_EQUAL(200, TestUtils::popValue(queue));
    TEST_ASSERT_TRUE(queue.isEmpty());
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