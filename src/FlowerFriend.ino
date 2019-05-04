#include "LiquidCrystal_I2C_Spark.h"
#include "DS18.h"


LiquidCrystal_I2C *lcd;

DS18 sensor(D3);
int inputPinLight = A1;
int inputPinWater = A0;
int led = D6;

int water = 0;
double celcius = 0;
bool notifyUserAboutfire = false;
bool notifyUserAboutWater = false;
bool notifyUserAboutTemperature = false;


/******************************* Smoothing Light Sensor Data *************************************************/
const int numberOfReadings = 10;
int lightReadings[numberOfReadings];
int readIndex = 0;
int total = 0;
int dailyHighestAverageLight = 0;


void setup(void)
{
  pinMode(inputPinWater, INPUT);
  pinMode(led, OUTPUT);

  //Particle variables
  Particle.variable("temperature", &celcius, DOUBLE);
  Particle.variable("water", &water, INT);
  Particle.variable("light", &dailyHighestAverageLight, INT);

  //Particle functions
  Particle.function("checkNetworkConnection", checkNetworkConnection);
  Particle.function("checkFireStatus", checkFireStatus);
  Particle.function("checkWaterStatus", checkWaterStatus);
  Particle.function("checkTemperatureStatus", checkTemperatureStatus);

  //LCD SET UP
  lcd = new LiquidCrystal_I2C(0x27, 16, 2);
  lcd->init();
  lcd->backlight();
  lcd->clear();

  // Looping for Light sensor
  for (int i = 0; i < numberOfReadings; i++) 
  {
    lightReadings[i] = 0;
  }
}

void loop()
{
  if (hasSensorDataChanged())
  {
    updateLED();
    updateDisplay();  
  }
  delay(100);
}


/********************************************* SENSOR ***************************************************/
bool hasSensorDataChanged()
{
  const int currentBestLigth = dailyHighestAverageLight;
  const int waterChange = upadetWaterSensorData();
  const double currentTemp = celcius;
  updateTermostateData();
  
  updateLigthSensorData();
  if ((currentBestLigth + 10 <= dailyHighestAverageLight) || (currentBestLigth - 10 >= dailyHighestAverageLight))
  {
    return true;
  } 
  else if (currentTemp + 1.00 <= celcius){
    return true;
  }
  else if (currentTemp - 1 >= celcius)
  {
    return true;
  }

  return waterChange;
}

bool upadetWaterSensorData()
{
  const int readWater = analogRead(inputPinWater)/100;
  if (water != readWater)
  {
    water = readWater;
    if (water <= 5)
    {
      notifyUserAboutWater = true;
    }
    return true;
  } 
  return false;
}

void updateLigthSensorData()
{
  total = total - lightReadings[readIndex];
  lightReadings[readIndex] = analogRead(inputPinLight);
  total = total + lightReadings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numberOfReadings) 
  {
    readIndex = 0;
  }

  const int averageLight = total / numberOfReadings;

  if (Time.second() == 0 && Time.hour() == 0)
  {
    dailyHighestAverageLight = 0;
  }
  else if (dailyHighestAverageLight < averageLight)
  {
    dailyHighestAverageLight = averageLight;
  }
}

void updateTermostateData()
{
  if (sensor.read()) 
  {
    celcius = (double) sensor.celsius();
  }
}

/********************************************* DISPLAY ***************************************************/
void updateDisplay()
{
  lcd->clear();

  const String waterStatus = displayWaterStatus();
  lcd->setCursor(0,0);
  lcd->print(waterStatus);
  
  int intCelcius = (int) celcius;
  const double differanse = celcius - (double) intCelcius;
  int add = (differanse >= 0.6) ? 1 : 0;
  intCelcius += add;

  lcd->print(" ");
  lcd->print(intCelcius);
  lcd->print("*C");

  const String lightStatus = displayLightStatus();
  lcd->setCursor(0,1);
  lcd->print(lightStatus);
}

String displayWaterStatus()
{  
  String message = "Water: ";
  if (water >= 40)
  {
    message = message + "Full";
  }
  else if (water >= 20)
  {
    message = message + "Good";
  }
  else if (water >= 10)
  {
    message = message + "OK";
  }
  else if (water >= 5)
  {
    message = message + "Low";
  }
  else
  {
    message = message + "None";
  }
  return message;
}

String displayLightStatus()
{
  String message = "Light: ";
  if (dailyHighestAverageLight >= 3700)
  {
    message = message + "A lot";
  }
  else if (dailyHighestAverageLight >= 2000)
  {
    message = message + "Good";
  }
  else if (dailyHighestAverageLight >= 1200)
  {
    message = message + "OK";
  }
  else if (dailyHighestAverageLight >= 1200)
  {
    message = message + "Low";
  }
  else
  {
    message = message + "Nothing";
  }
  return message;
}

/********************************************* LED ***************************************************/
void updateLED()
{
  digitalWrite(led, (water <= 5) ? HIGH : LOW);
}

/********************************************* CHECK STATUS ***************************************************/
int checkNetworkConnection(String extra)
{
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  return 0;
}

int checkFireStatus(String extra)
{
  if (notifyUserAboutfire) 
  {
    notifyUserAboutfire = false;
    return 1;
  }
  return 0;
}

int checkWaterStatus(String extra)
{
  if (notifyUserAboutWater)
  {
    notifyUserAboutWater = false;
    return 1;
  }
  return 0;
}

int checkTemperatureStatus(String extra)
{
  if (notifyUserAboutTemperature)
  {
    notifyUserAboutTemperature = false;
    return 1;
  }
  return 0;
}