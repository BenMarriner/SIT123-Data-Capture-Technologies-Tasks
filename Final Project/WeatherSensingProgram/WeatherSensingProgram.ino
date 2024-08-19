#include <SD.h>
#include <RTClib.h>
#include <DHT.h>

const int PIN_TEMPSENSOR = 2;
const int PIN_LEDRED = 3, PIN_LEDYELLOW = 4, PIN_LEDGREEN = 5, PIN_LEDHUMID = 6;
const int PIN_SDCARD = 10;
float humidity, temperature;
DHT tempSensor(PIN_TEMPSENSOR, DHT22);
RTC_DS1307 rtc;
DateTime now;
int sampleID;

void TurnOffTempLEDs()
{
  digitalWrite(PIN_LEDRED, LOW);
  digitalWrite(PIN_LEDYELLOW, LOW);
  digitalWrite(PIN_LEDGREEN, LOW);
}

void TurnOffHumidLED()
{
  digitalWrite(PIN_LEDHUMID, LOW);
}

int LEDFromTemperature()
{
  if (temperature < 14)
  {
    return PIN_LEDRED;
  }
  else if (temperature >= 14 && temperature < 20)
  {
    return PIN_LEDYELLOW;
  }
  else
  {
    return PIN_LEDGREEN;
  }
}

bool HighHumidity()
{
  if (humidity >= 70)
  {
    return HIGH;
  }
  return LOW;
}

void ConfigureTempLEDs()
{
  TurnOffTempLEDs();
  
  switch (LEDFromTemperature())
  {
    case PIN_LEDRED:
      digitalWrite(PIN_LEDRED, HIGH);
      break;
    case PIN_LEDYELLOW:
      digitalWrite(PIN_LEDYELLOW, HIGH);
      break;
    case PIN_LEDGREEN:
      digitalWrite(PIN_LEDGREEN, HIGH);
      break;
  }
}

void ConfigureHumidLED()
{
  digitalWrite(PIN_LEDHUMID, HighHumidity());
}

void ConfigureLEDs()
{
  ConfigureTempLEDs();
  ConfigureHumidLED();
}

void LEDTest()
{
  int highLED = PIN_LEDRED;
  do
  {
    TurnOffTempLEDs();
    TurnOffHumidLED();
    
    switch (highLED)
    {
      case PIN_LEDRED:
        digitalWrite(PIN_LEDRED, HIGH);
        break;
      case PIN_LEDYELLOW:
        digitalWrite(PIN_LEDYELLOW, HIGH);
        break;
      case PIN_LEDGREEN:
        digitalWrite(PIN_LEDGREEN, HIGH);
        break;
      case PIN_LEDHUMID:
        digitalWrite(PIN_LEDHUMID, HIGH);
        break;
    }
    highLED++;
    delay(250);
  }
  while (highLED != PIN_LEDHUMID + 1);

  digitalWrite(PIN_LEDRED, LOW);
  digitalWrite(PIN_LEDYELLOW, LOW);
  digitalWrite(PIN_LEDGREEN, LOW);
  digitalWrite(PIN_LEDHUMID, LOW);
}

void WriteDateTime(File &file)
{
  // Date
  file.print(now.day(), DEC);
  file.print("/");
  file.print(now.month(), DEC);
  file.print("/");
  file.print(now.year(), DEC);
  file.print(",");
  // Time
  file.print(now.hour(), DEC);
  file.print(":");
  file.print(now.minute(), DEC);
  file.print(":");
  file.print(now.second(), DEC);
  file.print(",");
}

void WriteTempSample()
{
  File temperaturesFile = SD.open("TEMP.CSV", FILE_WRITE);
  
  // Sample ID
  temperaturesFile.print(sampleID);
  temperaturesFile.print(",");

  // Date and Time
  WriteDateTime(temperaturesFile);
  
  // Temperature
  temperaturesFile.println(temperature);

  // Close file
  temperaturesFile.close();
}

void WriteHumSample()
{
  File humsFile = SD.open("HUM.CSV", FILE_WRITE);
  
  // Sample ID
  humsFile.print(sampleID);
  humsFile.print(",");
  
  // Date and Time
  WriteDateTime(humsFile);
  
  // Humidity
  humsFile.println(humidity);

  // Close file
  humsFile.close();
}

void setup() 
{
  Serial.begin(9600);
  rtc.begin();
  SD.begin(PIN_SDCARD);
  if (!SD.exists("TEMP.CSV"))
  {
    SD.open("TEMP.CSV").close();
  }
  if (!SD.exists("HUM.CSV"))
  {
    SD.open("HUM.CSV").close();
  }
  
  tempSensor.begin();

  sampleID = 1;
  
  pinMode(PIN_LEDRED, OUTPUT);
  pinMode(PIN_LEDYELLOW, OUTPUT);
  pinMode(PIN_LEDGREEN, OUTPUT);
  pinMode(PIN_LEDHUMID, OUTPUT);
  pinMode(PIN_SDCARD, OUTPUT);
  
  LEDTest();
}

void loop()
{
  humidity = tempSensor.readHumidity();
  temperature = tempSensor.readTemperature();
  now = rtc.now();

  ConfigureLEDs();
  WriteTempSample();
  WriteHumSample();
  sampleID++;

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(", Temperature: ");
  Serial.print(temperature);
  Serial.println();
  
  delay(60000);
}
