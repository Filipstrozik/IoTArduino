
// Zadanie 1: Program wykorzystujący czujniki DS18B20

// Przygotuj program, który uczyni zestaw laboratoryjny małą „stacją pogodową”. 
// Na wyświetlaczu LCD ma prezentować temperaturę wewnętrzną (czujnik wewnątrz zestawu) i 
// zewnętrzną (sonda na przewodzie). Ponadto program powinien pamiętać wartość maksymalną i 
// minimalną temperatury mierzonej przez sondę na przewodzie. Dodatkowo, dioda RGB, może
// sygnalizować czy jesteśmy w streﬁe komfortu temperaturowego, lub jest za gorąco, lub za zimno.
// Jest to zadanie przykładowe. Prowadzący może zmodyﬁkować lub zmienić treść zadania.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

OneWire oneWire(A1);
DallasTemperature tempSensors(&oneWire);

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3
int led[] = {LED_RED, LED_GREEN, LED_BLUE};

void initRGB() {
    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, LOW);

    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_GREEN, LOW);

    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, LOW);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void initLCD()
{
  	lcd.init();
    lcd.backlight();
  	lcd.print("Zadanie 1");
  	delay(1000);
}

void setup()
{   
    initRGB();
  	initLCD();
    tempSensors.begin();
	

    lcd.clear();
}

bool areWeInComfortZone(float tempOut) {
    return 26.0 < tempOut && tempOut < 31.0;
}

float tempIn = 0;
float tempOut = 0;
float tempOutMax = tempOut;
float tempOutMin = tempOut;

void updateMaxMin() {
    tempSensors.requestTemperatures();
    tempIn = tempSensors.getTempCByIndex(1);
    tempOut = tempSensors.getTempCByIndex(0);
    
    if (tempOutMax == 0 || tempOut > tempOutMax) {
        tempOutMax = tempOut;
    }

    if (tempOutMin == 0 || tempOut < tempOutMin) {
        tempOutMin = tempOut;
    }
}

void displayTemperatures() { // make display max and min
    char buffer[40];
    sprintf(buffer, "IN%4s", String(tempIn, 2).c_str());
    lcd.setCursor(0, 0);
    lcd.print(buffer);

    sprintf(buffer, "OU%4s", String(tempOut, 2).c_str());
    lcd.setCursor(0, 1);
    lcd.print(buffer);

    sprintf(buffer, "MIN%4s", String(tempOutMin, 2).c_str());
    lcd.setCursor(8, 0);
    lcd.print(buffer);

        sprintf(buffer, "MAX%4s", String(tempOutMax, 2).c_str());
    lcd.setCursor(8, 1);
    lcd.print(buffer);
}

void loop()
{
    updateMaxMin();
    displayTemperatures();
    
    if(areWeInComfortZone(tempOut)) {
        digitalWrite(LED_GREEN, HIGH);
    }
    else {
        digitalWrite(LED_GREEN, LOW);
    }
}