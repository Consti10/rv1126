#ifndef __EMMC_TEST_H_
#define __EMMC_TEST_H_
#include "rk_pcba_test_led.h"
#ifdef PCBA_PX3SE
    #define EMMC_CAPACITY 8
#endif
void *emmc_test(void *argv);

#endif
