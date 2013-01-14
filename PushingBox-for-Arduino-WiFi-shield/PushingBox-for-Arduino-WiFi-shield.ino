////
//
// General code from http://www.pushingbox.com for Arduino WiFI shield (official) v1.0
//
////

#include <WiFi.h>
 
  /////////////////
 // MODIFY HERE //
/////////////////
char wifissid[] = "WIFI_SSID";   //  your network SSID (name)
char wifipass[] = "WIFI_PASSWORD";    // your WPA network password

char DEVID1[] = "Your_DevID_Here";        //Scenario : "The mailbox is open"

//Numeric Pin where you connect your switch
uint8_t pinDevid1 = 3; // Example : the mailbox switch is connect to the Pin 3

// Debug mode
boolean DEBUG = true;
  //////////////
 //   End    //
//////////////


char serverName[] = "api.pushingbox.com";
boolean pinDevid1State = false;                // Save the last state of the Pin for DEVID1
boolean lastConnected = false;  
int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiClient client;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(pinDevid1, INPUT);
 
  // attempt to connect using WPA2 encryption:
  Serial.println("Attempting to connect to WPA network...");
  status = WiFi.begin(wifissid, wifipass);
 
  // if you're not connected, stop here:
  if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    while(true);
  }
  // if you are connected, print out info about the connection:
  else {
    Serial.println("Connected to network");
  }
}
 
void loop() {
   ////
   // Listening for the pinDevid1 state
   ////
  if (digitalRead(pinDevid1) == HIGH && pinDevid1State == false) // switch on pinDevid1 is ON
  {
    if(DEBUG){Serial.println("pinDevid1 is HIGH");}
    pinDevid1State = true;
    //Sending request to PushingBox when the pin is HIGHT
    sendToPushingBox(DEVID1);
  }
   if (digitalRead(pinDevid1) == LOW && pinDevid1State == true) // switch on pinDevid1 is OFF
  {
    if(DEBUG){Serial.println("pinDevid1 is LOW");}
    pinDevid1State = false;
    //Sending request to PushingBox when the pin is LOW
    //sendToPushingBox(DEVID1);    //Here you can run an other scenario by creating a DEVID2 variable
  }
 
  //DEBUG part
  // this write the respons from PushingBox Server.
  // You should see a "200 OK"
  if (client.available()) {
    char c = client.read();
    if(DEBUG){Serial.print(c);}
  }
 
  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    if(DEBUG){Serial.println();}
    if(DEBUG){Serial.println("disconnecting.");}
    client.stop();
  }
  lastConnected = client.connected();
}


//Function for sending the request to PushingBox
void sendToPushingBox(char devid[]){
  client.stop(); if(DEBUG){Serial.println("connecting...");}
  if(client.connect(serverName, 80)) { 
    if(DEBUG){Serial.println("connected");}
    if(DEBUG){Serial.println("sendind request");}
    client.print("GET /pushingbox?devid=");
    client.print(devid);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(serverName);
    client.println("User-Agent: Arduino");
    client.println();
  } 
  else { 
    if(DEBUG){Serial.println("connection failed");} 
  } 
}
