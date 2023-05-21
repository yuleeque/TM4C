#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"  /* two prev define "uint32_t"s and "bool"s occurred in sysctl.h */

#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

#include "inc/tm4c123gh6pm.h"


int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

//    // PortF: SW1, SW2 in, RGB LED out
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
//    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
//
//    // Port B: all 8 pins out
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xFF);

    int32_t SW1_state = 0;
    int32_t SW2_state = 0;
    int32_t wait; // meaningless variable to use for small delays

    //GPIOIntRegisterPin(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4, void (*pfnIntHandler)(void));
    SYSCTL_RCGC2_R |= 0x00000020;
      wait = SYSCTL_RCGC2_R;
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;
    GPIO_PORTF_AMSEL_R = 0x00;
    GPIO_PORTF_PCTL_R = 0x00000000;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_AFSEL_R = 0x00;
    GPIO_PORTF_PUR_R = 0x11;
    GPIO_PORTF_DEN_R = 0x1F;


    while(1){
        SW1_state = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
        SW2_state = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1);;

        if (SW1_state == 1 || SW2_state == 1) {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x08);

        }
        else {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x04);

        }
    }

}
