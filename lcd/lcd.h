#ifndef LCD_
#define LCD_

#define delay1 10000

#define RS GPIO_PIN_0              // 0x00000001  // GPIO pin 0
#define RW GPIO_PIN_1              // 0x00000002  // GPIO pin 1
#define EN GPIO_PIN_2              // 0x00000004  // GPIO pin 2
//#define PIN3 GPIO_PIN_3                 // 0x00000008  // GPIO pin 3 (not used)
#define D4 GPIO_PIN_4              // 0x00000010  // GPIO pin 4
#define D5 GPIO_PIN_5              // 0x00000020  // GPIO pin 5
#define D6 GPIO_PIN_6              // 0x00000040  // GPIO pin 6
#define D7 GPIO_PIN_7              // 0x00000080  // GPIO pin 7





#define CLEAR_DISPLAY 0x01
#define DISPLAY_OFF 0x08




void PortF_init(void);
void PortB_init(void);
int LCD_cmd(uint8_t data);
int LCD_cmd_8bit(uint8_t data);
int LCD_write(uint8_t data);
int LCD_isBusy(uint8_t data);
int LCD_init();

#endif



// FULL TABLE (to keep as a copy just in case)
/******************************************************************************
 * Instructions table LCD_cmd() follows (Table 6).
 * Skip RS and RW, they are already set in corresponding functions.
 * Use DB7..DB0 to form an argument you need.
 *                           RS RW | DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0    time (fOSC = 270kHz)
 * Clear display              0  0 |  0   0   0   0   0   0   0   1      1.52 ms
 * Return home                0  0 |  0   0   0   0   0   0   1   -      37 us
 * Entry mode set             0  0 |  0   0   0   0   0   1  I/D  S      37 us
 * Display on/off control     0  0 |  0   0   0   0   1   D   C   B      37 us
 * Cursor/display shift       0  0 |  0   0   0   1  S/C R/L  -   -      37 us
 * Function set               0  0 |  0   0   1   DL  N   F   -   -      37 us
 * Set CGRAM address          0  0 |  0   1  ACG ACG ACG ACG ACG ACG     37 us
 * Set DDRAM address          0  0 |  1  ADD ADD ADD ADD ADD ADD ADD     37 us
 * Read busy flag & address   0  1 |  BF  AC AC AC AC AC AC AC AC AC      0 us
 * Write data to CG or DDRAM  1  0 | <..........write data.........>     37 us
 * Read data from CG or DDRAM 1  1 | <..........read data..........>     37 us
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
 * BF = 1: Internally operating
 * BF = 0: Instructions acceptable
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
 *   PIN    DB7 DB6 DB5 DB4 -x- EN RW RS
 *   bit     7   6   5   4   3   2  1  0
 *****************************************************************************/
