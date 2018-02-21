/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       serial_manager.h
 */
#ifndef _SERVER_MANAGER_H_
#define _SERVER_MANAGER_H_

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include <ESP8266WebServer.h>
#include "gpio_manager.h"
#include "snsr_manager.h"
#include "nvm_manager.h"

/* ==================================================================== */
/* ============================ typedefs ============================== */
/* ==================================================================== */

/* Enumerate for GPIO's ID */
typedef enum Gpio_ID_Tag
{
  Gpio_ID_D4 = 0,
  Gpio_ID_D5,
  Gpio_ID_Last
  
}Gpio_ID_T;

/* ==================================================================== */
/* ============================ classes =============================== */
/* ==================================================================== */
class Server_Manager
{   
  public:
    void Server_Init();
    void Server_HandleClient();
    String Server_GetControlPage();
    String Server_GetLoginPage(String info_msg);
    void Server_UpdateGPIO(Gpio_ID_T gpio_id, String gpio_state);
    bool Server_IsAuthentified();
};

#endif /* _SERVER_MANAGER_H_ */

/* EOF */
