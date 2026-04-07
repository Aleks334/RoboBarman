#include "test_common.h"
using namespace TestUtils;

unsigned long TestUtils::clock = 0;

void TestUtils::tick(unsigned long ms) {
    clock += ms;
}

void TestUtils::tickFastForward(unsigned long ms) {
    tick(ms); 
}
