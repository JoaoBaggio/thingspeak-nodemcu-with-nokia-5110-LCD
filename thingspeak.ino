/*********************************************************************
This is an example sketch for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

// MIT License
// Copyright (c) 2016 Michiel van Hoorn
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Wifi
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

WiFiClient client;

// Network configuration - use your own data
const char* ssid = "SSID here";
const char* password = "wifi password here";

const char* thingspeakServer = "api.thingspeak.com";
String apikey = "thingspeak api write key here";

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


/*********************************************************************
Using modified version of Adafruit's PCD8544 library.
https://github.com/bbx10/Adafruit-PCD8544-Nokia-5110-LCD-library/tree/esp8266

Inspired onf bbx10 (https://github.com/bbx10) work and improved for NodeMCU.

== Display Connection ==

USB TTL     Node      Nokia 5110  Description
            MCU     PCD8544

            GND         GND         Ground
            3V          VCC         3.3V from NodeMCU to display
            D5          CLK         Output from ESP SPI clock
            D7          DIN         Output from ESP SPI MOSI to display data input
            D6          D/C         Output from display data/command to ESP
            D1          CS          Output from ESP to chip select/enable display
            D2          RST         Output from ESP to reset display
            15          LED         3.3V to turn backlight on, GND off

GND (blk)   GND                     Ground
5V  (red)   V+                      5V power from PC or charger
TX  (green) RX                      Serial data from IDE to ESP
RX  (white) TX                      Serial data to ESP from IDE
*********************************************************************/

Adafruit_PCD8544 display = Adafruit_PCD8544(12, 5, 4);
// display.println(char(247));

uint32_t timer;
int timeResolution = 1000;

#include "DHT.h"
DHT dht(D3, DHT22); //DHT22 sensor using D3 for data pin

char charBuf[20];
char charBufssid[20];

int prop[]={47, 45, 92, 124};
int beat = 0;

  //variable set
    int counter = 0;
    int countDown = 300; //desired 900 = 15 minutes
    
void setup()   {
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
     }
  
  display.begin();
  display.clearDisplay(); //
  display.display();

  
  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);
  
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.setCursor(0,0);
  display.println(" wifi setup ");
  display.setTextColor(BLACK);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
         display.setTextColor(BLACK);
         display.setCursor(0,10);
         display.println("Connecting...");
         delay(500);
        Serial.print(".");
        }     
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Connecting...");
  display.setTextColor(BLACK);
               
  display.setCursor(0,10);
  display.println("Connected");
  display.display();
  
  // create IP addres scroll string
  String ipaddr = "   ";
  ipaddr += WiFi.localIP().toString();
  ipaddr += "  /  ";
  int ipScrollLenght = ipaddr.length();
  ipaddr.toCharArray(charBuf, 50);
  String ipPrint = "";  
  int scrollCountIP = 0;
  
  // create SSID scroll string
  String ssidString = "   ";
  ssidString += String(ssid);
  ssidString += "  /  ";
  int ssidScrollLenght = ssidString.length();
  ssidString.toCharArray(charBufssid, 50);
  String ssidPrint = "";  
  int scrollCountssid = 0;
  

   
  for(int c=0; c<50; c++){    // set the length of the setup period
  
  display.setTextColor(BLACK);

  // obtain IPaddr string view
  for(int x=scrollCountIP; x<(6+scrollCountIP); x++)
  {
    ipPrint += charBuf[x];
  }
  
  // obtain SSID string view
  for(int x=scrollCountssid; x<(6+scrollCountssid); x++)
  {
    ssidPrint += charBufssid[x];
  }

  display.setCursor(0,20);
  display.println("SSID: ");
  display.setCursor(30,20);
  display.println(ssidPrint);

  display.setCursor(0,30);
  display.println("IP: ");
  display.setCursor(20,30);
  display.println(ipPrint);

  
  display.setCursor(0,40);
  String strenght = "Field=";
  strenght += String(WiFi.RSSI());
  strenght += "dBM";
  display.println(strenght);
  display.display();

  

  
  
  display.display();

  
  
  delay(500);
  // clean
    
  display.setTextColor(WHITE);

  display.setCursor(30,20);
  display.println(ssidPrint);
  
  display.setCursor(20,30);
  display.println(ipPrint);
  
  display.setCursor(0,40);
  display.println(strenght);
  display.display();
  ipPrint = "";
  ssidPrint = "";
  
  scrollCountIP++;
  scrollCountssid++;
  
  if (scrollCountIP>=(ipScrollLenght-1)){
    scrollCountIP = 0;
   
  }
  if (scrollCountssid>=(ssidScrollLenght-1)){
    scrollCountssid = 0;
  
  }
  
  }

  
}


void loop() {


 if (millis() > timer) {
    timer = millis() + timeResolution; 

    
     display.clearDisplay();
     display.setTextColor(WHITE, BLACK); // 'inverted' text
     display.setCursor(0,0);
     display.println("   Monitor   ");
     display.setTextColor(BLACK);

     String temp = "T=";
     temp += String(dht.readTemperature());
     temp += char(247);
     temp += "C";
     display.setTextSize(1);
     display.setCursor(0,10);
     display.println(temp);
     display.setTextSize(1);

     String hum = "H=";
     hum += String(dht.readHumidity());
     hum += "%";
     display.setCursor(0,20);
     display.println(hum);

     if(beat==0){
      display.setCursor(0,40);
      display.println(String(char(176)));
      }

      if(beat==1){
      display.setCursor(0,40);
      display.println(String(char(177)));
      }
     display.display();

     
     if(counter>=countDown){
      //Serial.println("");
      counter = 0;
      thingSpeak();
     }

     
     else{
      counter++;
      //Serial.print(counter);
      //Serial.print(" ");
      beat++;
      if(beat>2){
        beat=0;
      }
      
     }
     
   }
  
}


void thingSpeak(){
  WiFi.disconnect(); //close any open connection
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.setCursor(0,0);
  display.println("  Send Data  ");
  display.setTextColor(BLACK);
  display.setCursor(0,10);
  display.println("Connecting");
  display.display();
  
  delay(1000);

    WiFi.begin(ssid, password);
    int subCount = 0;
    
    
    while (WiFi.status() != WL_CONNECTED) {
        display.setTextColor(BLACK);
        display.setCursor(30,30);
        display.println(String(char(subCount[prop])));
        display.display(); 
         
        delay(200);

        display.setTextColor(WHITE);
        display.setCursor(30,30);
        display.println(String(char(subCount[prop])));
        display.display(); 
        subCount++;
        if(subCount>4){
          subCount=0;
        }
        }   
        

    double humidity = dht.readHumidity();
    double temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    }
    else {

    if (client.connect(thingspeakServer, 80)){
    String getJSON = "";
    getJSON += "GET http://" + (String) thingspeakServer;
    getJSON += "/update.json?api_key=" + (String) apikey;
    getJSON += "&field1=" + (String) humidity;
    getJSON += "&field2=" + (String) temperature + "\n";
    getJSON += "Connection: close\n\n";
    client.print(getJSON);
    
    

    
    Serial.println(getJSON);
    } 
      else
      {
      Serial.println("Connection to Thingspeak failed");
      }

      
      client.stop();
    }

    
    
    WiFi.disconnect();

    display.clearDisplay();
    display.setTextColor(WHITE, BLACK); // 'inverted' text
    display.setCursor(0,0);
    display.println("  Send Data  ");
    display.setTextColor(BLACK);
    
    display.setCursor(15,30);
    display.println("Finished");
    display.display();

    delay(2000);
}





