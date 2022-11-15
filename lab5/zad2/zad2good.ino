
// Przygotuj program, który będzie pozwalał sterować świeceniem diody RGB. Program ma być 
// wyposażony w przewijalne menu na ekranie wyświetlacza LCD (w kolejnych liniach wyświetlane 
// są dwie pozycje z menu). Samodzielnie zaprojektuj strukturę menu programu. Nawigacja po 
// menu (przemieszczanie się pomiędzy pozycjami) ma odbywać się za pomocą enkodera. Wybór
// pozycji menu odbywa się za pomocą przycisku. Minimalna funkcjonalność to zapalanie i 
// gaszenie wybranej diody. Można też zaimplementować ustawianie jasności każdego kolory 
// diody enkoderem. Warto też program oprzeć na przerwaniach zgłaszanych przy zmianie stanu enkodera.


#include <util/atomic.h>
#include <LiquidCrystal_I2C.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3
int led[] = {LED_RED, LED_GREEN, LED_BLUE};

#define ENCODER1 A2
#define ENCODER2 A3

#define DEBOUNCE_PERIOD 10UL

#define RED_BUTTON 2
#define GREEN_BUTTON 4

LiquidCrystal_I2C lcd(0x27, 16, 2);

//________________________________________________________________________________________________
String tsMenuOptions[] = {"Wbudowana", "RGB", "Set RGB", "Set Diode"};
int iMenusLen = 4;

int iCurrentInt = 0;
int iPrevInt = -1;

volatile unsigned long buttonTimestamp = 0UL;
unsigned long previousButtonTimestamp = 0UL;

int enkoderParameter = 1000;
int lastEn1 = LOW;
unsigned long lastChangeTimestamp = 0UL;
bool isSelected = false;

volatile int encoder1 = HIGH;
volatile int encoder2 = HIGH;
volatile unsigned long encoderTimestamp = 0UL;


static int lastIntensivity = -1;
//________________________________________________________________________________________________

void displayEnkoderParameter(String sVal) {
    lcd.setCursor(10,1);
    lcd.print("_____");
    lcd.setCursor(10,1);
    lcd.print(sVal);
}


void displayString(String sVal) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("wybrano:");
  	lcd.setCursor(0,1);
  	lcd.print(sVal);
}

void displayStrings(String sVal1, String sVal2) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(sVal1);
 	  lcd.setCursor(14,0);
  	lcd.print("<-");
    lcd.setCursor(0,1);
    lcd.print(sVal2);
}

void showExcerciseNumber(String sVal){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(sVal);
    delay(1500);
}

// blinking methods
//________________________________________________________________________________________________

void setRgbLed(int intensivity) {
    //const unsigned long BlinkChangePeriod = 1000UL;
  	static int led_index = 0;
    int mappedIntesivity = map(intensivity, 0, 1000, 0, 255);
    displayEnkoderParameter(String(mappedIntesivity));
  	Serial.println(mappedIntesivity);
  	if(mappedIntesivity != lastIntensivity)
    {
  		analogWrite(led[led_index], mappedIntesivity);
      	lastIntensivity = mappedIntesivity;
    }
    if (isGreenBuPressed()) 
    {
        led_index = ++led_index % 3;
    } 
}


void blinkBuiltinLed(int time) {
    //const unsigned long BlinkChangePeriod = 1000UL;
  	const unsigned long BlinkChangePeriod = time;
    displayEnkoderParameter(String(BlinkChangePeriod));
    static int ledState = LOW;
    static unsigned long lastBlinkChange = 0UL;

    if (millis() - lastBlinkChange >= BlinkChangePeriod)
    {
        if (ledState == HIGH)
        {
            ledState = LOW;
        }
        else
        {
            ledState = HIGH;
        }

        digitalWrite(LED_BUILTIN, ledState);
        lastBlinkChange += BlinkChangePeriod;
    }
}

void blinkRGB(int time) {
    const unsigned long BlinkChangePeriod = time/2;
    static unsigned long lastBlinkChange = 0UL;
    displayEnkoderParameter(String(BlinkChangePeriod));
    if (millis() - lastBlinkChange >= BlinkChangePeriod)
    {
        static int led_index = 0;
        digitalWrite(led[led_index], LOW);
        led_index = ++led_index % 3;
        digitalWrite(led[led_index], HIGH);
        lastBlinkChange += BlinkChangePeriod;
    }
}

void blinkRGBSmooth(int time) {
    const unsigned long BlinkChangePeriod = time/10;
    static int ledState = 0;
    static bool goUp = true;
    static unsigned long lastBlinkChange = 0UL;
    static int led_index = 0;

    if (millis() - lastBlinkChange >= BlinkChangePeriod)
    {   
        if(goUp) {
            analogWrite(led[led_index], ledState);

            ledState += 5;
            if(ledState >= 255) {
                analogWrite(led[led_index], 255);
                ledState = 255;
                goUp = false;
            }
        }
        else {
            analogWrite(led[led_index], ledState);

            ledState -= 5;
            if(ledState <= 0) {
                analogWrite(led[led_index], 0);
                ledState = 0;
                led_index = ++led_index % 3;
                analogWrite(led[led_index], 0);
                goUp = true;
            }
        }
        
        lastBlinkChange += BlinkChangePeriod;
    }
}

void allRgbOn(int intensivity) {
  	int mappedIntesivity = map(intensivity, 0, 1000, 0, 255);
    displayEnkoderParameter(String(mappedIntesivity));
    for(int i=0; i<3; i++) {
        digitalWrite(led[i], mappedIntesivity);
    }
}

void allOff() {
    for(int i=0; i<3; i++) {
        digitalWrite(led[i], 0);
    }
    digitalWrite(LED_BUILTIN, LOW);
}

//________________________________________________________________________________________________

void interruptAction() {
    buttonTimestamp = millis();
}

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

void initButtons() {
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
}


bool isBuPressedTemplate(int button, int& debounced_button_state, 
int& previous_reading, unsigned long& last_change_time)
{
    bool isPressed = false;
    int current_reading = digitalRead(button);
    if (previous_reading != current_reading)
    {
        last_change_time = millis();
    }
    if (millis() - last_change_time > DEBOUNCE_PERIOD)
    {
        if (current_reading != debounced_button_state)
        {
            if (debounced_button_state == HIGH && current_reading == LOW)
            {
                isPressed = true;
            }
            debounced_button_state = current_reading;
        }
    }
    previous_reading = current_reading;
    return isPressed;
}


bool isGreenBuPressed() {
    static int debounced_button_state = HIGH;
    static int previous_reading = HIGH;
    static unsigned long last_change_time = 0UL;
    return isBuPressedTemplate(GREEN_BUTTON, debounced_button_state, 
    previous_reading, last_change_time);
}

bool isRedBuPressed() {
    static int debounced_button_state = HIGH;
    static int previous_reading = HIGH;
    static unsigned long last_change_time = 0UL;
    return isBuPressedTemplate(RED_BUTTON, debounced_button_state, 
    previous_reading, last_change_time);
}


ISR(PCINT1_vect) {
    encoder1 = digitalRead(ENCODER1);
    encoder2 = digitalRead(ENCODER2);
    encoderTimestamp = millis();
}

void setup() {
    Serial.begin(9600);
    while (!Serial) {}
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(ENCODER1, INPUT_PULLUP);
    pinMode(ENCODER2, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();
    showExcerciseNumber("Zadanie 2");

    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT10);
    initButtons();
    initRGB();
    displayStrings(tsMenuOptions[iCurrentInt], tsMenuOptions[iCurrentInt + 1]);
    attachInterrupt(digitalPinToInterrupt(RED_BUTTON), interruptAction, FALLING);
}

void (*(fun[4]))(int) = {blinkBuiltinLed, blinkRGB, setRgbLed, allRgbOn};
void loop() {  
  	
    int en1;
    int en2;
    unsigned long timestamp;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        en1 = encoder1;
        en2 = encoder2;
        timestamp = encoderTimestamp;
    }

    // temp part of code for tests
  	if(!isSelected)
    {
      if (Serial.available()) {
          String command = Serial.readStringUntil('\n');
          if(command == "d") {
              iCurrentInt = (iCurrentInt + 1)% iMenusLen;

          } else if (command == "u") {
              if (iCurrentInt == 0) {
                  iCurrentInt = iMenusLen;
              }
              iCurrentInt -= 1;
          }
          if(iCurrentInt != iPrevInt) {
                  int iNextInt = (iCurrentInt + 1)% iMenusLen;
                  displayStrings(tsMenuOptions[iCurrentInt], tsMenuOptions[iNextInt]);
                  //isSelected = false;
                  iPrevInt = iCurrentInt;
          }

      }
    } else {
      if (Serial.available()) {
          String command = Serial.readStringUntil('\n');
          if(command == "u") {
            if(enkoderParameter < 1000)
            	enkoderParameter+=100;
          } else if (command == "d") {
            if(enkoderParameter > 0) 
				      enkoderParameter-=100;	
          }
      }
    }
  	
     if(!isSelected)
    {
      if (en1 == LOW && timestamp > lastChangeTimestamp + DEBOUNCE_PERIOD)
      {
          if (en2 == HIGH)
          {   
              // my code
              iCurrentInt = (iCurrentInt + 1)% iMenusLen;
          }
          else
          {   
              // my code
              if (iCurrentInt == 0) {
                  iCurrentInt = iMenusLen;
              }
              iCurrentInt -= 1;
          }
          lastChangeTimestamp = timestamp;

          // my code
          if(iCurrentInt != iPrevInt) {
              int iNextInt = (iCurrentInt + 1)% iMenusLen;
              displayStrings(tsMenuOptions[iCurrentInt], tsMenuOptions[iNextInt]);
              isSelected = false;
              iPrevInt = iCurrentInt;
          }
      }
      lastEn1 = en1;
  } else {
     if (en1 == LOW && timestamp > lastChangeTimestamp + DEBOUNCE_PERIOD)
      {
          if (en2 == HIGH)
          {   
            if(enkoderParameter < 1000)
            	enkoderParameter+=100;
          }
          else
          {   
            if(enkoderParameter > 0) 
				      enkoderParameter-=100;	
          }
          lastChangeTimestamp = timestamp;
      }
      lastEn1 = en1;
  
	}

    noInterrupts();
    unsigned long localButtonTimestamp = buttonTimestamp;
    interrupts();


	if (isGreenBuPressed()) {
            isSelected = true;
            displayString(tsMenuOptions[iCurrentInt]);
          	
    } 
	if (isRedBuPressed()) {
            isSelected = false;
            int iNextInt = (iCurrentInt + 1)% iMenusLen;
            displayStrings(tsMenuOptions[iCurrentInt], tsMenuOptions[iNextInt]);
    } 

  
  
  	
	String str = String(digitalRead(RED_BUTTON)) + " - " 
      +String( digitalRead(GREEN_BUTTON))+" - "+ String(isSelected)
      + " - " + String(enkoderParameter);
    Serial.println(str);

    if(isSelected) {
        fun[iCurrentInt](enkoderParameter);
    }
    else {
        allOff();
    }
}
