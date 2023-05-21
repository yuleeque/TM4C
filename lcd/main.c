#include <lcd.h>                // empty for now

#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"   /* two prev define "uint32_t"s and "bool"s occurred in sysctl.h */

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

    int SW1_state = 0;
    int SW2_state = 0;

    //TODO: add short noted from datasheet for each line to better see and navigate.
    //Init PortF
    //Register 60: General-Purpose Input/Output Run Mode Clock Gating Control (RCGCGPIO), offset 0x608
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;    // 5 bits in total for Ports: F(5), E(4), D(3), C(2), B(1), A(0)
    SysCtlDelay(2000);
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     //Register 19: GPIO Lock (GPIOLOCK), offset 0x520
    GPIO_PORTF_CR_R = 0x1F;             //Register 20: GPIO Commit (GPIOCR), offset 0x524
                                        //  1==The corresponding GPIOAFSEL, GPIOPUR, GPIOPDR, or GPIODEN bits can be written.
    GPIO_PORTF_AMSEL_R = 0x00;          //Register 21: GPIO Analog Mode Select (GPIOAMSEL), offset 0x528
    GPIO_PORTF_PCTL_R = 0x00000000;     //Register 22: GPIO Port Control (GPIOPCTL), offset 0x52C
    GPIO_PORTF_DIR_R = 0x0E;            //Register 2: GPIO Direction (GPIODIR), offset 0x400
    // GPIO_PORTF_CR_R now allows to write to following 3 registers.
    GPIO_PORTF_AFSEL_R = 0x00;          //Register 10: GPIO Alternate Function Select (GPIOAFSEL), offset 0x420
    GPIO_PORTF_PUR_R = 0x11;            //Register 15: GPIO Pull-Up Select (GPIOPUR), offset 0x510
    GPIO_PORTF_DEN_R = 0x1F;            //Register 18: GPIO Digital Enable (GPIODEN), offset 0x51C


    //Init PortB  (PB7 is ON but not used yet. Disable when all done.)
    //Register 60: General-Purpose Input/Output Run Mode Clock Gating Control (RCGCGPIO), offset 0x608
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    SysCtlDelay(2000);
    GPIO_PORTB_LOCK_R = 0x4C4F434B;     //Register 19: GPIO Lock (GPIOLOCK), offset 0x520
    GPIO_PORTB_CR_R = 0x7F;             //Register 20: GPIO Commit (GPIOCR), offset 0x524
                                        //  1==The corresponding GPIOAFSEL, GPIOPUR, GPIOPDR, or GPIODEN bits can be written.
    GPIO_PORTB_AMSEL_R = 0x80;          //Register 21: GPIO Analog Mode Select (GPIOAMSEL), offset 0x528
    GPIO_PORTB_PCTL_R = 0x00000000;     //Register 22: GPIO Port Control (GPIOPCTL), offset 0x52C
    GPIO_PORTB_DIR_R = 0x7F;            //Register 2: GPIO Direction (GPIODIR), offset 0x400
    GPIO_PORTB_AFSEL_R = 0x00;          //Register 10: GPIO Alternate Function Select (GPIOAFSEL), offset 0x420
    GPIO_PORTB_PUR_R = 0x80;//or 0x7F            //Register 15: GPIO Pull-Up Select (GPIOPUR), offset 0x510
    GPIO_PORTB_DEN_R = 0x7F;            //Register 18: GPIO Digital Enable (GPIODEN), offset 0x51C


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
