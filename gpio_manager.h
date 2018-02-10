#ifndef _GPIO_H_
#define _GPIO_H_

#include <Arduino.h>

#define LED_ON()                    (digitalWrite(LED_BUILTIN, LOW))
#define LED_OFF()                   (digitalWrite(LED_BUILTIN, HIGH))

#define GPIO_REMOTE_USED 2



const uint8_t GpioPin[GPIO_REMOTE_USED] = {D4 ,D5}; 



class Gpio_Manager
{
  public:
    void Gpio_Init();
};


#endif /* _GPIO_H_ */
