#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"

int soundDetectedPin = 14;
int soundDetectedVal = HIGH;
boolean bAlarm = false;

unsigned long lastSoundDetectTime;

int soundAlarmTime = 25;

void setup()
{
  Serial.begin(115200);
  pinMode(soundDetectedPin, INPUT);
  wifiStart();
  Serial.println("=== START");
}

void loop()
{
  digitalRead(soundDetectedPin) == LOW ? maybeStartAlarm() : maybeStopAlarm();
}

void maybeStopAlarm()
{
  unsigned long diff = millis() - lastSoundDetectTime;
  if ((diff > soundAlarmTime) && bAlarm)
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

void wifiStart()
{
  delay(10);
  Serial.print("Connecting to ");
  Serial.print(WOOF_WIFI_SSID);

#if defined(WOOF_WIFI_PASSWORD)
  WiFi.begin(WOOF_WIFI_SSID, WOOF_WIFI_PASSWORD);
#else
  Serial.println("...with no password.");
  WiFi.begin(WOOF_WIFI_SSID);
#endif

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

    http.begin(WOOF_POST_URL);
    http.addHeader("Content-Type", "text/plain");
    int httpResponseCode = http.POST("Woof!");

    if (httpResponseCode > 0)
    {
      Serial.println(http.getString()); //Show response in terminal (debug)
    }
    else
    {

      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
  else
  {
    Serial.println("WiFi FAILURE");
  }
}
