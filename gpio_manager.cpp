/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       gpio_manager.cpp
 */

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include "gpio_manager.h"

/* ==================================================================== */
/* ============================ functions ============================= */
/* ==================================================================== */

/*
 * This function initializes GPIOs
 */
void Gpio_Manager::Gpio_Init()
{
  for(uint8_t gpio_id = 0; gpio_id < GPIO_REMOTE_USED + 1; gpio_id++) 
  { 
    /* Set used GPIO's as output */
    pinMode(GpioPin[gpio_id], OUTPUT);
  }  
}

/* EOF */
