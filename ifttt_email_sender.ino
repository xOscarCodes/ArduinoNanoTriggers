#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

//please enter your sensitive data in the Secret tab
char ssid[] = SECRET_SSID;                // your network SSID (name)
char pass[] = SECRET_PASS;                // your network password (use for WPA, or use as key for WEP)

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger/Sunlight/with/key/cNaSNaDJm2fxJ-PUJslA7z-70gQV9dkqFTJ9vaQKNdT"; // change your EVENT-NAME and YOUR-KEY
String queryString = "?value1=57&value2=25";

String PATH_NAME1   = "/trigger/Dark/with/key/cNaSNaDJm2fxJ-PUJslA7z-70gQV9dkqFTJ9vaQKNdT"; // change your EVENT-NAME and YOUR-KEY

int dark = 0;

void setup() {
  WiFi.begin(ssid, pass);
  
  Serial.begin(9600);
  
  Wire.begin();
  
  lightMeter.begin();
  
  while(!Serial);
  
  if (client.connect(HOST_NAME, 80)){
    Serial.println("Connected to server");
  }
  else {
    Serial.println("Connection Failed");
  }
  
}

void loop() {
  
  float lux = lightMeter.readLightLevel();
  
  if (lux < 100 && dark == 0)
  {
    dark = 1;
    send_dark();
  }
  else if (lux > 100 && dark == 1)
  {
    dark = 0;
    send_sunlight();
  }
  
}

void send_sunlight()
{
  client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println();
  
  while(client.connected()){
    if (client.available()){
      char c = client.read();
      Serial.print(c);
    }
  }
  
  client.stop();
  Serial.println();
  Serial.println("disconnected");
}

void send_dark()
{
  client.println("GET " + PATH_NAME1 + queryString + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println();
  
  while(client.connected()){
    if (client.available()){
      char c = client.read();
      Serial.print(c);
    }
  }
  
  client.stop();
  Serial.println();
  Serial.println("disconnected"); 
}
