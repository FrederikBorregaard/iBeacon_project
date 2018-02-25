/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       wifi_manager.h
 */
#ifndef _WIFI_MANAGER_H_
#define _WIFI_MANAGER_H_

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include <ESP8266WiFi.h>
#include "nvm_manager.h"
#include "server_manager.h"

/* ==================================================================== */
/* ============================= defines ============================== */
/* ==================================================================== */
#define WIFI_IS_CONNECTED()     (WL_CONNECTED == WiFi.status())
#define WIFI_IS_DISCONNECTED()  (WL_CONNECTED != WiFi.status())

/* ==================================================================== */
/* ============================ classes =============================== */
/* ==================================================================== */
class WiFi_Manager
{
  public:
    void WiFi_Connect();
    void WiFi_establish_connection_timeout_event();
    void WiFi_reconnect_failed_timeout_event();

    /* Connection lost related methods */
    void WiFi_set_connection_lost_flag();
    void WiFi_clear_connection_lost_flag();
    bool WiFi_get_connection_lost_flag();
};

#endif /* _WIFI_MANAGER_H_ */

/* EOF */
