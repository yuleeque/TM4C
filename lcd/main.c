#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"   /* two prev define "uint32_t"s and "bool"s occurred in sysctl.h */
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include <lcd.h>                // empty for now


#define delay1 10000
#define RS GPIO_PIN_0              // 0x00000001  // GPIO pin 0
#define RW GPIO_PIN_1              // 0x00000002  // GPIO pin 1
#define EN GPIO_PIN_2              // 0x00000004  // GPIO pin 2
//#define GPIO_PIN_3              // 0x00000008  // GPIO pin 3
#define D4 GPIO_PIN_4              // 0x00000010  // GPIO pin 4
#define D5 GPIO_PIN_5              // 0x00000020  // GPIO pin 5
#define D6 GPIO_PIN_6              // 0x00000040  // GPIO pin 6
#define D7 GPIO_PIN_7              // 0x00000080  // GPIO pin 7


void PortF_init(void){
    //TODO: add short notes from datasheet for each line to better see and navigate.
    //Init PortF
    //Register 60: General-Purpose Input/Output Run Mode Clock Gating Control (RCGCGPIO), offset 0x608
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;    // 5 bits in total for Ports: F(5), E(4), D(3), C(2), B(1), A(0)
    SysCtlDelay(delay1);
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
}


void PortB_init(void){
    //Init PortB  (PB7 is ON but not used yet. Disable when all done.)
    //Register 60: General-Purpose Input/Output Run Mode Clock Gating Control (RCGCGPIO), offset 0x608
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    SysCtlDelay(delay1);
    GPIO_PORTB_LOCK_R = 0x4C4F434B;     //Register 19: GPIO Lock (GPIOLOCK), offset 0x520
    GPIO_PORTB_CR_R = 0x7F;             //Register 20: GPIO Commit (GPIOCR), offset 0x524
                                        //  1==The corresponding GPIOAFSEL, GPIOPUR, GPIOPDR, or GPIODEN bits can be written.
    GPIO_PORTB_AMSEL_R = 0x80;          //Register 21: GPIO Analog Mode Select (GPIOAMSEL), offset 0x528
    GPIO_PORTB_PCTL_R = 0x00000000;     //Register 22: GPIO Port Control (GPIOPCTL), offset 0x52C
    GPIO_PORTB_DIR_R = 0xFF;            //Register 2: GPIO Direction (GPIODIR), offset 0x400
    GPIO_PORTB_AFSEL_R = 0x00;          //Register 10: GPIO Alternate Function Select (GPIOAFSEL), offset 0x420
    GPIO_PORTB_PUR_R = 0xFF;//or 0x7F   //Register 15: GPIO Pull-Up Select (GPIOPUR), offset 0x510
    GPIO_PORTB_DEN_R = 0xFF;            //Register 18: GPIO Digital Enable (GPIODEN), offset 0x51C
    SysCtlDelay(delay1);
}


/******************************************************************************
 * TODO: extend to epecify any Port later
 * */
int LCD_cmd(uint8_t pins, uint8_t command){

    GPIOPinWrite(GPIO_PORTB_BASE, pins,  command ); // /*unused GPIO_PIN_3 here between D4 and EN*/
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x04 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x00 );
    SysCtlDelay(delay1);

    return 0;
}


/******************************************************************************
 * uint32_t symbol consists of 8 bits, formed of 4 MSB and 4 LSB
 * Example: letter 'H' is encoded as (see datasheet)
 *  0100 for D7-D4: 0x4
 *  1000 for D3-D0: 0x8
 *
 */
int LCD_write(uint8_t pins, uint8_t symbol){
    //       D7|D6|D5|D4|x|EN|RW|RS
    uint8_t MSB =  symbol & 0xF0;
    uint8_t LSB = (symbol & 0x0F) << 4;
    LCD_cmd( D7|D6|D5|D4        |RS,  MSB|0x01 );    // Also, set RS to 1 (data mode)
    LCD_cmd( D7|D6|D5|D4        |RS,  LSB|0x01 );

    return 0;
}


/******************************************************************************
 *
 * An internal reset circuit automatically initializes the HD44780U when the
 *  power is turned on.
 * When MCU was reset but LCD remains powered, init by instruction is needed.
 *
 * Cold Power-On Reset
 * HD44780U is initialized by the internal reset circuit.
 *
 * */
int LCD_init(){
//RS 0 inst, 1 data
//RW 0 write, 1 read

    // Warm POR (Power-On Reset).
    // HD44780U is initialized by instruction.
    SysCtlDelay(delay1*10); // 4.1 ms
    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0x30 );
    // 100 us (microseconds)
    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0x30 );


    // Cold Power-On Reset.
    // HD44780U is initialized by the internal reset circuit.
/* Function set */
    //       D7|D6|D5|D4|x|EN|RW|RS
    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0x20 );     //This instruction is performed as 8-bit. // RS: 0 for instructions, 1 for data

/* Now, LCD executes each 2nd command */
/* Function set */
    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0x20 );     //Set MSB 4 bits.
    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0x00 );     //Set LSB 4 bits.

/* Display on */
    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0x00 );
    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0xE0 );

/* Entry mode on */
    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0x00 );
    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0x60 );

/* Cursor or display shift*/
//    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0x10 );
//    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0xC0 );


/* Clear display */
    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0x00 );
    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0x10 );

/* Return home */
    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0x00 );
    LCD_cmd( D7|D6|D5|D4  |EN|RW|RS,  0x20 );
    return 0;
}


int main(void)
{
    int SW1_state = 0;
    int SW2_state = 0;

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    PortF_init();
    PortB_init();

    LCD_init();

    LCD_write ( D7|D6|D5|D4,  0x48);
    LCD_write ( D7|D6|D5|D4,  0x4A);


    while(1){
        SW1_state = GPIOPinRead( GPIO_PORTF_BASE, GPIO_PIN_0 );
        SW2_state = GPIOPinRead( GPIO_PORTF_BASE, GPIO_PIN_1 );;

        if (SW1_state == 1 || SW2_state == 1) {
            GPIOPinWrite( GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x08 );

        }
        else {
            GPIOPinWrite( GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x04 );

        }
    }

}
