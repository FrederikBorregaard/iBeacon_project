#include "nvm_manager.h"

#define EEPROM_FILL_VAL           ((uint8_t)0xFF)
#define EEPROM_SEP_VAL            ((uint8_t)0xFE)
#define EEPROM_QUANTITY_OF_SEP    ((uint8_t)0x02)
#define EEPROM_LAST_ASCII_OK_VAL  ((uint8_t)0x7E)

#define EEPROM_ENCRYPT_BYTE(x)    ((x * 2) - 13)
#define EEPROM_DECRYPT_BYTE(x)    ((char)((x + 13) / 2))


/*
  Nvm_ReadValue_Till_Sepatator
    - This function reads EEPROM data from start_idx to the separator EEPROM_SEP_VAL
      and store it in the eep_data_buf
      
    - note:
      separator should be placed between each data field (ex. somedataSEPnextdataSEPotherdataSEPfill)
*/
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
        eep_data_buf += EEPROM_DECRYPT_BYTE(eep_byte);
      }
      else
      {
        /* FILL data in EEPROM */
        break;
      }
    }
  }
}


/*
  Nvm_Init
    - This function should be called before EEPROM usage
*/
void Nvm_Manager::Nvm_Init()
{
  EEPROM.begin(EEPROM_SIZE_BYTE);
  delay(100);
}


/*
  Nvm_CredentialsWrite
    - This function writes EEPROM data with credential values (ssid and pass)
    - It returns operation status
      success - true
      no_success - false
*/
bool Nvm_Manager::Nvm_CredentialsWrite(const char *ssid, const char *pass, const uint16_t ssid_len, const uint16_t pass_len)
{    
  char credentials[ssid_len + pass_len + EEPROM_QUANTITY_OF_SEP];
  uint16_t cnt = EEPROM_CREDENTIALS_START_ADDR;

  /* Flag that checks if unicode characters are in appropriate range (int: 126 max) */
  bool unicode_range_ok = true;
  bool success_status = true;
  
  if(ssid_len + pass_len - EEPROM_QUANTITY_OF_SEP <= EEPROM_SIZE_BYTE)
  {
    while(*ssid != '\0')
    {
      if(*ssid > EEPROM_LAST_ASCII_OK_VAL)
      {
        unicode_range_ok = false;
        break;
      }
      else
      {
        credentials[cnt++] = EEPROM_ENCRYPT_BYTE(*ssid);
        ssid++;
      }
    }

    /* If SSID characters are appropriate unicode values */
    if(unicode_range_ok)
    {
      /* Separate SSID and the PASS */
      credentials[cnt++] = EEPROM_SEP_VAL;
      
      while(*pass != '\0')
      {
        if(*pass > EEPROM_LAST_ASCII_OK_VAL)
        {
          unicode_range_ok = false;
          break;
        }
        else
        {
          credentials[cnt++] = EEPROM_ENCRYPT_BYTE(*pass);
          pass++;
        }
      }

      /* If PASS characters are appropriate unicode values */
      if(unicode_range_ok)
      {
  
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
        success_status = false;
        Serial.println("EEPROM -> WRITE ERROR: PASS Unicode OOR");
      }
    }
    else
    {
      success_status = false;
      Serial.println("EEPROM -> WRITE ERROR: SSID Unicode OOR");
    }
  }
  else
  {
    success_status = false;
    Serial.println("EEPROM -> Write ERROR");
  }

  return success_status;
}


/*
  Nvm_CredentialsRead
    - This function reads credentials from the EEPROM(ssid and password) and stores it in the separate buffers
*/
void Nvm_Manager::Nvm_CredentialsRead(String &ssid_buf, String &pass_buf)
{
  /* Get SSID string */
  Nvm_ReadValue_Till_Sepatator(EEPROM_CREDENTIALS_START_ADDR, ssid_buf);

  /* Get PASS string - start_idx is offset of the ssid len and EEPROM_SEP_VAL(+1) */
  Nvm_ReadValue_Till_Sepatator(ssid_buf.length() + 1, pass_buf); 
}

