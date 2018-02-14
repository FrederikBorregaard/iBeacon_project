#include "wifi_manager.h"
#include "nvm_manager.h"
#include "server_manager.h"

/* SSID and Password of the AP */
String ssid;
String pass;

/* NvM handler */
Nvm_Manager eepr;

/* Web server handler */
Server_Manager srvr;

/* WiFi establishing connection related flag */
bool establish_failed = false;

/* WiFi connection lost related flag */
bool connection_lost = false;


/*
  WiFi connect function
    - This function connects using the credentials stored in EEPROM
    - This function starts the websocket whec connection is estabilished
*/
void WiFi_Manager::WiFi_Connect()
{ 
  /* Read SSID and PASSWORD from NvM */
  eepr.Nvm_CredentialsRead(ssid, pass);
  
  /* Connect to the access point 
   *  
   *    clear WIFI Credentials
   *    avoid to store WIFI configuration in Flash
   *    ensure WiFi mode is Station 
   */
  WiFi.disconnect(true);
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);

  Serial.printf("WIFI -> SSID: %s\n", ssid.c_str());
  Serial.printf("WIFI -> PASSWORD: %s\n", pass.c_str());
  
  WiFi.begin(ssid.c_str(), pass.c_str());
  Serial.println("WIFI -> Connecting");

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
    Serial.println("WIFI -> Connection timeout");
  }
  else
  {
    /* WiFi is connected */
    Serial.println("WIFI -> Connected");
  
    /* No timeout -> WiFi.status() == WL_CONNECTED */
    Serial.printf("WIFI -> IP address: ");
    Serial.println(WiFi.localIP());

    /* Start server */
    srvr.Server_Init();
  }
}


/*
  Establish connection failed event
    - This function is called when establishing wifi connection is failed (only when wifi is initialized)
    - Time for establishing is declared in tmr_manager.h (TMR_ESTABLISH_CONNECTION_TIMEOUT_MS)
*/
void WiFi_Manager::WiFi_establish_connection_timeout_event()
{
  establish_failed = true;
}


/*
  Reconnect failed event
    - This function is called when wifi reconnecting is failed
    - Time for reconnection is declared in tmr_manager.h (TMR_RECONNECT_TIMEOUT_MS)
*/
void WiFi_Manager::WiFi_reconnect_failed_timeout_event()
{
  Serial.println("WIFI -> Reconnect failed: RESET");
  
  WiFi.disconnect();
  ESP.restart();

  /* Wait until the reset occurs */
  while(1);
}


/*
  Set Connection lost flag
    - This function should be called to indicate lost WiFi connection
*/
void WiFi_Manager::WiFi_set_connection_lost_flag()
{
  connection_lost = true;
}


/*
  Clear Connection lost flag
    - This function should be called to indicate restore WiFi connection
*/
void WiFi_Manager::WiFi_clear_connection_lost_flag()
{
  connection_lost = false;
}


/*
  Get Connection lost flag
    - This function returns WiFi connection lost flag
*/
bool WiFi_Manager::WiFi_get_connection_lost_flag()
{
  return connection_lost;
}

