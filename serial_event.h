#ifndef _SERIAL_EVENT_H_
#define _SERIAL_EVENT_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "nvm_manager.h"
#include "tmr_config.h"

#define SERIAL_BAUDRATE             (115200)

typedef enum Credentials_State_Tag
{
  credentials_change_request = 0,
  credentials_ssid_stored,
  credentials_change_completed
  
}Credentials_State_T;

class Serial_Event
{
  private:
    String new_ssid;
    String new_password;
    String inputString;
    Credentials_State_T login_state;
    
    void Serial_ParseString(String s);
    
  public:
    Serial_Event();
    void Serial_RxEvent();
};

#endif /* _SERIAL_EVENT_H_ */
