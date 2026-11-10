/**
 * @file PCF8574_LCD.cpp
 * @brief Implementation of PCF8574 I2C LCD1602/LCD2004 character display driver
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "PCF8574_LCD.h"

#define LCD_ENABLE_BIT  0x04
#define LCD_RS_BIT      0x01
#define LCD_BACKLIGHT   0x08

component::PCF8574_LCD::PCF8574_LCD(uint8_t a_i2cAddress, uint8_t a_cols, uint8_t a_rows)
    : m_i2cAddr(a_i2cAddress),
      m_cols(a_cols),
      m_rows(a_rows),
      m_backlightMask(LCD_BACKLIGHT),
      mr_twi(core::TWI::getInstance(100000UL))
{
}

void component::PCF8574_LCD::init()
{
    _delay_ms(50);
    // Sequence to initialize HD44780 in 4-bit mode over PCF8574
    sendNibble(0x03, 0);
    _delay_ms(5);
    sendNibble(0x03, 0);
    _delay_us(150);
    sendNibble(0x03, 0);
    sendNibble(0x02, 0); // 4-bit mode

    sendByte(0x28, 0); // 2 lines, 5x8 font
    sendByte(0x0C, 0); // Display ON, Cursor OFF
    clear();
    sendByte(0x06, 0); // Entry mode auto increment
}

void component::PCF8574_LCD::clear()
{
    sendByte(0x01, 0);
    _delay_ms(2);
}

void component::PCF8574_LCD::setCursor(uint8_t a_col, uint8_t a_row)
{
    static const uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (a_row >= m_rows) a_row = m_rows - 1;
    sendByte(0x80 | (a_col + row_offsets[a_row]), 0);
}

void component::PCF8574_LCD::print(const char *ap_str)
{
    if (!ap_str) return;
    while (*ap_str) {
        sendByte(static_cast<uint8_t>(*ap_str++), LCD_RS_BIT);
    }
}

void component::PCF8574_LCD::setBacklight(uint8_t a_enable)
{
    m_backlightMask = a_enable ? LCD_BACKLIGHT : 0x00;
    sendNibble(0x00, 0);
}

void component::PCF8574_LCD::sendNibble(uint8_t a_nibble, uint8_t a_mode)
{
    uint8_t payload = (a_nibble << 4) | a_mode | m_backlightMask;
    mr_twi.start();
    mr_twi.write(m_i2cAddr << 1);
    mr_twi.write(payload | LCD_ENABLE_BIT);
    _delay_us(1);
    mr_twi.write(payload & ~LCD_ENABLE_BIT);
    _delay_us(50);
    mr_twi.stop();
}

void component::PCF8574_LCD::sendByte(uint8_t a_byte, uint8_t a_mode)
{
    sendNibble(a_byte >> 4, a_mode);
    sendNibble(a_byte & 0x0F, a_mode);
}
