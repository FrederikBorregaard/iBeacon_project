/*
 *  @Author:          Jakub Witowski
 *  @Project name:    iBeacon
 *  @File name:       serial_manager.cpp
 */
 
/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include "server_manager.h"

/* ==================================================================== */
/* ======================== global variables ========================== */
/* ==================================================================== */

/* Dummy values */
float temperature = 22.3 ;
float humidity = 45;
float pressure = 1002.5;
uint8_t light_level = 69;

/* Username and Password of the Website access */
String user_username;
String user_password;

/* Create WebServer instance */
ESP8266WebServer WServer(80);

/* Initialize the GpioState tab */
String  GpioState[GPIO_REMOTE_USED] = {"OFF", "OFF"};

/* NvM handler */
Nvm_Manager e;

/* ==================================================================== */
/* ================== local function declarations ===================== */
/* ==================================================================== */
inline void handleControlData();
inline void handleLogin();

/* ==================================================================== */
/* ================== local function definitions  ===================== */
/* ==================================================================== */

/* 
 *  handleControlData()
 *    - This functions handles the Server's requests related to the control website
 */
void handleControlData()
{
  Server_Manager s;
  String header;

  if(!s.Server_IsAuthentified())
  { 
    WServer.sendHeader("Location","/login");
    WServer.sendHeader("Cache-Control","no-cache");
    WServer.send(301);
  }
  else
  {
    /* Server authenticate passed ! */
    if(WServer.hasArg("D4")) 
    {
      s.Server_UpdateGPIO(Gpio_ID_D4, WServer.arg("D4")); 
    } 
    
    else if(WServer.hasArg("D5")) 
    {
      s.Server_UpdateGPIO(Gpio_ID_D5, WServer.arg("D5")); 
    } 
  
    else
    {
      WServer.send(200, "text/html", s.Server_GetControlPage());
    }
  }
}


/* 
 *  handleLogin()
 *    - This functions handles the Server's requests related to the login website
 */
void handleLogin()
{
  Server_Manager s;
  String msg = "";
  
  if(WServer.hasHeader("Cookie"))
  {
    String cookie = WServer.header("Cookie");
  }
  
  if(WServer.hasArg("DISCONNECT"))
  {
    Serial.println("SERVER -> LOGOUT");
    
    WServer.sendHeader("Location","/login");
    WServer.sendHeader("Cache-Control","no-cache");
    WServer.sendHeader("Set-Cookie","ESPSESSIONID=0");
    WServer.send(301);
  }

  else
  {
    if(WServer.hasArg("USERNAME") && WServer.hasArg("PASSWORD"))
    {
      /* Check if ligin credentials are appropriate */
      if(WServer.arg("USERNAME") == user_username &&  WServer.arg("PASSWORD") == user_password)
      {
        WServer.sendHeader("Location","/");
        WServer.sendHeader("Cache-Control","no-cache");
        WServer.sendHeader("Set-Cookie","ESPSESSIONID=1");
        WServer.send(301);
        
        Serial.println("SERVER -> LOGIN OK");
      }
      else
      {
        msg = "Wrong username or password!";
        Serial.println("SERVER -> LOGIN ERROR");
      }    
    }

    /* Show Login page */
    WServer.send(200, "text/html", s.Server_GetLoginPage(msg));
  }
}

/* ==================================================================== */
/* ============================ functions ============================= */
/* ==================================================================== */

/* 
 * Server_Init()
 *  - This functions initializes the server
 *  - It should be called when the WiFi connection is established
 */
void Server_Manager::Server_Init()
{ 
  /* Read Username and Password for Website access from NvM */
  e.Nvm_CredentialsRead(EEPROM_USER_CREDENTIALS_START_ADDR, user_username, user_password);
  
  /* Connect the callbacks */
  WServer.on("/", handleControlData);
  WServer.on("/login", handleLogin);

  /* List of headers to be recorded */
  const char *headerkeys[] = {"User-Agent","Cookie"};
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  
  /* Ask server to track these headers */
  WServer.collectHeaders(headerkeys, headerkeyssize);
  WServer.begin();
  
  Serial.println("SERVER -> Started");

  Serial.printf("SERVER -> USERNAME: %s\n", user_username.c_str());
  Serial.printf("SERVER -> PASSWORD: %s\n", user_password.c_str());
}


/* 
 * Server_HandleClient()
 *  - This functions handles Clients requests (webbrowsers requests)
 *  - This function should be called periodically in the loop
 */
void Server_Manager::Server_HandleClient()
{
  WServer.handleClient();
}


/* 
 * Server_IsAuthentified()
 *  - This function checks if header is present and correct
 */
bool Server_Manager::Server_IsAuthentified()
{
  bool success_status = false;
  
  if(WServer.hasHeader("Cookie"))
  {
    String cookie = WServer.header("Cookie");
    
    if(cookie.indexOf("ESPSESSIONID=1") != -1) 
    {
      success_status = true;
    }
    else
    {
      Serial.println("SERVER -> Authentication Failed");
    }
  }
  return success_status;
}


/* 
 * Server_UpdateGPIO()
 *  - This functions updates the GPIO information in website's source code
 */
void Server_Manager::Server_UpdateGPIO(Gpio_ID_T gpio_id, String gpio_state)
{
  Serial.print("GPIO -> "); 
  Serial.print(GpioPin[gpio_id]); 
  Serial.print(": "); 
  Serial.println(gpio_state);
  
  if(gpio_state == "1") 
  {
    digitalWrite(GpioPin[gpio_id], HIGH);

    /* Change span's value */
    GpioState[gpio_id] = "On";
    WServer.send(200, "text/html", Server_GetControlPage());
  } 
  
  else if(gpio_state == "0")
  {
    digitalWrite(GpioPin[gpio_id], LOW);

    /* Change span's value */
    GpioState[gpio_id] = "Off";
    WServer.send(200, "text/html", Server_GetControlPage());
  } 
  else 
  {
    Serial.println("GPIO -> Unknown request");
  }  
}


/* 
 *  Server_GetControlPage()
 *  - This functions prints the info website on the server
 */
String Server_Manager::Server_GetControlPage()
{
  String  webpage = "";
  
  webpage += "<html charset=UTF-8><head><meta http-equiv='refresh' content='60' name='viewport' content='width=device-width, initial-scale=1'/>";
  webpage +=    "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js'></script><script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
  webpage +=    "<link href='https://maxcdn.bootstrapcdn.com/bootswatch/3.3.7/darkly/bootstrap.min.css' rel='stylesheet'>";
  webpage +=    "<title>ESP8266 IoT - EMBEDDED SOLUTIONS jakub.witowski9302(at)gmail.com</title></head><body>";
  
  webpage +=    "<div class='container-fluid'>";
  webpage +=       "<div class='row'>";
  webpage +=          "<div class='col-md-12'>";
  webpage +=             "<div class='page-header'>";
  webpage +=                "<h1>";
  webpage +=                  "iBeacon! <small>Smart home solutions</small>";
  webpage +=                   "<a href=\'/login?DISCONNECT=YES\' class='btn' type='button'>LogOut</a>";
  webpage +=                "</h1>";
  webpage +=             "</div>";
  webpage +=          "</div>";
  webpage +=       "</div>";
  
  webpage +=       "<div class='row'>";
  webpage +=          "<div class='col-md-6'>";
  webpage +=             "<div class='page-header'><h1><small>Light management</small></h1></div>";
  webpage +=             "<div class='row'>";
  webpage +=                "<div class='col-md-4'>";
  webpage +=                   "<ul class='nav nav-pills'>";
  webpage +=                      "<li class='active'>";
  webpage +=                         "<a href='#'>";
  webpage +=                            "<span class='badge pull-right'>";
  webpage +=                               GpioState[Gpio_ID_D4];
  webpage +=                            "</span>";
  webpage +=                            "D4 output";
  webpage +=                         "</a>";
  webpage +=                      "</li><br>";
  webpage +=                   "</ul>";
  webpage +=                "</div>";
                              
                            /* D4 (GPIO2) related buttons handling */
  webpage +=                "<div class='col-md-4'>";
  webpage +=                  "<form action='/' method='POST'>";
  webpage +=                    "<button type='button submit' name='D4' value='1' class='btn btn-block btn-success'>";
  webpage +=                      "ON";
  webpage +=                    "</button>";
  webpage +=                  "</form>";
  webpage +=                "</div>";

  webpage +=                "<div class='col-md-4'>";
  webpage +=                  "<form action='/' method='POST'>";
  webpage +=                    "<button type='button submit' name='D4' value='0' class='btn btn-block btn-danger btn'>";
  webpage +=                      "OFF";
  webpage +=                    "</button>";
  webpage +=                  "</form>";
  webpage +=                "</div>";
  webpage +=             "</div>";
  
  
  webpage +=             "<div class='row'><div class='col-md-12'></div></div>";
  
  webpage +=             "<div class='row'>";
  webpage +=                "<div class='col-md-4'>";
  webpage +=                   "<ul class='nav nav-pills'>";
  webpage +=                      "<li class='active'>";
  webpage +=                         "<a href='#'>";
  webpage +=                            "<span class='badge pull-right'>";
  webpage +=                               GpioState[Gpio_ID_D5];
  webpage +=                            "</span>";
  webpage +=                            "D5 output";
  webpage +=                         "</a>";
  webpage +=                      "</li><br>";
  webpage +=                   "</ul>";
  webpage +=                "</div>";

                            /* D5 (GPIO14) related buttons handling */
  webpage +=                "<div class='col-md-4'>";
  webpage +=                  "<form action='/' method='POST'>";
  webpage +=                    "<button type='button submit' name='D5' value='1' class='btn btn-block btn-success'>";
  webpage +=                      "ON";
  webpage +=                    "</button>";
  webpage +=                  "</form>";
  webpage +=                "</div>";

  webpage +=                "<div class='col-md-4'>";
  webpage +=                  "<form action='/' method='POST'>";
  webpage +=                    "<button type='button submit' name='D5' value='0' class='btn btn-block btn-danger btn'>";
  webpage +=                      "OFF";
  webpage +=                    "</button>";
  webpage +=                  "</form>";
  webpage +=                "</div>";
  webpage +=             "</div>";
  
  webpage +=             "<div class='page-header'> <h1><small>Sensors</small></h1></div>";
  
  webpage +=             "<table class='table'>";
  webpage +=                "<thead>";
  webpage +=                   "<tr><th>#</th><th>Sensor</th><th>Value</th><th>Status </th></tr>";
  webpage +=                "</thead>";
  
  webpage +=                "<tbody>";
  webpage +=                   "<tr class='active'><td>1</td><td>Temperature</td>";
  webpage +=                      "<td>";
  webpage +=                         temperature;
  webpage +=                         " &#8451";
  webpage +=                      "</td>";
  webpage +=                      "<td>";
  webpage +=                         "OK";
  webpage +=                      "</td>";
  webpage +=                   "</tr>";
  
  webpage +=                   "<tr class='success'><td>2</td><td>Humidity</td>";
  webpage +=                      "<td>";
  webpage +=                         humidity;
  webpage +=                         " %";
  webpage +=                      "</td>";
  webpage +=                      "<td>";
  webpage +=                         "OK";
  webpage +=                      "</td>";
  webpage +=                   "</tr>";
  
  webpage +=                   "<tr class='warning'><td>3</td> <td>Pressure</td>";
  webpage +=                      "<td>";
  webpage +=                         pressure;
  webpage +=                         " hPa";
  webpage +=                      "</td>";
  webpage +=                      "<td>";
  webpage +=                         "OK";
  webpage +=                      "</td>";
  webpage +=                   "</tr>";
  
  webpage +=                   "<tr class='danger'><td>4</td><td>Light level</td>";
  webpage +=                      "<td>";
  webpage +=                         light_level;
  webpage +=                         " %";
  webpage +=                      "</td>";
  webpage +=                      "<td>";
  webpage +=                         "ERROR";
  webpage +=                      "</td>";
  webpage +=                   "</tr>";
  
  webpage +=                "</tbody>";
  webpage +=             "</table>";
  webpage +=          "</div>";
  webpage +=          "<div class='col-md-6'></div>";
  webpage +=       "</div>";
  
  webpage +=       "<div class='row'><div class='col-md-6'></div>";
  webpage +=          "<div class='col-md-6'></div>";
  webpage +=       "</div>";
  
  webpage +=       "<div class='row'>";
  webpage +=          "<div class='col-md-4'>";
  webpage +=             "<address><strong>Embedded Solutions</strong><br>jakub.witowski9302@gmail.com</address>";
  webpage +=          "</div>";
  
  webpage +=          "<div class='col-md-4'></div>";
  webpage +=          "<div class='col-md-4'></div>";
  webpage +=       "</div>";
  webpage +=    "</div>";
  webpage += "</body></html>";
  
  return webpage;
}


/* 
 *  Server_GetLoginPage()
 *   - This functions prints the login website on the server
 */
String Server_Manager::Server_GetLoginPage(String info_msg)
{
  String  webpage = "";

  webpage += "<html charset=UTF-8><head><meta http-equiv='refresh' content='60' name='viewport' content='width=device-width, initial-scale=1'/>";
  webpage +=   "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js'></script><script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
  webpage +=   "<link href='https://maxcdn.bootstrapcdn.com/bootswatch/3.3.7/darkly/bootstrap.min.css' rel='stylesheet'>";
  webpage +=   "<title>Login ESP8266 IoT - EMBEDDED SOLUTIONS</title></head><body>";

  webpage +=    "<div class='container-fluid'>";
  webpage +=       "<div class='row'>";
  webpage +=          "<div class='col-md-12'>";
  webpage +=             "<div class='page-header'>";
  webpage +=                "<h1>iBeacon! <small>Login page</small></h1>";
  webpage +=             "</div>";
             
  webpage +=             "<form class='form-horizontal' role='form'>";
  webpage +=                "<div class='form-group'>";
                      
  webpage +=                   "<label class='col-sm-2 control-label'>";
  webpage +=                      "User";
  webpage +=                   "</label>";

                               /* Username text handling */
  webpage +=                   "<div class='col-sm-3'>";
  webpage +=                      "<input type='text' class='form-control' name='USERNAME' placeholder='user name'><br>";
  webpage +=                   "</div>";
  
  webpage +=                "</div>";
                  
  webpage +=                "<div class='form-group'>";
  webpage +=                   "<label class='col-sm-2 control-label'>";
  webpage +=                      "Password";
  webpage +=                   "</label>";

                               /* Password text handling */
  webpage +=                   "<div class='col-sm-3'>";
  webpage +=                      "<input type='password' class='form-control' name='PASSWORD' placeholder='password'><br>";
  webpage +=                   "</div>";
  
  webpage +=                "</div>";
                  
  webpage +=                "<div class='form-group'>";
  webpage +=                   "<div class='col-sm-offset-2 col-sm-10'>";
  
                                  /* Sign In Button handling */
  webpage +=                      "<form action='/login' method='POST'>";
  webpage +=                         "<button type='submit button' name='SUBMIT' value='Submit' class='btn btn-default'>";
  webpage +=                            "Sign in";
  webpage +=                         "</button>";
  webpage +=                      "</form>";
  webpage +=                      "&nbsp&nbsp&nbsp&nbsp" + info_msg;
  
  webpage +=                   "</div>";
  webpage +=                "</div>";
  webpage +=             "</form>";
  webpage +=          "</div>";
  webpage +=       "</div>";
  webpage +=    "</div>";
  webpage += "</body></html>";
   
  return webpage;
}

/* EOF */
