#include "StepperMotor.h"
#include <math.h>

// TODO: steps to angle
// TODO: maximum speed
// TODO: speed and accelleration profiles

component::StepperMotor::StepperMotor(const mode &ar_mode,
                                      const io::Pin &ar_pinCoil1,
                                      const io::Pin &ar_pinCoil2,
                                      const io::Pin &ar_pinCoil3,
                                      const io::Pin &ar_pinCoil4)
    : m_pinCoil1(ar_pinCoil1),
      m_pinCoil2(ar_pinCoil2),
      m_pinCoil3(ar_pinCoil3),
      m_pinCoil4(ar_pinCoil4),
      stepMode(ar_mode)
{

    m_pinCoil1.toOutput();
    m_pinCoil2.toOutput();
    m_pinCoil3.toOutput();
    m_pinCoil4.toOutput();
    m_goalReached = 0;
    m_currentPos = 0;


}

component::StepperMotor::~StepperMotor()
{

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
    static int16_t l_stepNumber = 0;
    uint8_t l_stepDelay_ms = 0;
    uint16_t l_step=0;

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
        if (l_stepNumber== -a_step)
        {
            m_goalReached = 1;

        } else {
            l_stepNumber++;
            m_currentPos++;
        }


    } else {

        if (l_stepNumber == -a_step)
        {
            m_goalReached = 1;

        } else {
            l_stepNumber--;
            m_currentPos--;

        }
    }


    if (m_goalReached == 0)
    {

        switch (stepMode)
        {
            case mode::fullStep:
            {
                // equivalent to l_stepNumber % 4
                // 1 3 5 7 <->
                stepPulse(2*(l_stepNumber & 3)+1);
                break;
            }

            case mode::halfStep:
            {
                // equivalent to l_stepNumber % 8
                // 0 1 2 3 4 5 6 7 <->
                stepPulse(l_stepNumber & 7);
                break;
            }
        }

        stepDelay(l_stepDelay_ms);

    }
}

}

uint8_t component::StepperMotor::computeStepDelay(uint16_t a_step,
                       uint16_t a_speed,
                       uint16_t a_accel,
                       uint16_t a_decel)
{


   uint32_t l_accelTime= (1000UL*a_speed/a_accel);
   uint16_t l_decelTime = (1000UL*a_speed/a_decel);
   int16_t l_constSpeedTime=((1000UL*a_step/a_speed)-(l_accelTime/2)-(l_decelTime/2));
   static uint32_t l_time=0;
   static uint32_t l_speed=0;
   static uint64_t l_speed_time_product=0;
   static uint64_t l_current_position=0;







   if (l_constSpeedTime<0)
   {
       l_constSpeedTime=0;
       l_accelTime = static_cast<uint32_t>(1000UL*sqrtf(a_step/a_accel));
       a_speed=l_accelTime*a_accel/1000UL;
       l_decelTime = 1000UL*a_speed/a_decel;

   }




   if (l_time<=l_accelTime)
   {
       l_speed=a_accel*l_time;

   }
   else if ((l_time>l_accelTime) && (l_time<=(l_accelTime+l_constSpeedTime)))
   {
        l_speed=1000UL*a_speed;

   }
   else if ((l_time>(l_accelTime+l_constSpeedTime))&&(l_time<=l_accelTime+l_constSpeedTime+l_decelTime))
   {
       l_speed= (1000UL*a_speed)-a_decel*(l_time-l_accelTime-l_constSpeedTime);


   }


   l_time=l_time+1;


   l_speed_time_product = l_speed_time_product+l_speed;

   if (l_speed_time_product - l_current_position >= 1000000UL)
   {
       l_current_position=l_current_position+1000000UL;
       return 1;
   }
   else
   {
       stepDelay(1);
       return 0;
   }



 
    
}


void component::StepperMotor::step(const int16_t a_step, const uint16_t a_speed)
{
    static int16_t l_stepNumber = 0;
    static uint8_t l_stepDelay_ms = static_cast<uint8_t>(1000UL/a_speed);

    if (a_step<=0)
    {
        if (l_stepNumber== -a_step)
        {
            m_goalReached = 1;

        } else {
            l_stepNumber++;
            m_currentPos++;
        }


    } else {

        if (l_stepNumber == -a_step)
        {
            m_goalReached = 1;

        } else {
            l_stepNumber--;
            m_currentPos--;

        }
    }


    if (m_goalReached == 0)
    {

        switch (stepMode)
        {
            case mode::fullStep:
            {
                // equivalent to l_stepNumber % 4
                // 1 3 5 7 <->
                stepPulse(2*(l_stepNumber & 3)+1);
                break;
            }

            case mode::halfStep:
            {
                // equivalent to l_stepNumber % 8
                // 0 1 2 3 4 5 6 7 <->
                stepPulse(l_stepNumber & 7);
                break;
            }
        }

        stepDelay(l_stepDelay_ms);

    }
}

void component::StepperMotor::step(const int16_t a_step, const uint16_t a_speed, const float a_stepAngle)
{
    static int16_t l_stepNumber = 0;
    static uint8_t l_stepDelay_ms = static_cast<uint8_t>(1000UL*a_stepAngle/a_speed);

    if (a_step<=0)
    {
        if (l_stepNumber== -a_step)
        {
            m_goalReached = 1;

        } else {
            l_stepNumber++;
            m_currentPos++;
        }


    } else {

        if (l_stepNumber == -a_step)
        {
            m_goalReached = 1;

        } else {
            l_stepNumber--;
            m_currentPos--;

        }
    }


    if (m_goalReached == 0)
    {

        switch (stepMode)
        {
            case mode::fullStep:
            {
                // equivalent to l_stepNumber % 4
                // 1 3 5 7 <->
                stepPulse(2*(l_stepNumber & 3)+1);
                break;
            }

            case mode::halfStep:
            {
                // equivalent to l_stepNumber % 8
                // 0 1 2 3 4 5 6 7 <->
                stepPulse(l_stepNumber & 7);
                break;
            }
        }

        stepDelay(l_stepDelay_ms);

    }


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



void component::StepperMotor::stepDelay(uint8_t a_stepDelay)
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
