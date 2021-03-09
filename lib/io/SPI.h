/**
 * @file SPI.h
 * @brief Header file of the SPI class
 *
 * Usage example (test):
 #include "MCU.h"
 #include "SPI.h"

 #define SPI_SCK 5
 #define SPI_MISO 4
 #define SPI_MOSI 3
 #define SPI_SS 2


 int main(void) {

   // Init
   // initialize MCU
   core::MCU::init();
   // instantiate a SPI object
   io::SPI &myISP = io::SPI::getInstance(io::Pin(SPI_SCK,io::PortB),
                                         io::Pin(SPI_MISO,io::PortB),
                                         io::Pin(SPI_MOSI,io::PortB),
                                         io::Pin(SPI_SS,io::PortB));

   myISP.selectClockPrescaler(io::clockPrescaler::PS_128);
   myISP.selectDataMode(io::dataMode::mode_0);
   myISP.selectDataOrder(io::dataOrder::first_LSB);
   myISP.selectOperationMode(io::operationMode::master);

   myISP.masterSendByte(0x03);

   // Mainloop
   while (1) {



   }
   return 0;
 }
 *
 *
 * Basic class for IO abstraction of Pin and Port
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#ifndef SPI_H
#define SPI_H
#include "ha_base.h"
#include "Pin.h"

namespace io
{

enum class operationMode : uint8_t {
    master=0,    /**<  */
    slave,       /**<  */
    submaster, /**<  */
    disable, /**<  */
};

enum class clockPrescaler : uint8_t  {
     PS_4 = 0,
     PS_16,
     PS_64,
     PS_128,
     PS_2,
     PS_8,
     PS_32
};

enum class dataMode : uint8_t  {
     mode_0 = 0,
     mode_1,
     mode_2,
     mode_3,
};

enum class dataOrder : uint8_t  {
     first_MSB = 0,
     first_LSB
};


class SPI
{

public:
    static SPI& getInstance(const io::Pin &ar_pinSCK,
                            const io::Pin &ar_pinMISO,
                            const io::Pin &ar_pinMOSI,
                            const io::Pin &ar_pinSS);

    void selectDataMode(const dataMode& ar_dataMode);

    void selectDataOrder(const dataOrder& ar_dataOrder);

    void selectOperationMode(const operationMode& ar_operationMode);

    void selectClockPrescaler(const clockPrescaler& ar_clockPrescaler);

    void selectSlave(const uint8_t a_select);

    /** Is there write collision.
         */
    uint8_t writeCollision();

    /** Is serial transfer complete.
         */
    uint8_t transferComplete();


    static void enableTransferCompleteInterrupt(const uint8_t a_enable);

    void masterSendByte(const uint8_t &ar_byte);

    void masterReceiveByte(uint8_t &ar_byte);

    void slaveReceiveByte(uint8_t &ar_byte);

    /** Serial transfer complete ISR.
         */
    static void transferCompleteServiceRoutine() __asm__(STR(SPI_TRANSFER_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));



private:
    /** Constructor. Initalizes the USART0 object
         *
         *  @param ar_transMode defines transmission mode
         *  @param ar_comMode defines communication mode
         *  @param ar_frameSize defines data frame size
         *  @param ar_stopBit defines number of stop bits
         *  @param ar_parityMode defines parity mode
         */
    SPI(const io::Pin &ar_pinSCK,
        const io::Pin &ar_pinMISO,
        const io::Pin &ar_pinMOSI,
        const io::Pin &ar_pinSS);

    /** Destructor.
        */
    ~SPI();

    /** Overried Copy constructor.
        */
    SPI(const SPI&);

    /** Override assign operator.
        */
    const SPI& operator=(const SPI&);

    static volatile uint8_t m_data;

    io::Pin m_pinSCK; /**< pin object */
    io::Pin m_pinMISO; /**< pin object */
    io::Pin m_pinMOSI; /**< pin object */
    io::Pin m_pinSS; /**< pin object */







};

}
#endif // SPI_H
