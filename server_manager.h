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
#include "update_manager.h"

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
/* =========================== structures ============================= */
/* ==================================================================== */
/* Sensors status state to be printed on the website */
typedef struct Server_SensorState_Tag
{
  String Temp_SensorState;
  String Humid_SensorState;
  String Pres_SensorState;
  String Light_SensorState;
  
}Server_SensorState_T;

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
    
    void Server_Update_SensorsState(String t_status, String p_status, String h_status, String l_status);
};

#endif /* _SERVER_MANAGER_H_ */

/* EOF */
