#include <Arduino.h>

int soundDetectedPin = 14;
int soundDetectedVal = HIGH;
boolean bAlarm = false;

unsigned long lastSoundDetectTime;

int soundAlarmTime = 500;

void setup()
{
  Serial.begin(115200);
  pinMode(soundDetectedPin, INPUT);
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
    bAlarm = true;
  }
}
