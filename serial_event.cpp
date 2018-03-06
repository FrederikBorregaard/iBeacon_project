/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       serial_event.cpp
 */

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include "serial_event.h"

/* ==================================================================== */
/* ============================= defines ============================== */
/* ==================================================================== */
#define CREDENTIALS_CHANGE_COMPLETED()  (login_state == credentials_change_completed)

#define REBOOT()                        Serial.printf("REBOOT -> Reboot in progress\r\n"); \
                                        WiFi.disconnect();                                 \
                                        ESP.restart();                                     \
                                        while(1)
                                                 
/* ==================================================================== */
/* ======================== global variables ========================== */
/* ==================================================================== */
/* Serial Event handler */
Serial_Event serial_e;

/* NvM handler */
extern Nvm_Manager eeprom;

/* Sensor handler */
extern Sensor sensor;

/* GPIO handler */
extern Gpio_Manager gpio;

/* ==================================================================== */
/* ============================ functions ============================= */
/* ==================================================================== */

/*
 * Serial_Event Ctor
 */
Serial_Event::Serial_Event()
{
  Serial.begin(SERIAL_BAUDRATE);
  login_state = credentials_change_completed;

  /* Reserve bytes for inputString */
  inputString = "";
  new_ssid = "";
  new_password = "";
  new_username = "";
  
  inputString.reserve(EEPROM_CREDENTIAL_MAX_SIZE);
  new_ssid.reserve(EEPROM_CREDENTIAL_MAX_SIZE);
  new_password.reserve(EEPROM_CREDENTIAL_MAX_SIZE);
  new_username.reserve(EEPROM_CREDENTIAL_MAX_SIZE);
}


/*
 * Serial_ParseString
 *  - This function performs an action according to inputString given in
 *    the Serial_RxEvent
 */
void Serial_Event::Serial_ParseString(String s)
{ 
  /* Status value of EEPROM write operation */
  bool eep_write_stat = EEPROM_WRITE_ERROR;
  
  switch(login_state)
  {
    /**********************************************
     *        ACCESS POINT CREDENTIALS            *
     **********************************************/
    case ap_credentials_change_request:
    {
      new_ssid = s;      
      login_state = ap_credentials_ssid_stored;
      
      Serial.printf("LOGIN AP -> Enter PASSWORD\r\n");
      break;
    }

    case ap_credentials_ssid_stored:
    {
      new_password = s;
      login_state = credentials_change_completed;
      
      /* Write new AP credentials to the NvM */
      eep_write_stat = eeprom.Nvm_CredentialsWrite(EEPROM_AP_CREDENTIALS_START_ADDR, new_ssid.c_str(), new_password.c_str(), strlen(new_ssid.c_str()), strlen(new_password.c_str()));
      
      if(EEPROM_WRITE_ERROR != eep_write_stat)
      {
        Serial.printf("LOGIN AP -> Credentials CHANGED\r\n");

        /* Reboot device */
        REBOOT();
      }
      else
      {
        Serial.printf("LOGIN AP -> Credentials NOT CHANGED\r\n");

        /* Resume reconnect timer */
        Start_reconnect_tmr(TMR_RECONNECT_TIMEOUT_MS);
      }
      
      break;
    }

    /**********************************************
     *              USER CREDENTIALS              *
     **********************************************/
    case user_credentials_change_request:
    {
      new_username = s;      
      login_state = user_credentials_username_stored;

      Serial.printf("LOGIN USER -> Enter PASSWORD\r\n");
      break;
    }

    case user_credentials_username_stored:
    {
      new_password = s;
      login_state = credentials_change_completed;

      /* Write new USER credentials to the NvM */
      eep_write_stat = eeprom.Nvm_CredentialsWrite(EEPROM_USER_CREDENTIALS_START_ADDR, new_username.c_str(), new_password.c_str(), strlen(new_username.c_str()), strlen(new_password.c_str()));
      
      if(EEPROM_WRITE_ERROR != eep_write_stat)
      {
        Serial.printf("LOGIN USER -> Credentials CHANGED\r\n");

        /* Reboot device */
        REBOOT();
      }
      else
      {
        Serial.printf("LOGIN USER -> Credentials NOT CHANGED\r\n");

        /* Restore reconnect timer */
        Start_reconnect_tmr(TMR_RECONNECT_TIMEOUT_MS);
      }
      
      break;
    }

    case credentials_change_completed:
    {
      break;
    }

    default:
    {
      Serial.printf("LOGIN -> Unexpected error\r\n");
      break;
    }
  }

  if((String("reboot") == s) && CREDENTIALS_CHANGE_COMPLETED())
  {
    REBOOT();
  }
  
  else if((String("ap_login") == s) && CREDENTIALS_CHANGE_COMPLETED())
  {
    /* Stop reconnect timer */
    Stop_reconnect_tmr();
    Serial.printf("LOGIN AP -> Enter SSID\r\n");
    
    login_state = ap_credentials_change_request;
  }

  else if((String("user_login") == s) && CREDENTIALS_CHANGE_COMPLETED())
  {
    /* Stop reconnect timer */
    Stop_reconnect_tmr();
    Serial.printf("LOGIN USER -> Enter USERNAME\r\n");
    
    login_state = user_credentials_change_request;
  }

  else if((String("raw_eeprom") == s) && CREDENTIALS_CHANGE_COMPLETED())
  {
    eeprom.NvM_ReadRawData();
  }

  else if((String("sensor") == s) && CREDENTIALS_CHANGE_COMPLETED())
  {
    sensor.Sensor_DebugPrint();
  }

  else if((String("gpio") == s) && CREDENTIALS_CHANGE_COMPLETED())
  {
    gpio.Gpio_DebugPrint();
  }
  
  else
  {
    if(CREDENTIALS_CHANGE_COMPLETED())
    {
      Serial.printf("OK\r\n");
    }
  }
  
  /* Clear the string */
  inputString = "";
}


/*
 * Serial_RxEvent
 *  - SerialEvent occurs whenever a new data comes in the hardware serial RX. This
 *    routine is called periodically in the loop() function, so using delay inside loop can
 *    delay response. Multiple bytes of data may be available.
 */
void Serial_Event::Serial_RxEvent()
{
  while(Serial.available()) 
  {
    char inChar = (char)Serial.read();
    
    /* If the incoming character is a newline - set a flag */
    if(inChar == '\n') 
    {
      Serial_ParseString(inputString);
    }
    else
    {
      /* Add new char to the inputString */
      inputString += inChar;
    }
  }
}

/* EOF */
