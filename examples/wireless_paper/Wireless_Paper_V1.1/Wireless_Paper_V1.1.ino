/* Heltec Automation  Wireless_Paper_V1.0 Sensors test example
 *
 * Function:
 * Refresh e-paper through WIFI
 *
 * HelTec AutoMation, Chengdu, China
 * www.heltec.org
 * Operation instance:
 *
 * this project also realess in GitHub:
 * https://github.com/HelTecAutomation/Heltec_ESP32
 */

#include <WebServer.h>
#include <ESPmDNS.h>
#include "HT_lCMEN2R13EFC1.h"
#include "images.h"
#include "html.h"
HT_ICMEN2R13EFC1 wireless_paper(6, 5, 4, 7, 3, 2, -1, 6000000); // rst,dc,cs,busy,sck,mosi,miso,frequency

int width, height;
String HTTP_Payload;
WebServer server(80);     // Create WebServer object with port number 80
// By using port number 80, you can directly enter the IP address for access, while using other ports requires entering the IP: port number for access

const char *ssid = "your-ssid";
const char *password = "your-password";

// Distribute processing callback function
void Config_Callback()
{

  String Payload;
  const char *buff;

  Payload = server.arg("value"); // Obtain the data delivered
  buff = Payload.c_str();

  delay(100);
  int i = 0;
  char *token;
  token = strtok((char *)buff, ",");

  while (token != NULL)
  {
    int num = atoi(token);
    WiFi_Logo_bits[i] = num;
    token = strtok(NULL, ",");
    Serial.print(WiFi_Logo_bits[i]);
    i++;
  }
  drawImageDemo();
  Serial.println("dd");
}

void setup()
{

  Serial.begin(115200);
  Serial.println();

  VextON();
  delay(100);
  wireless_paper.init();

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());
  server.on("/", []()
            { server.send(200, "text/html", index_html); });

  server.on("/set", HTTP_GET, Config_Callback); // Bind the handler that is configured to deliver the function
  server.begin();
}
void drawImageDemo()
{
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  wireless_paper.clear();
  wireless_paper.update(BLACK_BUFFER);

  wireless_paper.clear();
  int x = width / 2 - WiFi_Logo_width / 2;
  int y = height / 2 - WiFi_Logo_height / 2;
  wireless_paper.drawXbm(0, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  wireless_paper.update(BLACK_BUFFER);
  wireless_paper.display();
  
}
void VextON(void)
{
  pinMode(45, OUTPUT);
  digitalWrite(45, LOW);
}

void VextOFF(void) // Vext default OFF
{
  pinMode(45, OUTPUT);
  digitalWrite(45, HIGH);
}
void loop()
{
  server.handleClient(); // Handle requests from clients
  
}
