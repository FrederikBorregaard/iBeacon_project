/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       snsr_manager.cpp
 */

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include "snsr_manager.h"

/* ==================================================================== */
/* ======================== global variables ========================== */
/* ==================================================================== */
/* Sensor handler */
Sensor sensor;

/* Sensor values structure handler */
Sensor::Sensor_Values_T sens_val;

/* Server Manager handler */
extern Server_Manager server;

/* ==================================================================== */
/* ============================ functions ============================= */
/* ==================================================================== */

/*
 * Sensor_Init
 *  - This function configures and initializes BME280 and ADC light sensor
 *  - Configured with suggested parameters for indoor monitoring
 */
bool Sensor::Sensor_Init()
{
  bool sensor_init_ok = sensor.begin();

  /* Print BME280 I2C address in hex */
  Serial.printf("SENSOR -> BME280 I2C addr: 0x%.2X\n", BME280_ADDRESS);
  
  /* Init values structure */
  sens_val.temperature = 0.0F;
  sens_val.pressure = 0.0F;
  sens_val.humidity = 0.0F;
  
  if(sensor_init_ok) 
  {
    /* 
     * Indoor Navigation Scenario
     * 
     *  - normal mode 
     *  - 16x pressure 
     *  - 2x temperature
     *  - 1x humidity oversampling
     *  - 0.5ms standby period
     *  - filter 16x
     */
     sensor.setSampling(Adafruit_BME280::MODE_NORMAL,
                        Adafruit_BME280::SAMPLING_X2,     /* temperature */
                        Adafruit_BME280::SAMPLING_X16,    /* pressure */
                        Adafruit_BME280::SAMPLING_X1,     /* humidity */
                        Adafruit_BME280::FILTER_X16,
                        Adafruit_BME280::STANDBY_MS_0_5);

    Serial.println("SENSOR -> Init OK");
  }
  else
  {
    Serial.println("SENSOR -> Init ERROR");
  }
  return sensor_init_ok;
}


/*
 * Sensor_UpdateValues
 *  - This function updates the Sensor_Values_T structure
 *  - Remember that BME280 performs measurement every ~40ms (25Hz)
 */
void Sensor::Sensor_UpdateValues()
{ 
  String temp_status;
  String pres_status;
  String humid_status;
  String light_status;
  
  sens_val.temperature = sensor.readTemperature();
  sens_val.pressure = sensor.readPressure() / 100.0F;
  sens_val.humidity = sensor.readHumidity();
  sens_val.light = analogRead(SENSOR_ANALOG_PIN);

  /* Temperature NaN value check */
  if(sens_val.temperature != sens_val.temperature)
  {
    temp_status = "ERROR";
  }
  else
  {
    temp_status = "OK";
  }

  /* Pressure NaN value check */
  if( (sens_val.pressure != sens_val.pressure) || sens_val.pressure <= 0)
  {
    pres_status = "ERROR";
  }
  else
  {
    pres_status = "OK";
  }

  /* Humidity NaN value check */
  if( (sens_val.humidity != sens_val.humidity) || sens_val.humidity <= 0)
  {
    humid_status = "ERROR";
  }
  else
  {
    humid_status = "OK";
  }

  /* ADC sensor - light status */
  light_status = "OK";
  
  server.Server_Update_SensorsState(temp_status, pres_status, humid_status, light_status);
}


/*
 * Sensor_DebugPrint
 *  - This function prints measured values on console
 */
void Sensor::Sensor_DebugPrint()
{
  Serial.printf("SENSOR -> TEMP: %.2f\n", sens_val.temperature);
  Serial.printf("SENSOR -> PRES: %.2f\n", sens_val.pressure);
  Serial.printf("SENSOR -> HUMI: %.2f\n", sens_val.humidity);
  Serial.printf("SENSOR -> LIGHT: %d\n", sens_val.light);
}

/* EOF */
