#include "nvm_manager.h"

#define EEPROM_FILL_VAL         ((uint8_t)0xFF)
#define EEPROM_SEP_VAL          ((uint8_t)0xFE)
#define EEPROM_QUANTITY_OF_SEP  ((uint8_t)0x02)

void Nvm_Manager::Nvm_ReadValue_Till_Sepatator(uint16_t start_idx, String &eep_data_buf)
{
  uint8_t eep_byte;
  
  for(int i = start_idx; i < EEPROM_SIZE_BYTE; i++)
  {
    eep_byte = EEPROM.read(i);

    if(eep_byte == EEPROM_SEP_VAL)
    {
      /* Separator occurred - Stop read SSID */
      break;
    }
    else
    {
      if(EEPROM_FILL_VAL != eep_byte)
      {
        eep_data_buf += (char)eep_byte;
      }
      else
      {
        /* FILL data in EEPROM */
        break;
      }
    }
  }
}


void Nvm_Manager::Nvm_Init()
{
  EEPROM.begin(EEPROM_SIZE_BYTE);
  delay(100);
}


void Nvm_Manager::Nvm_CredentialsWrite(const char *ssid, const char *pass, const uint16_t ssid_len, const uint16_t pass_len)
{    
  if(ssid_len + pass_len - EEPROM_QUANTITY_OF_SEP <= EEPROM_SIZE_BYTE)
  {
    char credentials[ssid_len + pass_len + EEPROM_QUANTITY_OF_SEP];
    uint16_t cnt = EEPROM_CREDENTIALS_START_ADDR;
    
    while(*ssid != '\0')
    {
      credentials[cnt++] = *ssid++;
    }

    /* Separate SSID and the PASS */
    credentials[cnt++] = EEPROM_SEP_VAL;
    
    while(*pass != '\0')
    {
      credentials[cnt++] = *pass++;
    }

    /* Separate PASS and the rest of EEP data */
    credentials[cnt++] = EEPROM_SEP_VAL;
    
    /* Terminate the credential's string */
    credentials[cnt] = '\0';

      
    /* Write credentials to the EEPROM */
    for(uint16_t i = EEPROM_CREDENTIALS_START_ADDR; i < ssid_len + pass_len + EEPROM_QUANTITY_OF_SEP; i++)
    {
      EEPROM.write(i, credentials[i]);
    }
    
    EEPROM.end();
    Serial.println("EEPROM -> Write OK");
  }
  else
  {
    Serial.println("EEPROM -> Write ERROR");
  }
}


void Nvm_Manager::Nvm_CredentialsRead(String &ssid_buf, String &pass_buf)
{
  /* Get SSID string */
  Nvm_ReadValue_Till_Sepatator(EEPROM_CREDENTIALS_START_ADDR, ssid_buf);

  /* Get PASS string */
  Nvm_ReadValue_Till_Sepatator(ssid_buf.length() + 1, pass_buf); 
}

