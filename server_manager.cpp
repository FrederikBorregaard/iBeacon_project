#include "server_manager.h"

ESP8266WebServer server(80);

typedef enum Gpio_ID_Tag
{
  Gpio_ID_D4 = 0,
  Gpio_ID_D5,
  Gpio_Last_ID
  
}Gpio_ID_T;

String  GpioState[GPIO_REMOTE_USED] = {"OFF","OFF"};

float temperature = 0 ;
float humidity = 0 ;
float pressure = 0;
uint8_t light_level = 0;

void updateGPIO(Gpio_ID_T gpio_id, String gpio_state);


String getPage()
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
  webpage +=                "<h1>iBeacon! <small>Smart home solutions</small></h1>";
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


void handleRoot()
{ 
  if(server.hasArg("D4")) 
  {
    updateGPIO(Gpio_ID_D4, server.arg("D4")); 
  } 
  
  else if(server.hasArg("D5")) 
  {
    updateGPIO(Gpio_ID_D5, server.arg("D5")); 
  } 
  
  else
  {
    server.send(200, "text/html", getPage());
  }  
}


void updateGPIO(Gpio_ID_T gpio_id, String gpio_state) 
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
    server.send(200, "text/html", getPage());
  } 
  
  else if(gpio_state == "0")
  {
    digitalWrite(GpioPin[gpio_id], LOW);

    /* Change span's value */
    GpioState[gpio_id] = "Off";
    server.send(200, "text/html", getPage());
  } 
  else 
  {
    Serial.println("GPIO -> Unknown request");
  }  
}

void ServerInit()
{
  temperature = 22;
  humidity = 52;
  pressure = 1002;
  light_level = 69;
  
  

  server.on ( "/", handleRoot );

  server.begin();
  Serial.println("SERVER -> Started");
}


void server_handle_client()
{
  server.handleClient();
}

