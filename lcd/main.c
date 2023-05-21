#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"  /* two prev define "uint32_t"s and "bool"s occurred in sysctl.h */




int main(void)
{

    //SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);  // 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //set sys clock to 80 MHz
    return 0;
}
