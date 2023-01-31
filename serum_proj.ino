#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

  //ENTER YOUR WIFI password
#define WIFI_SSID "CANALBOX-3A88"
#define WIFI_PASSWORD "0856936250"


// defines pins numbers

const int trigPin = D5;  //D4
const int echoPin = D6;   //D3

// defines variables
long duration;
int distance;
int z;

void setup() { 
  Serial.begin(9600);  
  // connectWifi();
    // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting"); 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".try again");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input
  
       // Starts the serial communication

  Serial.println("Welcome to Serum Monitoring System \n");
}

void loop() {

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor

  delay(2000);
  if (distance >= 0 && distance <= 10) {
    Serial.print("levels: ");
    z = 100 - distance * 10;
    Serial.print(z);
    SendSensorData(z);
    Serial.println("%");
  }
}
void connectWifi() {
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting"); 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".try again");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}
void SendSensorData(int level) {
  HTTPClient http;
  WiFiClient client;  //Declare object of class HTTPClient
  String serum, postData;
  serum = "ser9736h";

  //Post Data
  postData = "code=" + serum + "&level=" + level;

  http.begin(client, "http://192.168.1.68:8080/api/v1/user/update/634b95b472748f57b56ca4f2?serum.level=" + level);  //change the ip to your computer ip address
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");                                //Specify content-type header

  int httpCode = http.GET();          //Send the request
  String payload = http.getString();  //Get the response payload

  Serial.println(httpCode);  //Print HTTP return code
  Serial.println(payload);   //Print request response payload

  http.end();  //Close connection

  delay(5000);  //Post Data at every 5 seconds
}