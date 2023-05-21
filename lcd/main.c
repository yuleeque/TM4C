#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"  /* two prev define "uint32_t"s and "bool"s occurred in sysctl.h */

#include "driverlib/gpio.h"

#include "inc/hw_memmap.h"




int main(void)
{


    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);


    //initialize LCD
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF
                           );
    //GPIOPinTypeGPIOOutput(0x40005000, 0);  // GPIO Port B  base address in hw.memmap.h
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xFF);  // GPIO_PIN_1|..|GPIO_PIN_8

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x08);

//#define GPIO_PIN_0              0x00000001  // GPIO pin 0
//#define GPIO_PIN_1              0x00000002  // GPIO pin 1
//#define GPIO_PIN_2              0x00000004  // GPIO pin 2
//#define GPIO_PIN_3              0x00000008  // GPIO pin 3
//#define GPIO_PIN_4              0x00000010  // GPIO pin 4
//#define GPIO_PIN_5              0x00000020  // GPIO pin 5
//#define GPIO_PIN_6              0x00000040  // GPIO pin 6
//#define GPIO_PIN_7              0x00000080  // GPIO pin 7


    //init SW1 and SW2
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);


    //GPIOPinTypeGPIOInput(0x40025000); // Port F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);

    return 0;
}
