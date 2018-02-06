#ifndef _GPIO_H_
#define _GPIO_H_

#include <Arduino.h>

#define LED_ON()                    (digitalWrite(LED_BUILTIN, LOW))
#define LED_OFF()                   (digitalWrite(LED_BUILTIN, HIGH))

class Gpio_Manager
{
  public:
    void Gpio_Init();
};


#endif /* _GPIO_H_ */
