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


/******************************************************************************
 * Instructions table this function executes (Table 6):
 *                            DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0    time (fOSC = 270kHz)
 * Clear display                0   0   0   0   0   0   0   1      1.52 ms
 * Return home                  0   0   0   0   0   0   1   -      37 us
 * Entry mode set               0   0   0   0   0   1  I/D  S      37 us
 * Display on/off control       0   0   0   0   1   D   C   B      37 us
 * Cursor/display shift         0   0   0   1  S/C R/L  -   -      37 us
 * Function set                 0   0   1   DL  N   F   -   -      37 us
 * Set CGRAM address            0   1  ACG ACG ACG ACG ACG ACG     37 us
 * Set DDRAM address            1  ADD ADD ADD ADD ADD ADD ADD     37 us
 *
 * I/D = 1: Increment
 * I/D = 0: Decrement
 * S = 1: Accompanies display shift
 * S/C = 1: Display shift
 * S/C = 0: Cursor move
 * R/L = 1: Shift to the right
 * R/L = 0: Shift to the left
 * DL = 1: 8 bits
 * DL = 0: 4 bits
 * N = 1: 2 lines
 * N = 0: 1 line
 * F = 1: 5 x 10 dots
 * F = 0: 5 x 8 dots
 *
 * Notes:
 *      DDRAM: Display data RAM
 *      CGRAM: Character generator RAM
 *      ACG: CGRAM address
 *      ADD: DDRAM address (corresponds to cursor address)
 *      Execution time changes when frequency changes
 *      Example: When fCP or fOSC is 250 kHz:  37us * 270/250 = 40us
 *
 *   PIN    DB7 DB6 DB5 DB4 -x- EN RW RS
 *   bit     7   6   5   4   3   2  1  0
 *****************************************************************************/
int LCD_cmd(uint8_t data){
    uint8_t MSB =  data & 0xF0;
    uint8_t LSB = (data & 0x0F) << 4;

    // MSB
    GPIOPinWrite(GPIO_PORTB_BASE, D7|D6|D5|D4|RS,  MSB|0x00 );  // RS to 0 (instruction mode), RW to 0 (write)
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x04 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x00 );
    SysCtlDelay(delay1);

    // LSB
    GPIOPinWrite(GPIO_PORTB_BASE, D7|D6|D5|D4|RS,  LSB|0x00 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x04 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x00 );
    SysCtlDelay(delay1);
    return 0;
}

// Used at the beginning due to default 8-bit interface
int LCD_cmd_8bit(uint8_t data){
    GPIOPinWrite(GPIO_PORTB_BASE, D7|D6|D5|D4|RS,  data|0x00 ); // RS to 0 (instruction mode), RW to 0 (write)
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x04 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x00 );
    SysCtlDelay(delay1);
    return 0;
}

/******************************************************************************
 * Instruction (the only one) this function executes (Table 6):
 *                            DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0    time (fOSC = 270kHz)
 * Read busy flag & address    BF  AC AC AC AC AC AC AC AC AC      0 us
 *
 * BF = 1: Internally operating
 * BF = 0: Instructions acceptable
 * AC: Address counter used for both DD and CGRAM addresses
 *
 * Execution time changes when frequency changes
 * Example: When fCP or fOSC is 250 kHz:  37us * 270/250 = 40us
 *
 *   PIN    DB7 DB6 DB5 DB4 -x- EN RW RS
 *   bit     7   6   5   4   3   2  1  0
 *****************************************************************************/
int LCD_isBusy(uint8_t data){

    return 0;
}


/******************************************************************************
 * Instructions table this function executes (Table 6):
 *                             DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0    time (fOSC = 270kHz)
 * Write data to CG or DDRAM   <..........write data.........>     37 us
 *
 * Notes:
 *      DDRAM: Display data RAM
 *      CGRAM: Character generator RAM
 *      ACG: CGRAM address
 *      ADD: DDRAM address (corresponds to cursor address)
 *      Execution time changes when frequency changes
 *      Example: When fCP or fOSC is 250 kHz:  37us * 270/250 = 40us
 *
 *   PIN    DB7 DB6 DB5 DB4 -x- EN RW RS
 *   bit     7   6   5   4   3   2  1  0
 *****************************************************************************/
int LCD_write(uint8_t data){
    uint8_t MSB =  data & 0xF0;
    uint8_t LSB = (data & 0x0F) << 4;

    GPIOPinWrite(GPIO_PORTB_BASE, D7|D6|D5|D4|RS,  MSB|0x01 );  // RS to 1 (data mode), RW to 0 (write)
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x04 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x00 );
    SysCtlDelay(delay1);

    GPIOPinWrite(GPIO_PORTB_BASE, D7|D6|D5|D4|RS,  LSB|0x01 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x04 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x00 );
    SysCtlDelay(delay1);
    return 0;
}

/******************************************************************************
 * Instructions table this function executes (Table 6):
 * Use DB7..DB0 to form an argument you need.
 *                               DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0    time (fOSC = 270kHz)
 * Read data from CG or DDRAM    <..........read data..........>     37 us
 *
 * Notes:
 *      DDRAM: Display data RAM
 *      CGRAM: Character generator RAM
 *      Execution time changes when frequency changes
 *      Example: When fCP or fOSC is 250 kHz:  37us * 270/250 = 40us
 *
 *   PIN    DB7 DB6 DB5 DB4 -x- EN RW RS
 *   bit     7   6   5   4   3   2  1  0
 *****************************************************************************/
int LCD_read(uint8_t data){
    uint8_t MSB =  data & 0xF0;
    uint8_t LSB = (data & 0x0F) << 4;

    GPIOPinWrite(GPIO_PORTB_BASE, D7|D6|D5|D4|RS,  MSB|0x03 );  // RS to 1 (data mode), RW to 1 (read)
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x04 );
    SysCtlDelay(delay1);
    GPIOPinWrite(GPIO_PORTB_BASE, EN,  0x00 );
    SysCtlDelay(delay1);

    GPIOPinWrite(GPIO_PORTB_BASE, D7|D6|D5|D4|RS,  LSB|0x03 );
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
    LCD_cmd_8bit( 0x30 );
    LCD_cmd_8bit( 0x30 );
    LCD_cmd_8bit( 0x30 );
    LCD_cmd_8bit( 0x20 );

    // Interface is 4 bits long now.
    LCD_cmd( 0x20 );    // Set 4-bit operation, 1-line display, 5x8 font
    LCD_cmd( 0x0F );    // Turn on/off control
//    LCD_cmd( 0x0B );    // Turn on/off control
    LCD_cmd( 0x01 );    // Display clear
    LCD_cmd( 0x06 );    // Entry mode set

//TODO the foolowing actually should be in different functions like LCD_clear, LCD_setcursor etc.
/* Cursor or display shift*/
    //LCD_cmd( 0x10 );
    //LCD_cmd( 0xC0 );

/* Return home */
    //LCD_cmd( 0x00 );
    //LCD_cmd( 0x20 );
    return 0;
}
