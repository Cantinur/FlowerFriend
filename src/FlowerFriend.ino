#include "LiquidCrystal_I2C_Spark.h"
#include "DS18.h"
#include "pitches.h"

LiquidCrystal_I2C *lcd;

int pinWater = A0;
int pinLight = A1;
int pinFire = A2;

int pinAudio = D2;
DS18 sensor(D3);
int led = D6;

int water = 0;
double celcius = 0;
bool notifyUserAboutfire = false;
bool notifyUserAboutWater = false;
bool notifyUserAboutTemperature = false;

int melody[] = { NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

/******************************* Smoothing Sensor Data *************************************************/
const int numberOfReadings = 10;

// Light
int lightReadings[numberOfReadings];
int readLightIndex = 0;
int totalLight = 0;
int averageLight = 0;
int dailyHighestAverageLight = 0;

//Fire
int fireReadings[numberOfReadings];
int readFireIndex = 0;
int totalFire = 0;
int averageFireReading = 0;

void setup(void)
{
  pinMode(pinWater, INPUT);
  pinMode(led, OUTPUT);

  //Particle variables
  Particle.variable("temperature", &celcius, DOUBLE);
  Particle.variable("water", &water, INT);
  Particle.variable("light", &dailyHighestAverageLight, INT);
  Particle.variable("fire", &averageFireReading, INT);

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
    fireReadings[i] = 0;
  }
}

void loop()
{
  if (hasSensorDataChanged())
  {
    if (averageFireReading > 20) 
    {
      activateFireWorning();
    }
    updateLED();
    updateDisplay(); 
  }
  delay(100);
}


/********************************************* SENSOR ***************************************************/
bool hasSensorDataChanged()
{
  const int currentBestLigth = dailyHighestAverageLight;
  const bool waterChange = upadetWaterSensorData();
  const double currentTemp = celcius;
  
  updateTermostateData();
  updateLigthSensorData();
  updateFireSensorData();
  
  if ((currentBestLigth + 10 <= dailyHighestAverageLight) || (currentBestLigth - 10 >= dailyHighestAverageLight))
  {
    return true;
  } 
  else if ((currentTemp + 1.00 <= celcius) || (currentTemp - 1 >= celcius))
  {
    return true;
  }
  return waterChange;
}

bool upadetWaterSensorData()
{
  const int readWater = analogRead(pinWater)/100;
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
  totalLight = totalLight - lightReadings[readLightIndex];
  lightReadings[readLightIndex] = analogRead(pinLight);
  totalLight = totalLight + lightReadings[readLightIndex];
  readLightIndex = readLightIndex + 1;

  if (readLightIndex >= numberOfReadings) 
  {
    readLightIndex = 0;
  }

  averageLight = totalLight / numberOfReadings;

  if (Time.second() == 0 && Time.hour() == 0)
  {
    dailyHighestAverageLight = 0;
  }
  else if (dailyHighestAverageLight < averageLight)
  {
    dailyHighestAverageLight = averageLight;
  }
}

void updateFireSensorData()
{
  totalFire = totalFire - fireReadings[readFireIndex];
  fireReadings[readFireIndex] = analogRead(pinFire);
  totalFire = totalFire + fireReadings[readFireIndex];
  readFireIndex = readFireIndex + 1;

  if (readFireIndex >= numberOfReadings) 
  {
    readFireIndex = 0;
  }
  
  averageFireReading = totalFire / numberOfReadings;
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
  String message;
  if (water >= 40)
  {
    message = "Full";
  }
  else if (water >= 20)
  {
    message = "Good";
  }
  else if (water >= 10)
  {
    message = "OK";
  }
  else if (water >= 5)
  {
    message = "Low";
  }
  else
  {
    message = "None";
  }
  return "Water: " + message;
}

String displayLightStatus()
{
  String ligthMessage;
  if (dailyHighestAverageLight >= 3700)
  {
    ligthMessage = "A lot";
  }
  else if (dailyHighestAverageLight >= 3000)
  {
    ligthMessage = "Good";
  }
  else if (dailyHighestAverageLight >= 2000)
  {
    ligthMessage = "OK";
  }
  else if (dailyHighestAverageLight >= 1200)
  {
    ligthMessage = "Low";
  }
  else
  {
    ligthMessage = "Nothing";
  }
  return "Light: " + ligthMessage;
}

/********************************************* LED ***************************************************/
void updateLED()
{
  digitalWrite(led, (water <= 5) ? HIGH : LOW);
}

void activateFireWorning()
{
  notifyUserAboutfire = true;
  playAudio();
}

void playAudio()
{
  for (int thisNote = 0; thisNote < 8; thisNote++) 
  {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(pinAudio, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(pinAudio);
  }
}

/********************************************* CHECK STATUS ***************************************************/
// All of these functions has to take in a string, even thou I don't use them. 
// They also have to return a int

//Play note and blink light
int checkNetworkConnection(String extra)
{
  if (water >= 5)
  {
    digitalWrite(led, HIGH);
    playAudio();
    digitalWrite(led, LOW);
  }
  else 
  {
    digitalWrite(led, LOW);
    playAudio();
    digitalWrite(led, HIGH);
  }
  
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