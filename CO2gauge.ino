
/*
 *  This sketch sends data via HTTP GET requests to $host.
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

//ESP PWM RANGE: 1023
//GAUGE PWM RANGE: 0-540ish
  
const int GAUGE_PWM_MAX=540; //find your own
int gaugePin = D1;

const char* ssid     = "top";
const char* password = "secret";

//https://api.co2signal.com/v1/latest?countryCode=DK-DK1 -H 'auth-token: XxXxXxXxXxXxXxXx'

const String host = "api.co2signal.com";
const String url = "/v1/latest?countryCode=DK-DK1";
const String authToken = "XxXxXxXxXxXxXxXx"

//json stuff:
const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(5) + 310;
DynamicJsonDocument doc(capacity);

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println("Boop");
  pinMode(gaugePin, OUTPUT);
 
  for(int i=0;i<541;i++){
  analogWrite(gaugePin,i);
  //Serial.print(".");
  delay(5);
  }  
 
}

float fossilFuelPercentage=0.0;

void loop() {

    // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status()!= WL_CONNECTED) {delay(500); Serial.print(".");}
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClientSecure client;
  client.setInsecure(); //eliminate the need for at certificate fingerprint... but allows an attacker to impersonate the webserver...
  const int httpsPort = 443;
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.println("GET " + url + " HTTP/1.1"); 
  client.println("Host: " + host);
  client.println("Accept: */*");
  client.println("auth-token: " + authToken);
  //client.println("Connection: close");
  client.println(); //end of header
  
  delay(1000); //wait a while to let the server respond (increase on shitty connections)

  // Read all the lines of the reply from server
  String line;
  while(client.available()){

    yield();
    String line = client.readStringUntil('\n'); //newline return as delimiter
    //Serial.println(line);
    if(line.startsWith("{\"_disclaimer\":")) {
      deserializeJson(doc, line); //if the current line is the json string we want, parse it to make a json object.    
      Serial.println("found json!");
      fossilFuelPercentage = doc["data"]["fossilFuelPercentage"]; 
      //Serial.println("fossilFuelPercentage: " + String(data_fossilFuelPercentage));
    }
  }

  if(fossilFuelPercentage!=0.0) { //if data exists
    Serial.println("Fossil fuel percentage: " + String(fossilFuelPercentage));
    float renewablePercentage=100.0-fossilFuelPercentage;
    int pwmVal=int(float(GAUGE_PWM_MAX)*(renewablePercentage/100.0));
    Serial.println("pwmVal: " + String(pwmVal));
    analogWrite(gaugePin,pwmVal);
  }

  else Serial.println("fossilFuelPercentage at 0.0.. weird..");
  Serial.println("closing connection");

  client.stop();

  Serial.println("Turning off wifi");
  WiFi.disconnect();

  delay(15*60*1000); //wait 15 minutes... API rate limit is: 1/sec and 30/hour
}
