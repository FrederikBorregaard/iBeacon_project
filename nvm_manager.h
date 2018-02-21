/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       nvm_manager.h
 */
#ifndef NVM_MANAGER_H_
#define NVM_MANAGER_H_

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include <EEPROM.h>
#include <Arduino.h>

/* ==================================================================== */
/* ============================= defines ============================== */
/* ==================================================================== */
#define EEPROM_ALLOCATED_SIZE_BYTE        (256)

/* Max size for storing single pair of credentials (2 Pairs of credentials are stored AP and USER) */
#define EEPROM_CREDENTIAL_MAX_SIZE        (EEPROM_ALLOCATED_SIZE_BYTE / 2)

/* Access Point credentials Start Address */
#define EEPROM_AP_CREDENTIALS_START_ADDR    (0x00)

/* User credentials Start Address - for website access */
#define EEPROM_USER_CREDENTIALS_START_ADDR  (0x80)

#define EEPROM_WRITE_OK               ((bool)true)
#define EEPROM_WRITE_ERROR            ((bool)false)

/* ==================================================================== */
/* ============================ classes =============================== */
/* ==================================================================== */
class Nvm_Manager
{
  public:
    void Nvm_Init();
    bool Nvm_CredentialsWrite(uint16_t start_addr, const char *ssid, const char *pass, const uint16_t ssid_len, const uint16_t pass_len);
    void Nvm_CredentialsRead(uint16_t start_addr, String &ssid_buf, String &pass_buf);
    void NvM_ReadRawData();
  
  private:
    void Nvm_ReadValue_Till_Sepatator(uint16_t start_idx, String &eep_data_buf);
};

#endif /* NVM_MANAGER_H_ */

/* EOF */
