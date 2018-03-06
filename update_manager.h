/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       update_manager.h
 */
#ifndef _UPDATE_MANAGER_H_
#define _UPDATE_MANAGER_H_

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include "server_manager.h"

/* ==================================================================== */
/* ============================ defines =============================== */
/* ==================================================================== */
#define UPDATE_MANAGER_USERNAME   ("admin")
#define UPDATE_MANAGER_PASSWORD   ("admin")
#define UPDATE_PATH               ("/firmware")
#define UPDATE_HOST               ("esp8266-webupdate")

/* ==================================================================== */
/* ============================ classes =============================== */
/* ==================================================================== */
class Update_Manager
{
  private:
    const char* host = UPDATE_HOST;
    const char* update_path = UPDATE_PATH;
    const char* update_username = UPDATE_MANAGER_USERNAME;
    const char* update_password = UPDATE_MANAGER_PASSWORD;
    
  public:
    void Update_Manager_Init();
};

#endif /* _UPDATE_MANAGER_H_ */

/* EOF */
