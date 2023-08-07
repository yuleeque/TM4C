#include <stdbool.h>            // "bool" for sysctl.h, gpio.h
#include <stdint.h>             // "uint?_t" for sysctl.h, gpio.h
#include "driverlib/sysctl.h"   // SysCtlClockSet(), SysCtlClockGet()
#include <lcd.h>

#define CLEAR 0x1

uint32_t ui32ClockFrequency;


int main(void)
{

    // datasheet 5.2.5, Table 5-4
    SysCtlClockSet( SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ );
    PortB_init();
    ui32ClockFrequency = SysCtlClockGet();
    LCD_init();
    lcd( dataWrite 'B' );
    lcd( dataWrite 'Y' );
    lcd( dataWrite 'E' );
    lcd( cmdWrite CLEAR );
    lcd( dataWrite 'O' );
    lcd( dataWrite 'K' );
    lcd( dataWrite 'E' );
    lcd( dataWrite 'Y' );

    return 0;
}
