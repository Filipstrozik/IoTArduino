#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);


#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3


#define BLINK_RED 900UL
#define BLINK_GREEN 1000UL
#define BLINK_BLUE 1100UL


const unsigned long blink[] = {BLINK_RED, BLINK_GREEN, BLINK_BLUE};
int led[] = {LED_RED, LED_GREEN, LED_BLUE};


void initRGB()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, HIGH);

    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_GREEN, LOW);

    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, LOW);
}

void initLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Zadanie 2");
}

void setup() {
    initRGB();
    initLCD();
}

#define DEBOUNCE_PERIOD 10UL

bool blinkLed(int& led_index, int& ledState, unsigned long& lastBlinkChange)
{
  	const unsigned long currentLedBlinkPeriod = blink[led_index];
  	
  if(millis() - lastBlinkChange >= currentLedBlinkPeriod)
  {
    if(ledState == HIGH)
    {
      ledState = LOW;
      digitalWrite(led[led_index], ledState);
    }
    else
    {
      ledState = HIGH;
      digitalWrite(led[led_index], ledState);
    }
    lastBlinkChange += currentLedBlinkPeriod;
  }
}



void blinkRed()
{   
    int led_inx = 0;
    static int ledState = LOW;
    static unsigned long lastBlinkChange = 0UL;
    blinkLed(led_inx, ledState, lastBlinkChange);
}

void blinkGreen()
{   
    int led_inx = 1;
    static int ledState = LOW;
    static unsigned long lastBlinkChange = 0UL;
    blinkLed(led_inx, ledState, lastBlinkChange);
}

void blinkBlue()
{   
    int led_inx = 2;
    static int ledState = LOW;
    static unsigned long lastBlinkChange = 0UL;
    blinkLed(led_inx, ledState, lastBlinkChange);
}

void loop() {
  blinkRed();
  blinkGreen();
  blinkBlue();
}