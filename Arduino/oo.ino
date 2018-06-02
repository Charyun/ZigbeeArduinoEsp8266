#include "esp8266.h"
#include "SoftwareSerial.h"
#include <WiFi.h>

Esp8266 wifi;

char ssid[] = "菅腾霄的 iPhone"; //  your network SSID (name)
char password[] = "1234567890";    // your network password (use for WPA, or use as key for WEP)

//char ssid[] = "623"; //  your network SSID (name)
//char password[] = "liuersan110...";    // your network password (use for WPA, or use as key for WEP)

#define USERKEY  "ccdafc995f074e28bc45762035e31413" // replace your key here  
char server[] = "open.lewei50.com";   // name address for cosm API  
  String ip_addr;
SoftwareSerial mySerial(10, 11); // RX, TX
  int i = 30;

void setup() {
  delay(5000);
  Serial.begin(115200);
  Serial1.begin(115200);
  mySerial.begin(115200); 
  
  wifi.begin(&Serial, &mySerial);   //Serial is used to communicate with esp8266 module, mySerial is used to debug

  while (!wifi.connectAP(ssid, password)) {
    wifi.debugPrintln("connect ap fail !");
    //while (true);
  }
  
    wifi.debugPrintln("connect ap sucessful !");
  
  wifi.debugPrintln("");
  wifi.debugPrintln("WiFi connected");


  ip_addr = wifi.getIP();
  wifi.debugPrintln("esp8266 ip:" + ip_addr);
  wifi.setSingleConnect();
 
}

void loop() {
  int len = 0;
  String value, result, tem, hum, pm;
  String incomingData;
  int n = 5;
  
//  String ip_addr;
//  ip_addr = wifi.getIP();
  wifi.debugPrintln("esp8266 ip:" + ip_addr);
  
  while(n){
    if (wifi.connectTCPServer(server, "80")) {  
      wifi.debugPrintln("connecting successful");
      break;
    }
    delay(200);
    n--;  
    if(n == 0){
      wifi.debugPrintln("connecting fail");
    }
  }
  value = readLine();
  result = value.substring(3, value.length());
  
  tem = result.substring(1, 3);
  hum = result.substring(5, 7);
  pm = result.substring(9, 12);
  wifi.debugPrintln("tem: "+ tem + " hum: " + hum + " pm: " + pm);
  wifi.debugPrintln("");
//  len = 24 * 3 + 2 + 2 + 2 + 3;
  len = 83;
  wifi.debugPrintln("");

  
  /*
POST /api/V1/gateway/Updatesensors/02 HTTP/1.1
userkey:ccdafc995f074e28bc45762035e31413
Host: open.lewei50.com
Content-Length:28
Connection: close

[{"Name":"T1","Value":"61"}]
*/
  String postRequest =(String)"POST /api/V1/gateway/Updatesensors/02 HTTP/1.1\r\n" +  
                       "userkey:" + USERKEY + "\r\n" +      
                      "Host: open.lewei50.com \r\n" +
                      "Content-Length: " + len + "\r\n" +  
                      "Connection: close\r\n\r\n" +  
                      "[{\"Name\":\"T1\",\"Value\":\"" + tem +"\"},{\"Name\":\"H1\",\"Value\":\"" + hum +"\"},{\"Name\":\"P3\",\"Value\":\"" + pm +"\"}]\r\n";  
//  wifi.debugPrintln("postRequest:" + postRequest);

  String data = wifi.sendMessage(postRequest);
  wifi.setState(WIFI_IDLE);

  int state = wifi.getState();
  switch (state) {
      case WIFI_NEW_MESSAGE: 
        wifi.debugPrintln("new message!");
        incomingData = wifi.getMessage();
        wifi.debugPrintln("new message:"+incomingData);
        wifi.setState(WIFI_IDLE);
        break;
      case WIFI_CLOSED :              //reconnet to the TCP server 
        wifi.debugPrintln("server is closed! and trying to reconnect it!");
        if (wifi.connectTCPServer(server, "80")) {
          wifi.debugPrintln("reconnect OK!");
          wifi.setState(WIFI_IDLE);
        }
        else {
          wifi.debugPrintln("reconnect fail");
          wifi.setState(WIFI_CLOSED);
        }
        break;
      case WIFI_IDLE :              
        int sta = wifi.checkMessage(); 
        wifi.setState(sta);
        break;
  }
  i++;
  
  delay(9000);
}

String readLine()
{
  String str;
  int i = 0;
   char temp;
  while (Serial1.available())
  {
    temp = Serial1.read();
    str =  "" + str + temp;
    i++;
  }
  return str;
}
