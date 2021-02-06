// tests NDS square root IO registers
#include "../inc.h"

struct SQRTtest
{
    u32 lo, hi, res;
};

const SQRTtest sqrt32_tests[] = {
    {0, 0, 0},
    {1, 0, 1},
    {3, 0, 1},
    {4, 0, 2},
    {5, 0, 2},
    {143, 18, 11},
    {144, -1, 12},
    {145, 444, 12},
    {1<<30, -33, 1<<15},
    {4294836224, 555, 65534},
    {4294836225, 555, 65535},
    {4294836226, 555, 65535},
    {4294967295, 555, 65535}
};

const SQRTtest sqrt64_tests[] = {
    {0, 0, 0},
    {1, 0, 1},
    {3, 0, 1},
    {4, 0, 2},
    {5, 0, 2},
    {143, 0, 11},
    {144, 0, 12},
    {145, 0, 12},
    {1<<30, 0, 1<<15},
    {4294836224, 0, 65534},
    {4294836225, 0, 65535},
    {4294836226, 0, 65535},
    {4294967295, 0, 65535},
    {0, 1, 65536},
    {1, 1, 65536},
    {0, 2, 92681},
    {1111, 2, 92681},
    {999999, 2, 92687},
    {0, 1<<28, 1<<30},
    {1, 1<<28, 1<<30},
    {-1, (1<<28)-1, (1<<30)-1},
    {-1, -1, -1}
};




void check_sqrt(const SQRTtest array[], int test_count)
{
    for(int i = 0; i < test_count; i++)
    {
        REG_SQRTLO = array[i].lo;
        REG_SQRTHI = array[i].hi;

        wait_bit_value(i, SQRTCNT, 15, false); // wait for bit 15 = busy bit to be turned off

        u32 res = REG_SQRTRES;
        if (res != array[i].res) fail_test(i);
    }
}

void run_tests_ds_maths_sqrt_32()
{
    REG_SQRTCNT = 0; // 32-bit mode
    check_sqrt(sqrt32_tests, 13);
}

void run_tests_ds_maths_sqrt_64()
{
    REG_SQRTCNT = 1; // 64-bit mode
    check_sqrt(sqrt64_tests, 22);
}
