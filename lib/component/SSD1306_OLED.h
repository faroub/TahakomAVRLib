/**
 * @file SSD1306_OLED.h
 * @brief Header file for SSD1306 128x64 / 128x32 I2C OLED display driver
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#ifndef SSD1306_OLED_H
#define SSD1306_OLED_H

#include "ha_base.h"
#include "TWI.h"

namespace component
{

class SSD1306_OLED
{
public:
    /** Constructor
     *
     *  @param a_i2cAddress I2C address (default 0x3C or 0x3D)
     *  @param a_width Display width in pixels (128)
     *  @param a_height Display height in pixels (64 or 32)
     */
    SSD1306_OLED(uint8_t a_i2cAddress = 0x3C, uint8_t a_width = 128, uint8_t a_height = 64);

    /** Initialize SSD1306 controller over I2C
     */
    void init();

    /** Clear display RAM
     */
    void clear();

    /** Set cursor page and column
     *
     *  @param a_col Column pixel index (0..127)
     *  @param a_page Page index (0..7 for 64px, 0..3 for 32px)
     */
    void setCursor(uint8_t a_col, uint8_t a_page);

    /** Print string to display
     *
     *  @param ap_str Text string
     */
    void print(const char *ap_str);

private:
    void sendCommand(uint8_t a_cmd);
    void sendData(uint8_t a_data);

    uint8_t m_i2cAddr;
    uint8_t m_width;
    uint8_t m_height;
    core::TWI &mr_twi;
};

}

#endif
