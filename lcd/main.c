#include <stdbool.h>
#include <stdint.h>
//#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
//#include "driverlib/timer.h"
#include <lcd.h>

#define SW1     0x00000001
#define RED     0x00000002
#define BLUE    0x00000004
#define GREEN   0x00000008
#define SW2     0x00000010

#define writeData 0x200 |

uint32_t ui32ClockFrequency;


int main(void)
{

    // datasheet 5.2.5, Table 5-4
    SysCtlClockSet( SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ );
    PortF_init();
    PortB_init();
    ui32ClockFrequency = SysCtlClockGet();
    LCD_init();
    LCD_cmd( writeData 'O' );
    LCD_cmd( 0x200 | 'K' );
    //LCD_cmd( CLEAR_DISPLAY );

    // RED   GREEN  BLUE
    // 0x02  0x08   0x04
    while(1){
//        if( GPIOPinRead( GPIO_PORTF_BASE, GPIO_PIN_0 )){
//            GPIOPinWrite( GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x08 );
//        }
//        else if(GPIOPinRead( GPIO_PORTF_BASE, GPIO_PIN_4 )){
//            GPIOPinWrite( GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x04 );
//        }
//        else{
//            GPIOPinWrite( GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x02 );
//        }
        if( !GPIOPinRead( GPIO_PORTF_BASE, RED) ){
            GPIOPinWrite( GPIO_PORTF_BASE, RED|BLUE|GREEN, RED );
        }
        else if(!GPIOPinRead( GPIO_PORTF_BASE, GREEN)){
            GPIOPinWrite( GPIO_PORTF_BASE, RED|BLUE|GREEN, GREEN );
        }
        SysCtlDelay(ui32ClockFrequency / 3 / 2); // 3 cycles * N
    }
}
