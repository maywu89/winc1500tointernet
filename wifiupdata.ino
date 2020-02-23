//Modified from esp8266
//Modified for Hackster.io project to the winc1500



//#include "ESP8266WiFi.h"
#include <WiFi101.h>
#include "DHT.h"
#define DHTPIN 2    // what digital pin we're connected to
                    //pin2 to D4 on esp board
#define mos_pin  A0
#define light_pin  A1

// Uncomment whatever DHT sensor type you're using!
//#define DHTTYPE DHT11  // DHT 11
#define DHTTYPE DHT21  // DHT 21
//#define DHTTYPE DHT22  // DHT 22

DHT dht(DHTPIN,DHTTYPE);


const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "vBE310683996E8F3"; //device ID from Pushingbox 

const char* MY_SSID = "MIT GUEST";
//const char* MY_PWD =  "<YOUR_WIFI_PASSWORD_HERE>";


void setup()
{
  WiFi.setPins(8,7,4);
  Serial.begin(115200);
  dht.begin();
  Serial.print("Connecting to "+*MY_SSID);
  WiFi.begin(MY_SSID);
  Serial.println("going into wl connect");

  while (WiFi.status() != WL_CONNECTED) //not connected,..waiting to connect
    {
      delay(1000);
      Serial.print(".");
    }
  Serial.println("wl connected");
  Serial.println("");
  Serial.println("Credentials accepted! Connected to wifi\n ");
  Serial.println("");
}


void loop()
{
  
  //Wait between measurements longer then normal to slow donwn
  //google sheet populate, as We dont want to exceed free service quota
  delay(10000); //10 seconds, (sampling rate vs. service call quota)

  float humidityData = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float celData = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float fehrData = dht.readTemperature(true);
  //READ MOISTURE
  float mos = analogRead(mos_pin);
  float light = analogRead(light_pin);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidityData) || isnan(celData))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  //float hicData = dht.computeHeatIndex(celData, humidityData, false);
  // Compute heat index in Fahrenheit (the default)
//  float hifData = dht.computeHeatIndex(fehrData, humidityData);

  //Print to Serial monitor or Terminal of your chocice at 115200 Baud
  Serial.print("Humidity: ");
  Serial.print(humidityData);
  Serial.print(" %\t");
  Serial.print("Temperature in Cel: ");
  Serial.print(celData);
  Serial.print(" *C ");
  Serial.print("Moisture: ");
  Serial.print(mos);
  Serial.print("units");
  Serial.print("Light");
  Serial.print(light);
  


    
  WiFiClient client;  //Instantiate WiFi object

    //Start or API service using our WiFi Client through PushingBox
    if (client.connect(WEBSITE, 80))
      { 
         client.print("GET /pushingbox?devid=" + devid
       + "&humidityData=" + (String) humidityData
       + "&celData="      + (String) celData
       + "&mos="     + (String) mos
       + "&light="      + (String) light
         );

      client.println(" HTTP/1.1"); 
      client.print("Host: ");
      client.println(WEBSITE);
      client.println("User-Agent: ESP8266/1.0");
      client.println("Connection: close");
      client.println();
      }
}
