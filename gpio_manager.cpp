#include "gpio_manager.h"


void Gpio_Manager::Gpio_Init()
{
  for(uint8_t gpio_id = 0; gpio_id < GPIO_REMOTE_USED + 1; gpio_id++) 
  { 
    pinMode(GpioPin[gpio_id],OUTPUT);
  }  
}

