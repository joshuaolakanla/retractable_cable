//Code created by Joshua Olakanla jossytech
//Date uploaded 12-15-2022
//http://192.168.4.1/out1? nodemcu address
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <DNSServer.h>

const char WiFiPassword[] = "";//Leave Blank if you do not want to have a password to enter the Node
const char AP_NameChar[] = "JAndro" ;
char wifissid[] = "Jossy";
char password[] = "jossytecho";
const int RSSI_MAX =-50;// define maximum strength of signal in dBm
const int RSSI_MIN =-100;// define minimum strength of signal in dBm

WiFiServer server(80);
String request = "";
int wiper = 5;
int pusher = 4;
int in = 0;
int out = 2;
int remoteIn = 13;
int remoteOut = 14;
int remote_in;
int remote_out;
int app_mode = true;
int rssi_mode = false;
//bool toggle = true;
int scanDelayTime = 1000;
int wiFiDisplayTime = 3000;
int strength;




void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");

  if (app_mode == true){
    WiFi.disconnect();
    boolean conn = WiFi.softAP(AP_NameChar, WiFiPassword);
  }

  if (rssi_mode == true){
    WiFi.disconnect();
    WiFiManager wifiManager;
    wifiManager.autoConnect("Auto_AP");
    Serial.println("Connected....");
  }



  server.begin();
  pinMode(wiper, OUTPUT);
  pinMode(in, OUTPUT);
  pinMode(out, OUTPUT);
  pinMode(pusher, OUTPUT);
  pinMode(remoteIn, INPUT_PULLUP);
  pinMode(remoteOut, INPUT_PULLUP);
}







void loop() {
  remote_in = digitalRead(remoteIn);
  remote_out = digitalRead(remoteOut);
  




  //Check if it is in app mode
  WiFiClient client = server.available();
  if (app_mode == true) {
  if (!client)  {
    return;
  }
  request = client.readStringUntil('\r');
 }




  
  //Check if I should process the rssi
  if       (rssi_mode == true)  {   
    rssi_signal1();
    return;
  }
  
  

  //Remote and application section
  if       ( (request.indexOf("out1") > 0) || (remote_out == 0 ) )  {   
    Serial.println("out1");
    outpin();
  }
 else if  ( (request.indexOf("in1") > 0) || (remote_in == 0) ) {
    Serial.println("in1");
    inpin();
  }
  else {
    idle();
  }
  delay(5);
}
  



  


//Control pin output
void outpin() {
  Serial.println("Out pin activated!");
  analogWrite(wiper, 190);
  digitalWrite(out, 1);
  digitalWrite(pusher, 1);
      }


void inpin() {
  Serial.println("In pin activated!");
  digitalWrite(in, 1);
  digitalWrite(wiper, 1);
  analogWrite(pusher, 160);
  
  }
 
void idle() {
  Serial.println("Idle!!!");
  digitalWrite(in, 0);
  digitalWrite(out, 0);
}








//rssi processing section
void rssi_signal1(){
  Serial.print("Connected to: ");
  Serial.println(wifissid);
  Serial.print("Wifi signal strength is: ");
  int strength =dBmtoPercentage(WiFi.RSSI());
  Serial.println(strength); 
  if (strength < 70 && strength > 30) {
    outpin();
  }
  if (strength >= 70 && strength <90) {
    inpin();
  }
  if (strength >=90){
    idle();
  }
}

int dBmtoPercentage(int dBm)
{
  int quality;
    if(dBm <= RSSI_MIN)
    {
        quality = 0;
    }
    else if(dBm >= RSSI_MAX)
    {  
        quality = 100;
    }
    else
    {
        quality = 2 * (dBm + 100);
   }

     return quality;
}//dBmtoPercentage
