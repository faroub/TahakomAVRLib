/**
 * @file SSD1306_OLED.cpp
 * @brief Implementation of SSD1306 128x64 / 128x32 I2C OLED display driver
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "SSD1306_OLED.h"

// 5x7 ASCII Font Table subset
static const uint8_t font5x7[][5] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00 }, // space
    { 0x00, 0x00, 0x5F, 0x00, 0x00 }, // !
    { 0x00, 0x07, 0x00, 0x07, 0x00 }, // "
    { 0x14, 0x7F, 0x14, 0x7F, 0x14 }, // #
    { 0x24, 0x2A, 0x7F, 0x2A, 0x12 }, // $
    { 0x23, 0x13, 0x08, 0x64, 0x62 }, // %
    { 0x36, 0x49, 0x55, 0x22, 0x50 }, // &
    { 0x00, 0x05, 0x03, 0x00, 0x00 }, // '
    { 0x00, 0x1C, 0x22, 0x41, 0x00 }, // (
    { 0x00, 0x41, 0x22, 0x1C, 0x00 }, // )
    { 0x14, 0x08, 0x3E, 0x08, 0x14 }, // *
    { 0x08, 0x08, 0x3E, 0x08, 0x08 }, // +
    { 0x00, 0x50, 0x30, 0x00, 0x00 }, // ,
    { 0x08, 0x08, 0x08, 0x08, 0x08 }, // -
    { 0x00, 0x60, 0x60, 0x00, 0x00 }, // .
    { 0x20, 0x10, 0x08, 0x04, 0x02 }, // /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E }, // 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00 }, // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 }, // 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31 }, // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10 }, // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 }, // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 }, // 6
    { 0x01, 0x71, 0x09, 0x05, 0x03 }, // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 }, // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E }, // 9
};

component::SSD1306_OLED::SSD1306_OLED(uint8_t a_i2cAddress, uint8_t a_width, uint8_t a_height)
    : m_i2cAddr(a_i2cAddress),
      m_width(a_width),
      m_height(a_height),
      mr_twi(core::TWI::getInstance(400000UL))
{
}

void component::SSD1306_OLED::init()
{
    _delay_ms(10);
    sendCommand(0xAE); // Display OFF
    sendCommand(0xD5); sendCommand(0x80); // Set Display Clock Divide Ratio
    sendCommand(0xA8); sendCommand(m_height - 1); // Set Multiplex Ratio
    sendCommand(0xD3); sendCommand(0x00); // Display Offset
    sendCommand(0x40); // Start Line 0
    sendCommand(0x8D); sendCommand(0x14); // Enable Charge Pump
    sendCommand(0x20); sendCommand(0x00); // Memory Addressing Mode: Horizontal
    sendCommand(0xA1); // Segment Re-map
    sendCommand(0xC8); // COM Output Scan Direction
    sendCommand(0xDA); sendCommand((m_height == 64) ? 0x12 : 0x02); // COM Pins Configuration
    sendCommand(0x81); sendCommand(0xCF); // Set Contrast Control
    sendCommand(0xD9); sendCommand(0xF1); // Pre-charge Period
    sendCommand(0xDB); sendCommand(0x40); // VCOMH Deselect Level
    sendCommand(0xA4); // Entire Display ON
    sendCommand(0xA6); // Normal Display
    sendCommand(0xAF); // Display ON
    clear();
}

void component::SSD1306_OLED::clear()
{
    for (uint8_t page = 0; page < (m_height / 8); page++) {
        setCursor(0, page);
        mr_twi.start();
        mr_twi.write(m_i2cAddr << 1);
        mr_twi.write(0x40); // Data mode
        for (uint8_t col = 0; col < m_width; col++) {
            mr_twi.write(0x00);
        }
        mr_twi.stop();
    }
    setCursor(0, 0);
}

void component::SSD1306_OLED::setCursor(uint8_t a_col, uint8_t a_page)
{
    sendCommand(0xB0 | (a_page & 0x07));
    sendCommand(0x00 | (a_col & 0x0F));
    sendCommand(0x10 | ((a_col >> 4) & 0x0F));
}

void component::SSD1306_OLED::print(const char *ap_str)
{
    if (!ap_str) return;
    while (*ap_str) {
        char c = *ap_str++;
        uint8_t idx = (c >= ' ' && c <= '9') ? (c - ' ') : 0;
        mr_twi.start();
        mr_twi.write(m_i2cAddr << 1);
        mr_twi.write(0x40);
        for (uint8_t i = 0; i < 5; i++) {
            mr_twi.write(font5x7[idx][i]);
        }
        mr_twi.write(0x00); // char spacing
        mr_twi.stop();
    }
}

void component::SSD1306_OLED::sendCommand(uint8_t a_cmd)
{
    mr_twi.start();
    mr_twi.write(m_i2cAddr << 1);
    mr_twi.write(0x00); // Command mode
    mr_twi.write(a_cmd);
    mr_twi.stop();
}

void component::SSD1306_OLED::sendData(uint8_t a_data)
{
    mr_twi.start();
    mr_twi.write(m_i2cAddr << 1);
    mr_twi.write(0x40); // Data mode
    mr_twi.write(a_data);
    mr_twi.stop();
}
