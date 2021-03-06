/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       wifi_manager.cpp
 */

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include "wifi_manager.h"

/* ==================================================================== */
/* ======================== global variables ========================== */
/* ==================================================================== */
/* WiFi handler */
WiFi_Manager wifi;

/* NvM handler */
extern Nvm_Manager eeprom;

/* Web server handler */
extern Server_Manager server;

/* SSID and Password of the AP */
String ap_ssid;
String ap_pass;

/* WiFi establishing connection related flag */
bool establish_failed = false;

/* WiFi connection lost related flag */
bool connection_lost = false;

/* ==================================================================== */
/* ============================ functions ============================= */
/* ==================================================================== */

/*
 * WiFi connect function
 *  - This function connects using the credentials stored in EEPROM
 *  - This function starts the websocket whec connection is estabilished
 */
void WiFi_Manager::WiFi_Connect()
{ 
  /* Read SSID and PASSWORD for AP from NvM */
  eeprom.Nvm_CredentialsRead(EEPROM_AP_CREDENTIALS_START_ADDR, ap_ssid, ap_pass);
  
  /* Connect to the access point 
   *  
   *    clear WIFI Credentials
   *    avoid to store WIFI configuration in Flash
   *    ensure WiFi mode is Station 
   */
  WiFi.disconnect(true);
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);

  Serial.printf("WIFI -> SSID: %s\r\n", ap_ssid.c_str());
  Serial.printf("WIFI -> PASSWORD: %s\r\n", ap_pass.c_str());
  
  WiFi.begin(ap_ssid.c_str(), ap_pass.c_str());
  Serial.printf("WIFI -> Connecting\r\n");

  WiFi.setAutoReconnect(true);
 
  while(WIFI_IS_DISCONNECTED())
  {
    /* Wait for establishing the connection */
    if(false != establish_failed)
    {
      /* Timeout occurred */
      break;
    }
    delay(5);
  }

  if(WIFI_IS_DISCONNECTED())
  {
    /* Connecting broken due to timeout occurred */
    Serial.printf("WIFI -> Connection timeout\r\n");
  }
  else
  {
    /* WiFi is connected */
    Serial.printf("WIFI -> Connected\r\n");
    WiFi_Restore();
  }
}


/*
 * WiFi restorenect function
 *  - This function starts Server
 *  - This function should be called when WiFi connection is established
 */
void WiFi_Manager::WiFi_Restore()
{
  /* No timeout -> WiFi.status() == WL_CONNECTED */
  char buf[16];

  /* Get IP address */
  IPAddress ip = WiFi.localIP();
  
  /* Convert ip addres to char* and store it in the 'buf' */
  sprintf(buf, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
  
  printf("WIFI -> IP address: %s\r\n\r\n", buf);
  
  /* Start server */
  server.Server_Init();
}


/*
 * Establish connection failed event
 *  - This function is called when establishing wifi connection is failed (only when wifi is initialized)
 *  - Time for establishing is declared in tmr_manager.h (TMR_ESTABLISH_CONNECTION_TIMEOUT_MS)
 */
void WiFi_Manager::WiFi_establish_connection_timeout_event()
{
  establish_failed = true;
}


/*
 * Reconnect failed event
 *  - This function is called when wifi reconnecting is failed
 *  - Time for reconnection is declared in tmr_manager.h (TMR_RECONNECT_TIMEOUT_MS)
 */
void WiFi_Manager::WiFi_reconnect_failed_timeout_event()
{
  Serial.printf("WIFI -> Reconnect failed: RESET\r\n");
  
  WiFi.disconnect();
  ESP.restart();

  /* Wait until the reset occurs */
  while(1);
}


/*
 * Set Connection lost flag
 *  - This function should be called to indicate lost WiFi connection
 */
void WiFi_Manager::WiFi_set_connection_lost_flag()
{
  connection_lost = true;
}


/*
 * Clear Connection lost flag
 *  - This function should be called to indicate restore WiFi connection
 */
void WiFi_Manager::WiFi_clear_connection_lost_flag()
{
  connection_lost = false;
}


/*
 * Get Connection lost flag
 *  - This function returns WiFi connection lost flag
 */
bool WiFi_Manager::WiFi_get_connection_lost_flag()
{
  return connection_lost;
}

/* EOF */
