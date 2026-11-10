/**
 * @file PCF8574_LCD.h
 * @brief Header file for PCF8574 I2C LCD1602/LCD2004 character display driver
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#ifndef PCF8574_LCD_H
#define PCF8574_LCD_H

#include "ha_base.h"
#include "TWI.h"

namespace component
{

class PCF8574_LCD
{
public:
    /** Constructor
     *
     *  @param a_i2cAddress I2C 7-bit bus address (default 0x27 or 0x3F)
     *  @param a_cols Number of display columns (e.g. 16 or 20)
     *  @param a_rows Number of display rows (e.g. 2 or 4)
     */
    PCF8574_LCD(uint8_t a_i2cAddress = 0x27, uint8_t a_cols = 16, uint8_t a_rows = 2);

    /** Initialize LCD in 4-bit mode over I2C
     */
    void init();

    /** Clear LCD screen and reset cursor
     */
    void clear();

    /** Set cursor position
     *
     *  @param a_col Column index (0..cols-1)
     *  @param a_row Row index (0..rows-1)
     */
    void setCursor(uint8_t a_col, uint8_t a_row);

    /** Print string to current cursor position
     *
     *  @param ap_str Null-terminated text string
     */
    void print(const char *ap_str);

    /** Toggle backlight on/off
     */
    void setBacklight(uint8_t a_enable);

private:
    void sendNibble(uint8_t a_nibble, uint8_t a_mode);
    void sendByte(uint8_t a_byte, uint8_t a_mode);

    uint8_t m_i2cAddr;
    uint8_t m_cols;
    uint8_t m_rows;
    uint8_t m_backlightMask;
    core::TWI &mr_twi;
};

}

#endif
