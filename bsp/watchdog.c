#include "ti_msp_dl_config.h"
#include "watchdog.h"

void Watchdog_Init(void)
{
    DL_WWDT_enablePower(WWDT0);

    /*
     * 32.768 kHz LFCLK / 4 = 8192 Hz.
     * 2^15 counts gives a 4 second timeout before a SYSRST.
     */
    DL_WWDT_initWatchdogMode(WWDT0,
        DL_WWDT_CLOCK_DIVIDE_4,
        DL_WWDT_TIMER_PERIOD_15_BITS,
        DL_WWDT_RUN_IN_SLEEP,
        DL_WWDT_WINDOW_PERIOD_0,
        DL_WWDT_WINDOW_PERIOD_0);

    DL_WWDT_setActiveWindow(WWDT0, DL_WWDT_WINDOW0);
    DL_WWDT_setCoreHaltBehavior(WWDT0, DL_WWDT_CORE_HALT_STOP);
}

void Watchdog_Feed(void)
{
    DL_WWDT_restart(WWDT0);
}
