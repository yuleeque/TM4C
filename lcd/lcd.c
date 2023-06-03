#include <stdbool.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include <lcd.h>

// Not part of this lib: maybe use SW buttons to control LCD
void PortF_init(void){
    // 10.3 Initialization and Configuration
    //TODO: add short notes from datasheet for each line to better see and navigate
    // datasheet: 10.3 Initialization and Configuration

    //Init PortF
    //Register 60: General-Purpose Input/Output Run Mode Clock Gating Control (RCGCGPIO), offset 0x608
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;    // 5 bits in total for Ports: F(5), E(4), D(3), C(2), B(1), A(0)

    GPIO_PORTF_DIR_R = 0x0E;            //Register 2: GPIO Direction (GPIODIR), offset 0x400

    // 10.2.4 Commit Control
    GPIO_PORTF_CR_R = 0x1F;             //Register 20: GPIO Commit (GPIOCR), offset 0x524  // figure out exact pins need it
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     //Register 19: GPIO Lock (GPIOLOCK), offset 0x520

    GPIO_PORTF_AFSEL_R = 0x00;          //Register 10: GPIO Alternate Function Select (GPIOAFSEL), offset 0x420
    GPIO_PORTF_PCTL_R = 0x00000000;     //Register 22: GPIO Port Control (GPIOPCTL), offset 0x52C

    // skip setting the drive strength for each pin? (step 4)

    GPIO_PORTF_PUR_R = 0x11;            //Register 15: GPIO Pull-Up Select (GPIOPUR), offset 0x510

    GPIO_PORTF_DEN_R = 0xFF;            //Register 18: GPIO Digital Enable (GPIODEN), offset 0x51C

    GPIO_PORTF_AMSEL_R = 0x00;          //Register 21: GPIO Analog Mode Select (GPIOAMSEL), offset 0x528
}


void PortB_init(void){
    //Init PortB  (PB3 is ON but not used at the moment)
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    GPIO_PORTB_DIR_R = 0xF7;
    GPIO_PORTB_CR_R = 0xF7;
    GPIO_PORTB_LOCK_R = 0x4C4F434B;
    GPIO_PORTB_AFSEL_R = 0x00;
    GPIO_PORTB_PCTL_R = 0x00000000;
    GPIO_PORTB_PUR_R = 0xF7;
    GPIO_PORTB_DEN_R = 0xF7;
    GPIO_PORTB_AMSEL_R = 0x00;
}


/* FULL TABLE *****************************************************************
 *                             RS RW | DB7 DB6 DB5 DB4  |  DB3 DB2 DB1 DB0
 * Clear display                0  0 |  0   0   0   0   |   0   0   0   1
 * Return home                  0  0 |  0   0   0   0   |   0   0   1   -
 * Entry mode set               0  0 |  0   0   0   0   |   0   1  I/D  S
 * Display on/off control       0  0 |  0   0   0   0   |   1   D   C   B
 * Cursor/display shift         0  0 |  0   0   0   1   |  S/C R/L  -   -
 * Function set                 0  0 |  0   0   1   DL  |   N   F   -   -
 * Set CGRAM address            0  0 |  0   1  ACG ACG  |  ACG ACG ACG ACG
 * Set DDRAM address            0  0 |  1  ADD ADD ADD  |  ADD ADD ADD ADD
 * Read busy flag & address     0  1 |  BF  AC  AC  AC  |   AC  AC  AC  AC
 * Write data to CG or DDRAM    1  0 | <......... write | data ..........>
 * Read data from CG or DDRAM   1  1 | <......... read  | data ..........>
 *
 * I/D = 1: Increment, I/D = 0: Decrement
 * S = 1: Accompanies display shift, S = 0: No shift
 *
 * D = 1: Display on
 * C = 1: Cursor on
 * B = 1: Blink on
 *
 * S/C = 1: Display shift, S/C = 0: Cursor move
 * R/L = 1: Shift to the right, R/L = 0: Shift to the left
 *
 * DL = 1: 8 bits, DL = 0: 4 bits
 * N = 1: 2 lines, N = 0: 1 line
 * F = 1: 5 x 10 dots, F = 0: 5 x 8 dots
 *
 * BF = 1: Internally operating, BF = 0: Instructions acceptable
 *
 * Notes:
 *      DDRAM: Display data RAM
 *      CGRAM: Character generator RAM
 *      ACG: CGRAM address
 *      ADD: DDRAM address (corresponds to cursor address)
 *      AC: Address counter used for both DD and CGRAM addresses
 *      Execution time changes when frequency changes
 *      Example: When fCP or fOSC is 250 kHz:  37us * 270/250 = 40us
 *
 *              PIN    DB7 DB6 DB5 DB4 -x- EN RS RW
 *              bit     7   6   5   4   3   2  1  0
 *****************************************************************************/
int lcd(uint16_t data){

    // set Read-or-Write and Instruction-or-Data
    GPIOPinWrite(GPIO_PORTB_BASE, RS|RW,        data >> 8 );

    // MSB
    GPIOPinWrite(GPIO_PORTB_BASE, D7|D6|D5|D4,  data & 0x0F0);
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x04 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x00 );
    SysCtlDelay(delay1);

    // LSB
    GPIOPinWrite(GPIO_PORTB_BASE, D7|D6|D5|D4, (data & 0x00F) << 4 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x04 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x00 );
    SysCtlDelay(delay1);
    return 0;
}

// Used at the beginning due to default 8-bit interface
int lcd_8bit(uint16_t data){
    GPIOPinWrite(GPIO_PORTB_BASE, RS|RW,        data >> 8 );
    // MSB and LSB simultaneously but DB3-0 are Don't Cares so just D7-4
    GPIOPinWrite(GPIO_PORTB_BASE, D7|D6|D5|D4,  data & 0x0F );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x04 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x00 );
    SysCtlDelay(delay1);
    return 0;
}



/******************************************************************************
 * An internal reset circuit automatically initializes LCD when power is on.
 * Still, it's much better to initialize it explicitly by instruction:
 * "If the power supply conditions for correctly operating the internal reset
 *  circuit are not met, initialization by instructions becomes necessary."
 * */
int LCD_init(){
    // Interface is 8 bit long by default.
    lcd_8bit( cmdWrite 0x30 );
    lcd_8bit( cmdWrite 0x30 );
    lcd_8bit( cmdWrite 0x30 );
    lcd_8bit( cmdWrite 0x20 );

    // Interface is 4 bits long now.
    lcd( cmdWrite 0x20 );    // Set 4-bit operation, 1-line display, 5x8 font
    lcd( cmdWrite 0x0F );    // Turn on/off control
    lcd( cmdWrite 0x01 );    // Display clear
    lcd( cmdWrite 0x06 );    // Entry mode set

//TODO the foolowing actually should be in different functions like LCD_clear, LCD_setcursor etc.
/* Cursor or display shift*/
    //lcd( 0x10 );
    //lcd( 0xC0 );

/* Return home */
    //lcd( 0x00 );
    //lcd( 0x20 );
    return 0;
}
