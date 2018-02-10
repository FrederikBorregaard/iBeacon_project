#ifndef _SERVER_MANAGER_H_
#define _SERVER_MANAGER_H_

#include <ESP8266WebServer.h>
#include "gpio_manager.h"

String getPage();
void server_handle_client();
void ServerInit();

#endif /* _SERVER_MANAGER_H_ */
