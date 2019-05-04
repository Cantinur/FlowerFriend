#include "Adafruit_Sensor.h"
#include "Adafruit_DHT.h"
#include "LiquidCrystal_I2C_Spark.h"

LiquidCrystal_I2C *lcd;
int led = D6;

/************************************ Water and water sensor ********************************************/
#define DHTPIN 2
#define DHTTYPE DHT11	
DHT dht(DHTPIN, DHTTYPE);

/************************************ Water and water sensor ********************************************/
int lastSecond = 0;
int inputPinWater = A0;
int water = 0;

/******************************* For Smoothing Light Sensor Data*************************************************/
const int numberOfReadings = 10;
int readings[numberOfReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int dailyHighestAverage = 0;
int inputPinLight = A1;

void setup(void)
{
  pinMode(inputPinWater, INPUT);
  pinMode(led, OUTPUT);

  dht.begin();

  //LCD SET UP
  lcd = new LiquidCrystal_I2C(0x27, 16, 2);
  lcd->init();
  lcd->backlight();
  lcd->clear();

  // Looping for Light sensor
  for (int thisReading = 0; thisReading < numberOfReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  //Particle.function("WaterLevel")

  // Make sure something is displayed
  handleWaterLevelOnScreen();
  
}

void loop(void)
{
  handleWaterLevel();
  sunSensor();
  delay(1000);

  float h = dht.getHumidity();
// Read temperature as Celsius
	float t = dht.getTempCelcius();

  lcd->setCursor(0,1);
  lcd->print(t);
  lcd->print(F("°C"));
  delay(5000);

  lcd->setCursor(0,1);
  lcd->print(h);
  lcd->print(F("%"));
  delay(5000);
}

int getCurrentWaterLevel()
{
  return analogRead(inputPinWater)/100;
}

void handleWaterLevel()
{
  const int readWater = getCurrentWaterLevel();
  if (water != readWater)
  {
    water = readWater;
    handleWaterLevelOnScreen(); 
  } 
}

void handleWaterLevelOnScreen()
{  
  lcd->setCursor(1,0);
  lcd->clear();
  if (water >= 40)
  {
    digitalWrite(led, LOW);
    lcd->print("Water: Full");
  }
  else if (water >= 20)
  {
    digitalWrite(led, LOW);
    lcd->print("Water: Good");
  }
  else if (water >= 10)
  {
    digitalWrite(led, LOW);
    lcd->print("Water: OK");
  }
  else if (water >= 5)
  {
    digitalWrite(led, LOW);
    lcd->print("Water: Low");
  }
  else
  {
    digitalWrite(led, HIGH);
    lcd->print("Water: Bad");
  }

  //MARK:- TEST display int from water level
  //lcd->setCursor(0,1);
  //lcd->print(water);
}

void sunSensor()
{
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(inputPinLight);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numberOfReadings) 
  {
    readIndex = 0;
  }

  average = total / numberOfReadings;
  
  if (average > dailyHighestAverage)
  {
    dailyHighestAverage = average;
  }

  if (Time.hour() == 0)
  {
    dailyHighestAverage = 0;
  }

  lcd->setCursor(0,1);
  lcd->print(average);
}
