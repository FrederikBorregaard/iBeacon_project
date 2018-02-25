/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       snsr_manager.h
 */
#ifndef _SNSR_MANAGER_H_
#define _SNSR_MANAGER_H_

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "server_manager.h"

/* ==================================================================== */
/* ============================ classes =============================== */
/* ==================================================================== */
/* ADC pin with light sensor connected */
#define SENSOR_ANALOG_PIN   (A0)

/* ==================================================================== */
/* ============================ classes =============================== */
/* ==================================================================== */
class Sensor
{
  private:
    Adafruit_BME280 sensor;
        
  public:
    bool Sensor_Init();
    void Sensor_UpdateValues();
    void Sensor_DebugPrint();
    
    typedef struct Sensor_Values_Tag
    {
      float temperature;
      float pressure;
      float humidity;
      int light;
    }Sensor_Values_T;
};

#endif /* _SNSR_MANAGER_H_ */

/* EOF */
