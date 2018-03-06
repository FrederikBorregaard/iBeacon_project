/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       update_manager.cpp
 */
 
/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include "update_manager.h"

/* ==================================================================== */
/* ======================== global variables ========================== */
/* ==================================================================== */
/* Update manager handler */
Update_Manager ota;

/* Update Server handler */
ESP8266HTTPUpdateServer httpUpdater;

/* Web  Server handler */
extern ESP8266WebServer WServer;

/* ==================================================================== */
/* ================== local function definitions  ===================== */
/* ==================================================================== */

/* 
 *  Update_Manager_Init()
 *    - This functions initializes OTA (Over The Air) update
 */
void Update_Manager::Update_Manager_Init()
{
  Serial.printf("OTA -> Update request\r\n");
 
  MDNS.begin(host);
  Serial.printf("OTA -> MDNS host started\r\n");
  
  httpUpdater.setup(&WServer, update_path, update_username, update_password);
  Serial.printf("OTA -> Update Server setup complete\r\n");
  
  MDNS.addService("http", "tcp", 80);
  Serial.printf("OTA -> MDNS service added\r\n");
  
  Serial.printf("OTA -> Update server: http://%s.local%s\r\n", host, update_path);
}


/* EOF */
