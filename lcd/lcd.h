#ifndef LCD_
#define LCD_

#define delay1 10000

#define RW GPIO_PIN_0              // 0x00000001  // GPIO pin 0
#define RS GPIO_PIN_1              // 0x00000002  // GPIO pin 1
#define EN GPIO_PIN_2              // 0x00000004  // GPIO pin 2
//#define PIN3 GPIO_PIN_3                 // 0x00000008  // GPIO pin 3 (not used)
#define D4 GPIO_PIN_4              // 0x00000010  // GPIO pin 4
#define D5 GPIO_PIN_5              // 0x00000020  // GPIO pin 5
#define D6 GPIO_PIN_6              // 0x00000040  // GPIO pin 6
#define D7 GPIO_PIN_7              // 0x00000080  // GPIO pin 7


#define cmdWrite    0x000 |
#define cmdRead     0x100 |
#define dataWrite   0x200 |
#define dataRead    0x300 |


/* finding more-or-less consistent function names
 *
 * void lcd_clear(void)
 * void lcd_home(void)
 * void lcd_entry_mode_left(int steps)
 * void lcd_entry_mode_right(int steps)
 * void lcd_display_on(bool)
 * void lcd_cursor_on(bool)
 * void lcd_cursor_blink(bool)
 * void lcd_cursor_move_left(int steps)  -->      \
 * void lcd_cursor_move_right(int steps) -->       \-- to be wrapped into lcd_setCursor or lcd_cursor_set
 *                                             \-- OR... keep the original two and add lcd_move_up, lcd_move_down
 * void lcd_display_shift_left(int steps)
 * void lcd_display_shift_right(int steps)
 * void lcd_settings(int interfaceDataLength,
 *              int numOfLines,
 *              int charFont)
 * void lcd_CGRAM_set(6 bit wide arg)
 * void lcd_DDRAM_set(7 bit wide arg)
 *      lcd_RAM_read(void)
 *      lcd_RAM_write(void)
 *      lcd_busyFlag_read(void)
 */


#define CLEAR_DISPLAY 0x01
#define DISPLAY_OFF 0x08


void PortF_init(void);
void PortB_init(void);
int lcd(uint16_t data);
int lcd_8bit(uint16_t data);
int LCD_init();

#endif




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
 *      PIN    DB7 DB6 DB5 DB4 -x- EN RW RS
 *      bit     7   6   5   4   3   2  1  0
 *****************************************************************************/
