/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       serial_event.h
 */
#ifndef _SERIAL_EVENT_H_
#define _SERIAL_EVENT_H_

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "nvm_manager.h"
#include "tmr_config.h"

/* ==================================================================== */
/* ============================= defines ============================== */
/* ==================================================================== */
#define SERIAL_BAUDRATE             (115200)

/* ==================================================================== */
/* ============================ typedefs ============================== */
/* ==================================================================== */
typedef enum Credentials_State_Tag
{
  ap_credentials_change_request = 0,
  ap_credentials_ssid_stored,
  
  user_credentials_change_request,
  user_credentials_username_stored,
  
  credentials_change_completed
  
}Credentials_State_T;


/* ==================================================================== */
/* ============================ classes =============================== */
/* ==================================================================== */
class Serial_Event
{
  private:
    Credentials_State_T login_state;
  
    String new_ssid;
    String new_password;
    String new_username;
    String inputString;
    void Serial_ParseString(String s);
    
  public:
    Serial_Event();
    void Serial_RxEvent();
};

#endif /* _SERIAL_EVENT_H_ */

/* EOF */
