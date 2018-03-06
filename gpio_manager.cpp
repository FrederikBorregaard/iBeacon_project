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
/* ======================== global variables ========================== */
/* ==================================================================== */
/* Gpio handler */
Gpio_Manager gpio;

/* ==================================================================== */
/* ============================ functions ============================= */
/* ==================================================================== */

/*
 * This function initializes GPIOs
 */
void Gpio_Manager::Gpio_Init()
{
  for(uint8_t pin_id = 0; pin_id < GPIO_REMOTE_USED; pin_id++) 
  { 
    /* Set used GPIO's as output */
    pinMode(GpioPin[pin_id], OUTPUT);
  }  
}

/*
 * This prints current GPIO's state
 */
void Gpio_Manager::Gpio_DebugPrint()
{
  int pin_state;
  
  for(uint8_t pin_id = 0; pin_id < GPIO_REMOTE_USED; pin_id++)
  {
    Serial.printf("GPIO -> "); 
    Serial.print(GpioPin[pin_id]); 
    Serial.printf(": "); 

    /* Read pin state */
    pin_state = digitalRead(GpioPin[pin_id]);
    
    if(0 == pin_state)
    {
      Serial.printf("OFF\r\n"); 
    }
    else
    {
      Serial.printf("ON\r\n"); 
    }
  }
}

/* EOF */
