/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       nvm_manager.cpp
 */

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include "nvm_manager.h"

/* ==================================================================== */
/* ============================= defines ============================== */
/* ==================================================================== */
#define EEPROM_FILL_VAL           ((uint8_t)0xFF)
#define EEPROM_SEP_VAL            ((uint8_t)0xFE)
#define EEPROM_QUANTITY_OF_SEP    ((uint8_t)0x02)
#define EEPROM_LAST_ASCII_OK_VAL  ((uint8_t)0x7E)

#define EEPROM_ENCRYPT_BYTE(x)    ((x * 2) - 13)
#define EEPROM_DECRYPT_BYTE(x)    ((char)((x + 13) / 2))

/* Only for debug usage - storing raw data in EEPROM is unsafety */
#define EEPROM_RAW_READ(x)        ((char)(x))
#define EEPROM_RAW_WRITE(x)       (x)

/* ==================================================================== */
/* ======================== global variables ========================== */
/* ==================================================================== */
/* EEPROM handler */
Nvm_Manager eeprom;

/* ==================================================================== */
/* ============================ functions ============================= */
/* ==================================================================== */

/*
 * Nvm_ReadValue_Till_Sepatator
 *  - This function reads EEPROM data from start_idx to the separator EEPROM_SEP_VAL
 *    and store it in the eep_data_buf
 *     
 *  - note:
 *    separator should be placed between each data field (ex. somedataSEPnextdataSEPotherdataSEPfill)
 */
void Nvm_Manager::Nvm_ReadValue_Till_Sepatator(uint16_t start_idx, String &eep_data_buf)
{
  uint8_t eep_byte;
  
  for(uint16_t i = start_idx; i < EEPROM_ALLOCATED_SIZE_BYTE; i++)
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
 * Nvm_Init
 *  - This function should be called before EEPROM usage
 */
void Nvm_Manager::Nvm_Init()
{
  EEPROM.begin(EEPROM_ALLOCATED_SIZE_BYTE);
  delay(100);
}


/*
 * Nvm_CredentialsWrite
 *  - This function writes EEPROM data with credential values (ssid and pass)
 *  - It returns operation status
 *    success - true
 *    no_success - false
 */
bool Nvm_Manager::Nvm_CredentialsWrite(uint16_t start_addr, const char *ssid, const char *pass, const uint16_t ssid_len, const uint16_t pass_len)
{      
  char credentials[ssid_len + pass_len + EEPROM_QUANTITY_OF_SEP];
  uint16_t cnt = 0;

  /* Flag that checks if unicode characters are in appropriate range (int: 126 max) */
  bool unicode_range_ok = true;
  bool success_status = true;
  
  if(ssid_len + pass_len - EEPROM_QUANTITY_OF_SEP <= EEPROM_CREDENTIAL_MAX_SIZE)
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
        for(uint16_t idx = start_addr, cnt = 0; idx < start_addr + ssid_len + pass_len + EEPROM_QUANTITY_OF_SEP; idx++, cnt++)
        {
          EEPROM.write(idx, credentials[cnt]);
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
 * Nvm_CredentialsRead
 *  - This function reads credentials from the EEPROM(ssid and password) and stores it in the separate buffers
 */
void Nvm_Manager::Nvm_CredentialsRead(uint16_t start_addr, String &ssid_buf, String &pass_buf)
{ 
  /* Get SSID or USER login string */
  Nvm_ReadValue_Till_Sepatator(start_addr, ssid_buf);

  /* Get PASS string - start_idx is offset of the ssid len and EEPROM_SEP_VAL(+1) */
  Nvm_ReadValue_Till_Sepatator(start_addr + ssid_buf.length() + 1, pass_buf); 
}


/*
 * NvM_ReadRawData
 *  - This function reads raw EEPROM data
 */
void Nvm_Manager::NvM_ReadRawData()
{
  Serial.println("EEPROM -> Read raw data START");
  
  for(uint16_t cnt = 0; cnt < EEPROM_ALLOCATED_SIZE_BYTE; cnt++)
  {
    Serial.println((char)EEPROM.read(cnt));
  }

  Serial.println("EEPROM -> Read raw data END");
}

/* EOF */
