#pragma once

namespace TestUtils
{
    extern unsigned long clock;

    void tick(unsigned long ms = 1);
    void tickFastForward(unsigned long ms);
}