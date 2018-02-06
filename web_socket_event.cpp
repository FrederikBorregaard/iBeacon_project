#include "web_socket_event.h"
#include "json_parser.h"
#include "gpio_manager.h"

WebSocketsServer webSocket = WebSocketsServer(81);
IPAddress client_IpAddr;

/* 
  JSON parser

  Inside the brackets, JSON_BUF_LEN is the size of the pool in bytes.
  Don't forget to change this value to match your JSON document.
  Use arduinojson.org/assistant to compute the capacity.
*/
StaticJsonBuffer<JSON_BUF_LEN> jsonBuffer;

/*
  JSON input string.

  It's better to use a char[] as shown here.
  If you use a const char* or a String, ArduinoJson will
  have to make a copy of the input in the JsonBuffer.
*/
char json[JSON_BUF_LEN];

/*
   webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
    - webSocketEvent is called on webSocket data received when the connection is set
      
      num - socket's number
      type - web socket state
      payload - data received via websocket
      length - length of the data
*/
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      {
        Serial.printf("[%u] Disconnected!\n", num);
        break;
      }

    case WStype_CONNECTED:
      {
        client_IpAddr = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, client_IpAddr[0], client_IpAddr[1], client_IpAddr[2], client_IpAddr[3], payload);

        /* Send message to client */
        webSocket.sendTXT(num, "Connected");
        break;
      }

    case WStype_TEXT:
      {
        /* Print received data */
        Serial.printf("%s\n", payload);

        /* Copy payload data to the json buffer */
        memcpy(json, payload, length);

        /*
          It's a reference to the JsonObject, the actual bytes are inside the
          JsonBuffer with all the other nodes of the object tree.
          Memory is freed when jsonBuffer goes out of scope.
        */
        jsonBuffer.clear();
        JsonObject& root = jsonBuffer.parseObject(json);

        /* Test if parsing succeeds. */
        if (false != root.success())
        {
          /*
            Fetch values.

            Most of the time, you can rely on the implicit casts.
            In other case, you can do root["time"].as<long>();
          */
          bool led_id = root["LED1"];

          if (false != led_id)
          {
            LED_ON();
          }
          else
          {
            LED_OFF();
          }

          /* send message to client */
          webSocket.sendTXT(num, payload);
        }

        else
        {
          Serial.println("parseObject() failed");
        }
        break;
      }

    case WStype_BIN:
      {
        Serial.printf("[%u] get binary length: %u\n", num, length);
        hexdump(payload, length);

        /* Send message to client */
        webSocket.sendBIN(num, payload, length);
        break;
      }

    default:
      {
        Serial.printf("Default state error");
      }
  }
}

/*
   webSocketInit(void)
    - webSocketInit should be called when the connsction is 
      estabilished during the initialization
*/
void webSocketInit(void)
{
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

/*
   webSocketLoop(void)
    - webSocketLoop should be called in the infinite loop
      to control all websocket related operations
*/
void webSocketLoop(void)
{
  webSocket.loop();
}
