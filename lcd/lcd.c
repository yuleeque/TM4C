#include <stdbool.h>            // "bool" for sysctl.h, gpio.h
#include <stdint.h>             // "uint?_t" for sysctl.h, gpio.h
#include "inc/tm4c123gh6pm.h"   // Reg defines for PortF_init(), PortB_init()
#include "inc/hw_memmap.h"      // PIO_PORTB_BASE for the basic lcd(), lcd_8bit()
#include "driverlib/sysctl.h"   // SysCtlDelay(), which is to be replaced by SysTick()
#include "driverlib/gpio.h"     // GPIO_PIN_? defines
#include <lcd.h>

void PortB_init(void){
    // Datasheet -- 10.3 Initialization and Configuration
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    GPIO_PORTB_DIR_R = 0xF7;
    GPIO_PORTB_CR_R = 0xF7;
    GPIO_PORTB_LOCK_R = 0x4C4F434B;
    GPIO_PORTB_AFSEL_R = 0x00;
    GPIO_PORTB_PCTL_R = 0x00000000;
    // skip setting the drive strength for each pin? (step 4)
    GPIO_PORTB_PUR_R = 0xF7;
    GPIO_PORTB_DEN_R = 0xF7;
    GPIO_PORTB_AMSEL_R = 0x00;
}

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
    GPIOPinWrite(GPIO_PORTB_BASE, D7|D6|D5|D4,  data & 0x0F0 );
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
    return 0;
}
