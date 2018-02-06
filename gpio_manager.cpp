#include "gpio_manager.h"


void Gpio_Manager::Gpio_Init()
{
  pinMode(LED_BUILTIN, OUTPUT);
  LED_OFF();
}

