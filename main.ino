#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

int soundDetectedPin = 14;
int soundDetectedVal = HIGH;
boolean bAlarm = false;

unsigned long lastSoundDetectTime;

int soundAlarmTime = 500;

void setup()
{
  Serial.begin(115200);
  pinMode(soundDetectedPin, INPUT);
  wifiStart();
  Serial.println("=== INIT");
}

void loop()
{
  soundDetectedVal = digitalRead(soundDetectedPin);

  if (soundDetectedVal == LOW)
  {
    maybeStartAlarm();
  }
  else
  {
    maybeStopAlarm();
  }
}

void maybeStopAlarm()
{
  if ((millis() - lastSoundDetectTime) > soundAlarmTime && bAlarm)
  {
    Serial.println("=== EXIT ALARM STATE");
    bAlarm = false;
  }
}

void maybeStartAlarm()
{
  lastSoundDetectTime = millis();

  if (!bAlarm)
  {
    Serial.println("=== ENTER ALARM STATE");
    sendHttpAlert();
    bAlarm = true;
  }
}

const char *ssid = "FOX.BUILD";     // The SSID (name) of the Wi-Fi network you want to connect to
const char *password = "fox.build"; // The password of the Wi-Fi network

void wifiStart()
{
  delay(10);

  WiFi.begin(ssid, password); // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print('.');
  }

  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer
}

void sendHttpAlert()
{
  if (WiFi.status() == WL_CONNECTED)
  { //Check WiFi connection status

    HTTPClient http;

    http.begin("http://10.1.10.145:3000/blah");   //Specify destination for HTTP request
    http.addHeader("Content-Type", "text/plain"); //Specify content-type header

    int httpResponseCode = http.POST("POSTING from ESP32"); //Send the actual POST request

    if (httpResponseCode > 0)
    {

      String response = http.getString(); //Get the response to the request

      Serial.println(httpResponseCode); //Print return code
    }
    else
    {

      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end(); //Free resources
  }
  else
  {

    Serial.println("Error in WiFi connection");
  }
}
