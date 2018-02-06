# WebSocketServer

/*
 *  @Author:          Jakub Witowski 
 *  @Creation date:   06.02.2018 
 *  @Project name:    WebSocketServer
 * 
 * 
 *  Functionality:
 *  
 *    - Implemented basic command line interface
 *      - Serial Baud Rate
 *      - Full logging implemented
 *      - "login", and "reboot" command
 *      
 *    - Implemented WiFi AP connection
 *      - SSID and PASSWORD are stored in EEPROM
 *      - change credentials during the runtime using "login" command
 *      
 *    - Reboot support
 *      - Reboot after configurable timeout when cannot connect to the AP
 *      - Reboot after configurable timeout when lost WiFi connection and cannot restore it
 *      
 *    - Implemented WebSocketSerwer
 *      - Configurable PORT and IP nr: PORT @@81
 *      - Receiving JSON package: JSON input buffer
 *      - Supported commands in JSON format:
 *        - "{"LED1" : 1}"
 *        - "{"LED1" : 0}"
 *        
 *        
 *    Configurable parameters:
 *      - Serial Baud Rate: 115200
 *      - Establishing connection timeout: 16000ms
 *      - Trying reconnect timeout: 20000ms
 *      - WebSocketServer Port: 81
 *      - JSON input buffer size: 200bytes
 */