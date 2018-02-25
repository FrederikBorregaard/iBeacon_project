/*
 *  @Author:          Jakub Witowski 
 *  @Project name:    iBeacon
 * 
 * 
 *  Functionality:
 *  
 *    - Implemented basic command line interface
 *      - Serial Baud Rate
 *      - Full logging implemented
 *      - "ap_login", "user_login", "reboot", "raw_eeprom", "sensors" commands
 *      
 *    - Implemented WiFi AP connection
 *      - AP_SSID and AP_PASSWORD are encrypted and stored in EEPROM
 *      - change AP credentials during the runtime using "ap_login" command
 *      - change USER credentials during the runtime using "user_login" command
 *      
 *    - Reboot support
 *      - Reboot after configurable timeout when cannot connect to the AP
 *      - Reboot after configurable timeout when lost WiFi connection and cannot restore it
 *      
 *    - Login website to secure remote access
 *      - USERNAME and USER_PASSWORD are encrypted and stored in EEPROM
 *       
 *    - Implemented Sensors support
 *      - BME280 I2C address: BME280_ADDRESS 0x76
 *      - Light Sensor using ADC
 *      - Measured values
 *      
 *    Configurable parameters:
 *      - Serial Baud Rate: 115200
 *      - Establishing connection timeout: 16000ms
 *      - Trying reconnect timeout: 20000ms
 */
 
/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include <Arduino.h>
#include "serial_event.h"
#include "wifi_manager.h"
#include "nvm_manager.h"
#include "gpio_manager.h"
#include "tmr_config.h"
#include "server_manager.h"
#include "snsr_manager.h"

/* ==================================================================== */
/* ======================== global variables ========================== */
/* ==================================================================== */
/* Establish connection timer related objects */
Ticker timer_establish_connection;
Ticker timer_reconnect;
Ticker timer_new_measure;

/* Other handlers */
extern Nvm_Manager eeprom;
extern WiFi_Manager wifi;
extern Gpio_Manager gpio;
extern Serial_Event serial_e;
extern Server_Manager server;
extern Sensor sensor;

/* ==================================================================== */
/* ==================== function prototypes =========================== */
/* ==================================================================== */
inline void establish_connection_timeout_wrapper();
inline void reconnect_failed_timeout_wrapper();
inline void new_measure_timeout_wrapper();

/* ==================================================================== */
/* ============================ functions ============================= */
/* ==================================================================== */

/*  
 *  setup()
 *    - This function is called when the program starts
 */
void setup()
{  
  Serial.println("REBOOT -> OK");
  
  eeprom.Nvm_Init();
  gpio.Gpio_Init();
  (void)sensor.Sensor_Init();

  /* Start measuring timer and update status on the website */
  Start_sensor_measurement_tmr(TMR_SENSOR_MEASUREMENT_PERIOD_MS);
  server.Server_Update_SensorsState("ERROR", "ERROR", "ERROR", "ERROR");
  
  /* Initialize timers */
  Start_est_connection_tmr(TMR_ESTABLISH_CONNECTION_TIMEOUT_MS);
  
  wifi.WiFi_Connect();
  Serial.println("WIFI -> Setup complete");
}


/*  
 *   loop()
 *    - This function is called periodically at the runtime
 */
void loop()
{ 
  if(WIFI_IS_DISCONNECTED())
  {
    if(false == wifi.WiFi_get_connection_lost_flag())
    {
       /*  If reach here 
        *   -> WiFi status is NOT connected
        *   -> connection lost flag is false - set it !
        */
      Serial.println("WIFI -> Connection ERROR");

      /* Start "try reconnect" timer */
      Start_reconnect_tmr(TMR_RECONNECT_TIMEOUT_MS);
      wifi.WiFi_set_connection_lost_flag();
    }
  }
  else
  {
    if(false != wifi.WiFi_get_connection_lost_flag())
    {
       /*  If reach here 
        *   -> WiFi status is connected
        *   -> connection lost flag is true - clear it !
        */
      Serial.println("WIFI -> Connected after error");
      Serial.println("WIFI -> IP address: ");
      Serial.println(WiFi.localIP());
      
      /* Disable and reset "try reconnect" timer */
      Stop_reconnect_tmr();
      wifi.WiFi_clear_connection_lost_flag();
    }
    /* Handle server requests */
    server.Server_HandleClient();
  }
  serial_e.Serial_RxEvent();
}

/****************************************************/
/*         RECONNECT TIMER RELATED FUNCTIONS        */
/****************************************************/


/*
 *  Start_reconnect_tmr
 *    - This function starts timer_reconnect timer
 */
void Start_reconnect_tmr(uint16_t tmout)
{
  timer_reconnect.attach_ms(tmout, reconnect_failed_timeout_wrapper);
}


/*
 *  Stop_reconnect_tmr
 *    - This function stops timer_reconnect timer
 */
void Stop_reconnect_tmr()
{
  timer_reconnect.detach();
}


/*  
 *   reconnect_failed_timeout_wrapper()
 *    - This wrapper is called on timer_reconnect timers timeout event
 */
inline void reconnect_failed_timeout_wrapper()
{
  wifi.WiFi_reconnect_failed_timeout_event();
}

/****************************************************/
/*  ESTABLISH CONNECTION TIMER RELATED FUNCTIONS    */
/****************************************************/


/*
 *  Start_est_connection_tmr
 *    - This function starts timer_establish_connection timer
 */
void Start_est_connection_tmr(uint16_t tmout)
{
  timer_establish_connection.once_ms(tmout, establish_connection_timeout_wrapper);
}


/*  
 *   establish_connection_timeout_wrapper()
 *    - This wrapper is called on timer_establish_connection timers timeout event
 */
inline void establish_connection_timeout_wrapper()
{
  wifi.WiFi_establish_connection_timeout_event();
}

/****************************************************/
/*  NEW SENSOR MEASUREMENT TIMER RELATED FUNCTIONS  */
/****************************************************/

/*
 *  Start_sensor_new_measurement
 *    - This function starts timer_establish_connection timer
 */
void Start_sensor_measurement_tmr(uint16_t tmout)
{
  timer_new_measure.attach_ms(tmout, new_measure_timeout_wrapper);
}


/*  
 *   new_measure_timeout_wrapper()
 *    - This wrapper is called on timer_new_measure timeout event
 */
inline void new_measure_timeout_wrapper()
{
  sensor.Sensor_UpdateValues();
}

/* EOF */
