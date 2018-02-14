#include "serial_event.h"

#define CREDENTIALS_CHANGE_COMPLETED()  (login_state == credentials_change_completed)

Nvm_Manager eep;

Serial_Event::Serial_Event()
{
  Serial.begin(SERIAL_BAUDRATE);
  login_state = credentials_change_completed;

  /* Reserve bytes for inputString */
  inputString = "";
  new_ssid = "";
  new_password = "";
  
  inputString.reserve(EEPROM_SIZE_BYTE);
  new_ssid.reserve(EEPROM_SIZE_BYTE);
  new_password.reserve(EEPROM_SIZE_BYTE);
}

/*
 * Perform an action according to inputString given in
 * the Serial_RxEvent
 */
void Serial_Event::Serial_ParseString(String s)
{ 
  /* Status value of EEPROM write operation */
  bool eep_write_stat = EEPROM_WRITE_ERROR;
  
  switch(login_state)
  {
    case credentials_change_request:
    {
      new_ssid = s;      
      login_state = credentials_ssid_stored;
      
      Serial.println("LOGIN -> Enter PASSWORD");
      break;
    }

    case credentials_ssid_stored:
    {
      new_password = s;
      login_state = credentials_change_completed;
      
      /* Write new credentials to the NvM */
      eep_write_stat = eep.Nvm_CredentialsWrite(new_ssid.c_str(), new_password.c_str(), strlen(new_ssid.c_str()), strlen(new_password.c_str()));
      
      if(EEPROM_WRITE_ERROR != eep_write_stat)
      {
        Serial.println("LOGIN -> Credentials CHANGED");
      }
      else
      {
        Serial.println("LOGIN -> Credentials NOT CHANGED");
      }

      /* Start reconnect timer */
      Start_reconnect_tmr(TMR_RECONNECT_TIMEOUT_MS);
      
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
    Serial.println("REBOOT -> Reboot in progress");
    
    WiFi.disconnect();
    ESP.restart();

    /* Wait until the reset occurs */
    while(1);
  }
  
  else if((String("login") == s) && CREDENTIALS_CHANGE_COMPLETED())
  {
    /* Stop reconnect timer */
    Stop_reconnect_tmr();
    Serial.println("LOGIN -> Enter SSID");
    
    login_state = credentials_change_request;
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
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void Serial_Event::Serial_RxEvent()
{
  while(Serial.available()) 
  {
    char inChar = (char)Serial.read();
    
    /* if the incoming character is a newline - set a flag */
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

