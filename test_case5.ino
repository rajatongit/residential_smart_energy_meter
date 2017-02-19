#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;
float conv_to_kWh(int count);

const int analogInPin = A0;  
const int analogOutPin = 9; 
int sensorValue = 0;        
int outputValue = 0;
int count_10=0;
int count_hour=0;
int count_30=0;
int resetPin = 12;
int count=0;
float diff_float=0;
unsigned long interval_10 = 20000;
unsigned long interval_hour = 930000; 
unsigned long previousMillis=0;
unsigned long previousMillis_10=0;
unsigned long previousMillis_hour=0;
unsigned long currentMillis=0;
int initial_count_10=0;
int diff_count_10=0;
int initial_count_hour=0;
int diff_count_hour=0;
float kWh_in_main=0; 
String thingSpeakAddress= "http://api.thingspeak.com/update?";
String writeAPIKey;
String tsfield1Name;
String request_string;
HTTPClient http;
void setup() {
  Serial.begin(9600);  
  Serial.println("reseted the arduino");
  WiFi.disconnect();
  delay(1000);
  Serial.println("START");
  WiFi.begin("apec");
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
  delay(300);
  Serial.println("...");
  }
  Serial.println("connected.");
  // initialize serial communications at 9600 bps:
}

void loop() {
  sensorValue = analogRead(analogInPin);
  analogWrite(analogOutPin, outputValue);
  if(sensorValue>400)
  {
    count++;
  }
  currentMillis = millis();
  if((unsigned long)(currentMillis-previousMillis_10) >= interval_10)
  {   
  
    diff_count_10=count-initial_count_10;
    Serial.println(diff_count_10);
    Serial.println("This was after 10 minutes");
    initial_count_10=count;
    Serial.println("10 minutes are over");
    previousMillis_10=currentMillis;
    diff_float=conv_to_kWh(diff_count_10);
    Serial.println(diff_float);
    if (client.connect("api.thingspeak.com",80)) 
      {
      writeAPIKey = "key=GARJ7I7HI53Y68CJ";
      tsfield1Name = "&field1=10";
      request_string = thingSpeakAddress;
      request_string += "key=";
      request_string += "GARJ7I7HI53Y68CJ";
      request_string += "&";
      request_string += "field1";
      request_string += "=";
      request_string += diff_float;
      http.begin(request_string);
      http.GET();
      http.end();
      }
  }
}
float conv_to_kWh(int count)
{
  float kWh;
  kWh = count*0.3125; //0.3125
  return kWh;
}

