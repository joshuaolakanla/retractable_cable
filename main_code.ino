//Code created by Joshua Olakanla jossytech
//Date uploaded 12-15-2022
//http://192.168.4.1/out1? nodemcu address
//rx is for rssi mode
//pin D6 is for app mode
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <DNSServer.h>

const char WiFiPassword[] = "";//Leave Blank if you do not want to have a password to enter the Node
const char AP_NameChar[] = "Cable_Control" ;
char wifissid[] = "Jossy";
char password[] = "jossytecho";
const int RSSI_MAX =-50;// define maximum strength of signal in dBm
const int RSSI_MIN =-100;// define minimum strength of signal in dBm

WiFiServer server(80);
String request = "";
int pusher = 5;
int wiper = 3;
int in = 0; //D3
int out = 15; //D8
int remoteIn = 13;
int remoteOut = 14;
int remote_in;
int remote_out;
int app_mode = false;
int rssi_mode = true;
int scanDelayTime = 1000;
int wiFiDisplayTime = 3000;
int strength;
int app_trig = 4; //D2
int rssi_trig = 12; //D6
int app_signal;
int rssi_signal;
int limit = A0;
int lim_switch;
bool outpin_stop;
bool inpin_stop;






void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");
  pinMode(app_trig, INPUT_PULLUP);
  pinMode(rssi_trig, INPUT_PULLUP);
  pinMode(limit, INPUT);
  pinMode(in, OUTPUT);
  pinMode(out, OUTPUT);
  pinMode(pusher, OUTPUT);
  pinMode(remoteIn, INPUT_PULLUP);
  pinMode(remoteOut, INPUT_PULLUP);
  pinMode(wiper, OUTPUT);
  app_signal = digitalRead(app_trig);
  rssi_signal = digitalRead(rssi_trig);
  if (app_signal == false){
    Serial.println("App mode...");
    WiFi.disconnect();
    boolean conn = WiFi.softAP(AP_NameChar, WiFiPassword);
  }

  if (rssi_signal == false){
    idle();
    WiFi.disconnect();
    idle();
    WiFiManager wifiManager;
    idle();
    wifiManager.autoConnect("Wifi_Calibration");
  }
  


  server.begin();
  
}







void loop() {
  remote_in = digitalRead(remoteIn);
  remote_out = digitalRead(remoteOut);
  
  




  //Check if it is in app mode
  WiFiClient client = server.available();
  if (app_signal == false) {
  if (!client)  {
    return;
  }
  request = client.readStringUntil('\r');
 }

  

  lim_switch = analogRead(limit);
  if (lim_switch>900){
    outpin_stop = true;
  }
  else {
    outpin_stop = false;
  }
  
  if (lim_switch>400 && lim_switch< 700){
   inpin_stop = true;
  }
  else {
   inpin_stop = false;
  }


  
  //Check if I should process the rssi
  if       (rssi_signal == false)  {   
    rssi_signal1();
    return;
  }
  
  

  //Remote and application section
  if       ( ((request.indexOf("out1") > 0) || (remote_out == 0 )) && outpin_stop==false )  {   
    outpin();
  }
 else if   ( ((request.indexOf("in1") > 0) || (remote_in == 0)) && inpin_stop == false ) {
    inpin();
  }
  else {
    idle();
  }
  delay(5);
}
  

//rssi processing section
void rssi_signal1(){
  Serial.print("Connected to: ");
  Serial.println(wifissid);
  Serial.print("Wifi signal strength is: ");
  strength =dBmtoPercentage(WiFi.RSSI());

  lim_switch = analogRead(limit);
  if (lim_switch>900){
    outpin_stop = true;
  }
  else {
    outpin_stop = false;
  }
  
  if (lim_switch>400 && lim_switch< 700){
   inpin_stop = true;
  }
  else {
   inpin_stop = false;
  }


  if (strength > 10 && strength <= 79 && outpin_stop==false) {
    idle();
    outpin();
  }

  else if (strength >= 80 && strength <93 && inpin_stop==false) {
    idle();
    inpin();
  }
  else {
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



//Control pin output
//in
void outpin() {
  digitalWrite(out, 1);
  analogWrite(wiper, 90);
  digitalWrite(pusher, 1);
      }

//out
void inpin() {
  digitalWrite(in, 1);
  digitalWrite(wiper, 1);
  analogWrite(pusher, 110);
  
  }
 
void idle() {
  Serial.println("Idle!!!");
  digitalWrite(in, 0);
  digitalWrite(out, 0);
}
