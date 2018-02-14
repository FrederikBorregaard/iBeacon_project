#ifndef NVM_MANAGER_H_
#define NVM_MANAGER_H_

#include <EEPROM.h>
#include <Arduino.h>

#define EEPROM_SIZE_BYTE              (128)
#define EEPROM_CREDENTIALS_START_ADDR (0x00)

#define EEPROM_WRITE_OK               ((bool)true)
#define EEPROM_WRITE_ERROR            ((bool)false)
class Nvm_Manager
{
  public:
    void Nvm_Init();
    bool Nvm_CredentialsWrite(const char *ssid, const char *pass, const uint16_t ssid_len, const uint16_t pass_len);
    void Nvm_CredentialsRead(String &ssid_buf, String &pass_buf);
  
  private:
    void Nvm_ReadValue_Till_Sepatator(uint16_t start_idx, String &eep_data_buf);
};

#endif /* NVM_MANAGER_H_ */
