#include "USART0.h"

io::USART0::USART0(const transMode& ar_transMode,
                   const comMode& ar_comMode,
                   const frameSize& ar_frameSize,
                   const frameSync& ar_frameSync,
                   const parityMode& ar_parityMode)
                   : mr_transMode(ar_transMode),
                     mr_comMode(ar_comMode),
                     mr_parityMode(ar_parityMode),
                     mr_frameSize(ar_frameSize),
                     mr_frameSync(ar_frameSync)
{




}

io::USART0::~USART0()
{

}
