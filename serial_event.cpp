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

#define REBOOT()                        Serial.println("REBOOT -> Reboot in progress"); \
                                        WiFi.disconnect();                              \
                                        ESP.restart();                                  \
                                        while(1)
                                                 
/* ==================================================================== */
/* ======================== global variables ========================== */
/* ==================================================================== */

/* NvM handler */
Nvm_Manager eep;

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
      
      Serial.println("LOGIN AP -> Enter PASSWORD");
      break;
    }

    case ap_credentials_ssid_stored:
    {
      new_password = s;
      login_state = credentials_change_completed;
      
      /* Write new AP credentials to the NvM */
      eep_write_stat = eep.Nvm_CredentialsWrite(EEPROM_AP_CREDENTIALS_START_ADDR, new_ssid.c_str(), new_password.c_str(), strlen(new_ssid.c_str()), strlen(new_password.c_str()));
      
      if(EEPROM_WRITE_ERROR != eep_write_stat)
      {
        Serial.println("LOGIN AP -> Credentials CHANGED");

        /* Reboot device */
        REBOOT();
      }
      else
      {
        Serial.println("LOGIN AP -> Credentials NOT CHANGED");

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

      Serial.println("LOGIN USER -> Enter PASSWORD");
      break;
    }

    case user_credentials_username_stored:
    {
      new_password = s;
      login_state = credentials_change_completed;

      /* Write new USER credentials to the NvM */
      eep_write_stat = eep.Nvm_CredentialsWrite(EEPROM_USER_CREDENTIALS_START_ADDR, new_username.c_str(), new_password.c_str(), strlen(new_username.c_str()), strlen(new_password.c_str()));
      
      if(EEPROM_WRITE_ERROR != eep_write_stat)
      {
        Serial.println("LOGIN USER -> Credentials CHANGED");

        /* Reboot device */
        REBOOT();
      }
      else
      {
        Serial.println("LOGIN USER -> Credentials NOT CHANGED");

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
      Serial.println("LOGIN -> Unexpected error");
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
    Serial.println("LOGIN AP -> Enter SSID");
    
    login_state = ap_credentials_change_request;
  }

  else if((String("user_login") == s) && CREDENTIALS_CHANGE_COMPLETED())
  {
    /* Stop reconnect timer */
    Stop_reconnect_tmr();
    Serial.println("LOGIN USER -> Enter USERNAME");
    
    login_state = user_credentials_change_request;
  }

  else if((String("raw_eeprom") == s) && CREDENTIALS_CHANGE_COMPLETED())
  {
    eep.NvM_ReadRawData();
  }
  

  else
  {
    if(CREDENTIALS_CHANGE_COMPLETED())
    {
      Serial.println("OK");
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
