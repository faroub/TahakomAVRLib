#include "USART0.h"

io::USART0::USART0(const opMode& ar_opMode,
                   const comMode& ar_comMode,
                   const frameSize& ar_frameSize,
                   const frameSync& ar_frameSync)
                   : mr_opMode(ar_opMode),
                     mr_comMode(ar_comMode),
                     mr_frameSize(ar_frameSize),
                     mr_frameSync(ar_frameSync)
{




}

io::USART0::~USART0()
{

}
