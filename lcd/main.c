#include <lcd.h>                // empty for now


#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"   /* two prev define "uint32_t"s and "bool"s occurred in sysctl.h */

#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

#include "inc/tm4c123gh6pm.h"

#define delay1 10000


#define RS GPIO_PIN_0              // 0x00000001  // GPIO pin 0
#define RW GPIO_PIN_1              // 0x00000002  // GPIO pin 1
#define EN GPIO_PIN_2              // 0x00000004  // GPIO pin 2
//#define GPIO_PIN_3              // 0x00000008  // GPIO pin 3
#define D4 GPIO_PIN_4              // 0x00000010  // GPIO pin 4
#define D5 GPIO_PIN_5              // 0x00000020  // GPIO pin 5
#define D6 GPIO_PIN_6              // 0x00000040  // GPIO pin 6
#define D7 GPIO_PIN_7              // 0x00000080  // GPIO pin 7


/*TODO: extend to epecify any Port later*/
int LCD_cmd(uint32_t pins, uint32_t command){

    GPIOPinWrite(GPIO_PORTB_BASE, pins,  command ); // /*unused GPIO_PIN_3 here between D4 and EN*/
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x04 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x00 );
    SysCtlDelay(delay1);

    return 0;
}



/*
 * RS 0 inst, 1 data
 * RW 0 write, 1 read
 * */

int LCD_init(){
    /*power supply on*/                         //HD44780U is initialized by the internal reset circuit.

/* Function set */
    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0x20);     //This instruction completes with one write.
/* Function set */
    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0x20);     //Set MSB 4 bits.
    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0x00);     //Set LSB 4 bits.

    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0x20);
    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0x00);

/* Cursor or display shift*/
//    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0x10);
//    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0xC0);


/* Clear display */
    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0x00);
    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0x10);

/* Return home */
    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0x00);
    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0x20);

/* Display on */
    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0x00);
    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0xF0);

/* Entry mode on */
    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0x00);
    LCD_cmd(D7|D6|D5|D4|  EN|RW|RS,  0x60);

    return 0;
}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);



    int SW1_state = 0;
    int SW2_state = 0;


    //TODO: add short noted from datasheet for each line to better see and navigate.
    //Init PortF
    //Register 60: General-Purpose Input/Output Run Mode Clock Gating Control (RCGCGPIO), offset 0x608
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;    // 5 bits in total for Ports: F(5), E(4), D(3), C(2), B(1), A(0)
    SysCtlDelay(500000);
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     //Register 19: GPIO Lock (GPIOLOCK), offset 0x520
    GPIO_PORTF_CR_R = 0x1F;             //Register 20: GPIO Commit (GPIOCR), offset 0x524
                                        //  1==The corresponding GPIOAFSEL, GPIOPUR, GPIOPDR, or GPIODEN bits can be written.
    GPIO_PORTF_AMSEL_R = 0x00;          //Register 21: GPIO Analog Mode Select (GPIOAMSEL), offset 0x528
    GPIO_PORTF_PCTL_R = 0x00000000;     //Register 22: GPIO Port Control (GPIOPCTL), offset 0x52C
    GPIO_PORTF_DIR_R = 0x0E;            //Register 2: GPIO Direction (GPIODIR), offset 0x400
    // GPIO_PORTF_CR_R now allows to write to following 3 registers.
    GPIO_PORTF_AFSEL_R = 0x00;          //Register 10: GPIO Alternate Function Select (GPIOAFSEL), offset 0x420
    GPIO_PORTF_PUR_R = 0x11;            //Register 15: GPIO Pull-Up Select (GPIOPUR), offset 0x510
    GPIO_PORTF_DEN_R = 0xFF;            //Register 18: GPIO Digital Enable (GPIODEN), offset 0x51C


    //Init PortB  (PB7 is ON but not used yet. Disable when all done.)
    //Register 60: General-Purpose Input/Output Run Mode Clock Gating Control (RCGCGPIO), offset 0x608
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    SysCtlDelay(500000);
    GPIO_PORTB_LOCK_R = 0x4C4F434B;     //Register 19: GPIO Lock (GPIOLOCK), offset 0x520
    GPIO_PORTB_CR_R = 0x7F;             //Register 20: GPIO Commit (GPIOCR), offset 0x524
                                        //  1==The corresponding GPIOAFSEL, GPIOPUR, GPIOPDR, or GPIODEN bits can be written.
    GPIO_PORTB_AMSEL_R = 0x80;          //Register 21: GPIO Analog Mode Select (GPIOAMSEL), offset 0x528
    GPIO_PORTB_PCTL_R = 0x00000000;     //Register 22: GPIO Port Control (GPIOPCTL), offset 0x52C
    GPIO_PORTB_DIR_R = 0xFF;            //Register 2: GPIO Direction (GPIODIR), offset 0x400
    GPIO_PORTB_AFSEL_R = 0x00;          //Register 10: GPIO Alternate Function Select (GPIOAFSEL), offset 0x420
    GPIO_PORTB_PUR_R = 0xFF;//or 0x7F   //Register 15: GPIO Pull-Up Select (GPIOPUR), offset 0x510
    GPIO_PORTB_DEN_R = 0xFF;            //Register 18: GPIO Digital Enable (GPIODEN), offset 0x51C
    SysCtlDelay(500000);

    //Init LCD
    LCD_init();
    GPIOPinWrite(GPIO_PORTB_BASE, RS,  0x01 ); // 0 for instructions, 1 for data
    LCD_cmd(D7|D6|D5|D4,  0x40);
    LCD_cmd(D7|D6|D5|D4,  0x80);
    LCD_cmd(D7|D6|D5|D4,  0x40);
    LCD_cmd(D7|D6|D5|D4,  0x90);




    //GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1,  0x00 );

    //GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6,  0x30 );



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
