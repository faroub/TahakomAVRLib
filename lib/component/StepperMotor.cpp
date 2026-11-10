#include "StepperMotor.h"
#include <math.h>

// TODO: steps to angle
// TODO: maximum speed

component::StepperMotor::StepperMotor(const mode &ar_mode,
                                      const io::Pin &ar_pinCoil1,
                                      const io::Pin &ar_pinCoil2,
                                      const io::Pin &ar_pinCoil3,
                                      const io::Pin &ar_pinCoil4)
    : m_pinCoil1(ar_pinCoil1),
      m_pinCoil2(ar_pinCoil2),
      m_pinCoil3(ar_pinCoil3),
      m_pinCoil4(ar_pinCoil4),
      stepMode(ar_mode),
      m_goalReached(0),
      m_stepNumber(0),
      m_targetStep(0),
      m_currentPos(0),
      m_profileTime(0),
      m_profileSpeed(0),
      m_speedTimeProduct(0),
      m_profilePosition(0)
{

    m_pinCoil1.toOutput();
    m_pinCoil2.toOutput();
    m_pinCoil3.toOutput();
    m_pinCoil4.toOutput();

}

component::StepperMotor::~StepperMotor()
{

}

void component::StepperMotor::beginMove(const int16_t a_step)
{
    m_stepNumber = 0;
    m_targetStep = a_step;
    m_profileTime = 0;
    m_profileSpeed = 0;
    m_speedTimeProduct = 0;
    m_profilePosition = 0;
    m_goalReached = (a_step == 0) ? 1 : 0;
}

void component::StepperMotor::setCurrentPos(uint16_t a_currentPos)
{
    m_currentPos = a_currentPos;
}

uint16_t component::StepperMotor::currentPos()
{
    return m_currentPos;
}


void component::StepperMotor::step(const int16_t a_step,
        const uint16_t a_speed,
        const uint16_t a_accel,
        const uint16_t a_decel)
{
    uint16_t l_stepDelay_ms = 0;
    uint16_t l_step=0;

    // a different target, or a call after the goal was reached, starts
    // a new move and resets the profile state of the previous move
    if (m_goalReached || (a_step != m_targetStep))
    {
        beginMove(a_step);
    }

    if (a_step<=0)
    {
       l_step = -a_step;
    } else
    {
        l_step = a_step;
    }

    l_stepDelay_ms = computeStepDelay(l_step,
                                      a_speed,
                                      a_accel,
                                      a_decel);

    if (l_stepDelay_ms>0)
    {

    if (a_step<=0)
    {
        m_stepNumber++;
        m_currentPos++;
        if (m_stepNumber == -a_step)
        {
            m_goalReached = 1;
        }

    } else {

        m_stepNumber--;
        m_currentPos--;
        if (m_stepNumber == -a_step)
        {
            m_goalReached = 1;
        }
    }


    switch (stepMode)
    {
        case mode::fullStep:
        {
            // equivalent to m_stepNumber % 4
            // 1 3 5 7 <->
            stepPulse(2*(m_stepNumber & 3)+1);
            break;
        }

        case mode::halfStep:
        {
            // equivalent to m_stepNumber % 8
            // 0 1 2 3 4 5 6 7 <->
            stepPulse(m_stepNumber & 7);
            break;
        }
    }

    stepDelay(l_stepDelay_ms);

    }
    else
    {
        // no step fired this tick: keep the 1 ms polling cadence
        stepDelay(1);
    }

}

uint16_t component::StepperMotor::computeStepDelay(uint16_t a_step,
                       const uint16_t a_speed,
                       const uint16_t a_accel,
                       const uint16_t a_decel)
{

   // a profile cannot be driven by a zero speed, acceleration or
   // deceleration: fire no step and avoid the division by zero
   if ((a_speed == 0) || (a_accel == 0) || (a_decel == 0))
   {
       return 0;
   }

   uint32_t l_accelTime= (1000UL*a_speed/a_accel);
   uint32_t l_decelTime = (1000UL*a_speed/a_decel);
   uint16_t l_speed = a_speed;   // rescaled when the ramp is shortened
   // signed and 32-bit wide: the constant-speed phase can be negative
   // before the ramp correction below, and 1000*steps/step/s overflows
   // int16 for any move longer than a few steps
   int32_t l_constSpeedTime = static_cast<int32_t>(1000UL*a_step/a_speed)
                              - static_cast<int32_t>(l_accelTime/2UL)
                              - static_cast<int32_t>(l_decelTime/2UL);




   if (l_constSpeedTime<0)
   {
       l_constSpeedTime=0;
       // sqrtf of the float ratio: the old integer division
       // a_step/a_accel truncated to 0 for any realistic ramp
       l_accelTime = static_cast<uint32_t>(1000UL*sqrtf(static_cast<float>(a_step)
                                                      / static_cast<float>(a_accel)));
       l_speed=static_cast<uint16_t>(l_accelTime*a_accel/1000UL);
       l_decelTime = 1000UL*l_speed/a_decel;

   }




   if (m_profileTime<=l_accelTime)
   {
       m_profileSpeed=a_accel*m_profileTime;

   }
   else if ((m_profileTime>l_accelTime) && (m_profileTime<=(l_accelTime+l_constSpeedTime)))
   {
        m_profileSpeed=1000UL*l_speed;

   }
   else if ((m_profileTime>(l_accelTime+l_constSpeedTime))&&(m_profileTime<=l_accelTime+l_constSpeedTime+l_decelTime))
   {
       m_profileSpeed= (1000UL*l_speed)-a_decel*(m_profileTime-l_accelTime-static_cast<uint32_t>(l_constSpeedTime));


   }


   m_profileTime=m_profileTime+1;


   m_speedTimeProduct = m_speedTimeProduct+m_profileSpeed;

   if (m_speedTimeProduct - m_profilePosition >= 1000000UL)
   {
       m_profilePosition=m_profilePosition+1000000UL;
       return 1;
   }
   else
   {
       return 0;
   }



 
    
}


void component::StepperMotor::step(const int16_t a_step, const uint16_t a_speed)
{
    // a different target, or a call after the goal was reached, starts a
    // new move; the step delay is recomputed on each call instead of
    // being cached once from the first call
    if (m_goalReached || (a_step != m_targetStep))
    {
        beginMove(a_step);
    }
    uint16_t l_stepDelay_ms = static_cast<uint16_t>(1000UL/a_speed);

    if (a_step<=0)
    {
        m_stepNumber++;
        m_currentPos++;
        if (m_stepNumber == -a_step)
        {
            m_goalReached = 1;
        }

    } else {

        m_stepNumber--;
        m_currentPos--;
        if (m_stepNumber == -a_step)
        {
            m_goalReached = 1;
        }
    }

    switch (stepMode)
    {
        case mode::fullStep:
        {
            // equivalent to m_stepNumber % 4
            // 1 3 5 7 <->
            stepPulse(2*(m_stepNumber & 3)+1);
            break;
        }

        case mode::halfStep:
        {
            // equivalent to m_stepNumber % 8
            // 0 1 2 3 4 5 6 7 <->
            stepPulse(m_stepNumber & 7);
            break;
        }
    }

    stepDelay(l_stepDelay_ms);
}

void component::StepperMotor::step(const int16_t a_step, const uint16_t a_speed, const float a_stepAngle)
{
    // a different target, or a call after the goal was reached, starts a
    // new move; the step delay is recomputed on each call instead of
    // being cached once from the first call
    if (m_goalReached || (a_step != m_targetStep))
    {
        beginMove(a_step);
    }
    uint16_t l_stepDelay_ms = static_cast<uint16_t>(1000UL*a_stepAngle/a_speed);

    if (a_step<=0)
    {
        m_stepNumber++;
        m_currentPos++;
        if (m_stepNumber == -a_step)
        {
            m_goalReached = 1;
        }

    } else {

        m_stepNumber--;
        m_currentPos--;
        if (m_stepNumber == -a_step)
        {
            m_goalReached = 1;
        }
    }

    switch (stepMode)
    {
        case mode::fullStep:
        {
            // equivalent to m_stepNumber % 4
            // 1 3 5 7 <->
            stepPulse(2*(m_stepNumber & 3)+1);
            break;
        }

        case mode::halfStep:
        {
            // equivalent to m_stepNumber % 8
            // 0 1 2 3 4 5 6 7 <->
            stepPulse(m_stepNumber & 7);
            break;
        }
    }

    stepDelay(l_stepDelay_ms);
}


void component::StepperMotor::stepPulse(const uint8_t a_stepPulse)
{

    switch (a_stepPulse) {
      case 0:    // 1000
      {
        m_pinCoil1.setHigh();
        m_pinCoil2.setLow();
        m_pinCoil3.setLow();
        m_pinCoil4.setLow();
        break;
      }
      case 1:    // 1100
      {
        m_pinCoil1.setHigh();
        m_pinCoil2.setHigh();
        m_pinCoil3.setLow();
        m_pinCoil4.setLow();
        break;
      }
      case 2:    //0100
      {
        m_pinCoil1.setLow();
        m_pinCoil2.setHigh();
        m_pinCoil3.setLow();
        m_pinCoil4.setLow();
        break;
      }
      case 3:    //0110
      {
        m_pinCoil1.setLow();
        m_pinCoil2.setHigh();
        m_pinCoil3.setHigh();
        m_pinCoil4.setLow();
        break;
      }
      case 4:    //0010
      {
        m_pinCoil1.setLow();
        m_pinCoil2.setLow();
        m_pinCoil3.setHigh();
        m_pinCoil4.setLow();
        break;
      }
      case 5:    //0011
      {
        m_pinCoil1.setLow();
        m_pinCoil2.setLow();
        m_pinCoil3.setHigh();
        m_pinCoil4.setHigh();
        break;
      }
      case 6:    //0001
      {
        m_pinCoil1.setLow();
        m_pinCoil2.setLow();
        m_pinCoil3.setLow();
        m_pinCoil4.setHigh();
        break;
      }
      case 7:    //1001
      {
        m_pinCoil1.setHigh();
        m_pinCoil2.setLow();
        m_pinCoil3.setLow();
        m_pinCoil4.setHigh();
        break;
      }
    }


}



void component::StepperMotor::stepDelay(uint16_t a_stepDelay)
{
    while(a_stepDelay--)
    {
        _delay_ms(1);
    }
}

uint8_t component::StepperMotor::goalReached()
{
    return m_goalReached;

}
