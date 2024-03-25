#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

int waterLevelPin = 7;
int waterTankValue = 0;

int relayPin = 10;

int soilSensorPin = A0;
int moistureValue = 0; 

int photoresistorPin = A1;
int lightValue = 0;

// Emoji Bitcode
static const uint8_t PROGMEM
  rainCloud[] =
  { B11000011,
    B10000001,
    B11000011,
    B11111111,
    B11011011,
    B11110111,
    B11011111,
    B11111011 },
  // Single-Colored Cactus    
  cactus[] = 
  { B00000000,
    B00000000,
    B00011000,
    B00011100,
    B00111000,
    B00011000,
    B11111111,
    B11111111 },  
  // Bi-Colored Cactus
  // -----------------------------------------
  cactus_1[] = // Cactus Plant
  { B00000000,
    B00000000,
    B00011000,
    B00011100,
    B00111000,
    B00011000,
    B00000000,
    B00000000 },   
  cactus_2[] = // Ground
  { B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B11111111 },
  // -----------------------------------------
  bigSun[] =
  { B01010100,
    B00000001,
    B10111100,
    B00111101,
    B10111100,
    B00111101,
    B10000000,
    B00101010 },
  moon[] =
  { B11111111,
    B11100111,
    B11001111,
    B11011111,
    B11011111,
    B11001111,
    B11100111,
    B11111111 };
  // ENTER YOUR EMOJI BITCODE ABOVE

void setup() {
 Serial.begin(9600);

 pinMode(relayPin, OUTPUT);
 pinMode(waterLevelPin, INPUT_PULLUP);
 digitalWrite(relayPin, HIGH); // start with water pump off

 matrix.begin(0x70); // pass in the matrix address
 matrix.setRotation(3); // rotate display
}

void loop() {
  // -------------------- CHECK SOIL ---------------------------
  moistureValue = analogRead(soilSensorPin);
  moistureValue = map(moistureValue, 0, 1023, 0, 100);

  // print the soil moisture value
  Serial.print("Soil Moisture Value : ");
  Serial.println(moistureValue);

  // -------------------- IF SOIL IS DRY -----------------------
  // print the status of the soil
  // values based on pothos plant
  if (moistureValue > 50) {
    Serial.println("Status: Soil moisture is too dry - time to water!");  
    // display BI-COLORED CACTUS
    matrix.clear();
    matrix.drawBitmap(0, 0, cactus_1, 8, 8, LED_GREEN);
    matrix.drawBitmap(0, 0, cactus_2, 8, 8, LED_YELLOW);
    matrix.writeDisplay();

    // -------------------- CHECK WATER TANK --------------------
    // checking the resevoir levels to make sure there is enough water:
    waterTankValue = digitalRead(waterLevelPin);

    Serial.print("Water Tank Value = ");
    Serial.println(waterTankValue);

    // if water tank is empty, keep pump off
    if (waterTankValue == 1) {
      digitalWrite(relayPin, HIGH);
      Serial.println("WATER PUMP OFF");
      delay(1000);
      // display 'WATER TANK IS EMPTY' emoji
      matrix.clear();
      matrix.drawBitmap(0, 0, empty, 8, 8, LED_RED);
      matrix.writeDisplay();
      delay(10000);
    // if water tank has enough water, turn on pump
    } else {
      Serial.println("WATER PUMP ON");
      digitalWrite(relayPin, LOW);
      delay(1500);
      digitalWrite(relayPin, HIGH);
      delay(3000);
    }
  // -------------------- IF SOIL IS TOO WET -----------------------
  } else if (moistureValue < 30) {
    Serial.println("Status: Soil is too wet");
    // display RAIN CLOUD
    matrix.clear();
    matrix.drawBitmap(0, 0, rainCloud, 8, 8, LED_YELLOW);
    matrix.writeDisplay();
  // -------------------- IF SOIL IS PERFECT -----------------------
  } else {
    Serial.println("Status: Soil moisture is perfect");

    // -------------------- CHECK SUNLIGHT ---------------------------
    // read sunlight value through the photoresistor
    lightValue = analogRead(photoresistorPin);

    // print the sunlight value
    Serial.print("Sunlight Value: ");
    Serial.println(lightValue);
    // -------------------- IF TOO DARK -------------------------
    if (lightValue < 250) {
      Serial.println("Status: There is not enough sunlight");
      // display MOON
      matrix.clear();
      matrix.drawBitmap(0, 0, moon, 8, 8, LED_YELLOW);
      matrix.writeDisplay();
    // -------------------- IF TOO BRIGHT -----------------------
    } else if (lightValue > 750) {
      Serial.println("Status: This area is too bright");
      // display SUN
      matrix.clear();
      matrix.drawBitmap(0, 0, bigSun, 8, 8, LED_YELLOW);
      matrix.writeDisplay();
    // -------------------- IF PERFECT --------------------------
    } else {
      Serial.println("Status: The sunlight is perfect!");
      // display "PERFECT" emoji
      matrix.clear();
      matrix.drawBitmap(0, 0, perfect, 8, 8, LED_GREEN);
      matrix.writeDisplay();
    }
  delay(60000); // wait a min before checking soil again
  }
}
