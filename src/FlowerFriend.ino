#include "LiquidCrystal_I2C_Spark.h";

LiquidCrystal_I2C *lcd;
int led = D6;

/************************************ Water and water sensor ********************************************/
int lastSecond = 0;
int inputPinWater = A0;
int water = 0;

/******************************* For Smoothing Light Sensor*************************************************/
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int dailyHighestAverage = 0;
int inputPinLight = A1;

void setup(void)
{
  pinMode(inputPinWater, INPUT);
  pinMode(led, OUTPUT);

  lcd = new LiquidCrystal_I2C(0x27, 16, 2);
  lcd->init();
  lcd->backlight();
  lcd->clear();

  handleWaterLevleOnScreen();
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop(void)
{
  handleWaterLevle();
  sunSensor();
  delay(1000);
}

void handleWaterLevle()
{
  const int readWater = analogRead(inputPinWater);
  if (10 + water <= readWater)
  {
    water = readWater;
    handleWaterLevleOnScreen(); 
  } 
  else if (water - 10 >= readWater)
  {
    water = readWater;
    handleWaterLevleOnScreen(); 
  }
}

void handleWaterLevleOnScreen()
{  
  lcd->setCursor(1,0);
  lcd->clear();
  if (water > 1700)
  {
    digitalWrite(led, LOW);
    lcd->print("Water: Full");
  }
  else if (water > 1000)
  {
    digitalWrite(led, LOW);
    lcd->print("Water: Good");
  }
  else if (water > 500)
  {
    digitalWrite(led, LOW);
    lcd->print("Water: OK");
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

  if (readIndex >= numReadings) 
  {
    readIndex = 0;
  }

  average = total / numReadings;
  
  if (average > dailyHighestAverage)
  {
    dailyHighestAverage = average;
  }

  if (Time.hour() == 0)
  {
    dailyHighestAverage = 0;
  }

  lcd->setCursor(0,1);
  lcd->print(dailyHighestAverage);
}

