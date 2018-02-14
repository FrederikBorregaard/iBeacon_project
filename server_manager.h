#ifndef _SERVER_MANAGER_H_
#define _SERVER_MANAGER_H_

#include <ESP8266WebServer.h>
#include "gpio_manager.h"


/* Enumerate for GPIO's ID */
typedef enum Gpio_ID_Tag
{
  Gpio_ID_D4 = 0,
  Gpio_ID_D5,
  Gpio_ID_Last
  
}Gpio_ID_T;

class Server_Manager
{
  private:
    

  public:
    void Server_Init();
    void Server_HandleClient();
    String Server_GetPage();
    void Server_UpdateGPIO(Gpio_ID_T gpio_id, String gpio_state);
};

#endif /* _SERVER_MANAGER_H_ */
