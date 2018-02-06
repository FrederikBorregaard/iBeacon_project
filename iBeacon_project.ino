/*
 *  @Author:          Jakub Witowski 
 *  @Creation date:   06.02.2018 
 *  @Project name:    WebSocketServer
 * 
 * 
 *  Functionality:
 *  
 *    - Implemented basic command line interface
 *      - Serial Baud Rate
 *      - Full logging implemented
 *      - "login", and "reboot" command
 *      
 *    - Implemented WiFi AP connection
 *      - SSID and PASSWORD are stored in EEPROM
 *      - change credentials during the runtime using "login" command
 *      
 *    - Reboot support
 *      - Reboot after configurable timeout when cannot connect to the AP
 *      - Reboot after configurable timeout when lost WiFi connection and cannot restore it
 *      
 *    - Implemented WebSocketSerwer
 *      - Configurable PORT and IP nr: PORT @@81
 *      - Receiving JSON package: JSON input buffer
 *      - Supported commands in JSON format:
 *        - "{"LED1" : 1}"
 *        - "{"LED1" : 0}"
 *        
 *        
 *    Configurable parameters:
 *      - Serial Baud Rate: 115200
 *      - Establishing connection timeout: 16000ms
 *      - Trying reconnect timeout: 20000ms
 *      - WebSocketServer Port: 81
 *      - JSON input buffer size: 200bytes
 */
 
/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include <Arduino.h>

#include "web_socket_event.h"
#include "serial_event.h"
#include "wifi_manager.h"
#include "nvm_manager.h"
#include "gpio_manager.h"
#include "tmr_config.h"

/* ==================================================================== */
/* ============================ constants ============================= */
/* ==================================================================== */         

/* ==================================================================== */
/* ======================== global variables ========================== */
/* ==================================================================== */
/* Establish connection timer related objects */
Ticker timer_establish_connection;
Ticker timer_reconnect;

/* Other handlers */
Nvm_Manager eeprom;
WiFi_Manager wifi;
Gpio_Manager gpio;
Serial_Event serial_e;

/* ==================================================================== */
/* ==================== function prototypes =========================== */
/* ==================================================================== */
inline void establish_connection_timeout_wrapper();
inline void reconnect_failed_timeout_wrapper();

/* ==================================================================== */
/* ============================ functions ============================= */
/* ==================================================================== */

/*  setup()
      - This function is called when the program starts
*/
void setup()
{  
  Serial.println("REBOOT -> OK");
  
  eeprom.Nvm_Init();
  gpio.Gpio_Init();

  /* Initialize timers */
  Start_est_connection_tmr(TMR_ESTABLISH_CONNECTION_TIMEOUT_MS);
  
  wifi.WiFi_Connect();
  Serial.println("WIFI -> Setup complete");
}

/*  loop()
      - This function is called periodically at the runtime
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
    webSocketLoop();
  }
  serial_e.Serial_RxEvent();
}

/*  reconnect_failed_timeout_wrapper()
      - This wrapper is called on timer_reconnect timers timeout event
*/
inline void reconnect_failed_timeout_wrapper()
{
  wifi.WiFi_reconnect_failed_timeout_event();
}

/*  reconnect_failed_timeout_wrapper()
      - This wrapper is called on timer_establish_connection timers timeout event
*/
inline void establish_connection_timeout_wrapper()
{
  wifi.WiFi_establish_connection_timeout_event();
}

/*
 *  Timer related functions
 */
void Start_est_connection_tmr(uint16_t tmout)
{
  timer_establish_connection.once_ms(tmout, establish_connection_timeout_wrapper);
}

void Start_reconnect_tmr(uint16_t tmout)
{
  timer_reconnect.attach_ms(tmout, reconnect_failed_timeout_wrapper);
}

void Stop_reconnect_tmr()
{
  timer_reconnect.detach();
}


