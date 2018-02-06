#ifndef _WEB_SOCKET_EVENT_H_
#define _WEB_SOCKET_EVENT_H_

#include <WebSocketsServer.h>
#include <Arduino.h>

void webSocketInit(void);
void webSocketLoop(void);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

#endif /* _WEB_SOCKET_EVENT_H_ */
