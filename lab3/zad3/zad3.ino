#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#define RED_BUTTON 2
#define GREEN_BUTTON 4
#define DEBOUNCE_PERIOD 10UL
#define DEBOUNCE_DISPLAY 100UL //10

unsigned long last_time_break = 0UL;
unsigned long previous_time_sum = 0UL;
bool isTimerRunning = false;

void initBus()
{
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void initLCD()
{
  lcd.init();
  lcd.clear();
  lcd.backlight();
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

bool isBuReleasedTemplate(int button, int& debounced_button_state, 
int& previous_reading, unsigned long& last_change_time)
{
    bool isReleased = false;
    int current_reading = digitalRead(button);
    if (previous_reading != current_reading)
    {
        last_change_time = millis();
    }
    if (millis() - last_change_time > DEBOUNCE_PERIOD)
    {
        if (current_reading != debounced_button_state)
        {
            if (debounced_button_state == LOW && current_reading == HIGH)
            {
                isReleased = true;
            }
            debounced_button_state = current_reading;
        }
    }
    previous_reading = current_reading;
    return isReleased;
}


bool isGreenBuReleased() {
    static int debounced_button_state = LOW;
    static int previous_reading = LOW;
    static unsigned long last_change_time = 0UL;
    return isBuReleasedTemplate(GREEN_BUTTON, debounced_button_state, 
    previous_reading, last_change_time);
}

bool isRedBuReleased() {
    static int debounced_button_state = LOW;
    static int previous_reading = LOW;
    static unsigned long last_change_time = 0UL;
    return isBuReleasedTemplate(RED_BUTTON, debounced_button_state, 
    previous_reading, last_change_time);
}

bool wasGreenPressedAndReleased() {
    static bool prev_green = false;
    if (isGreenBuPressed()) {
        prev_green = true;
    }
    if (prev_green && isGreenBuReleased()) {
        prev_green = false;
        return true;
    }
    return false;
}

bool wasRedPressedAndReleased() {
    static bool prev_red = false;
    if (isRedBuPressed()) {
        prev_red = true;
    }
    if (prev_red && isRedBuReleased()) {
        prev_red = false;
        return true;
    }
    return false;
}


void setup() {
    initBus();
    initLCD();
    
    lcd.setCursor(0,0);
    lcd.print("Zadanie 3 :)");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("-:--:-");
}

void displayString(String str_val) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(str_val);
}

void displayValue(unsigned long current_time) {
  static unsigned long last_displayed = 0UL;
  if (last_displayed != current_time) {
    lcd.clear();
    lcd.setCursor(0,0);
    int setna = current_time/100;
    int secs = current_time/1000;
    int mins = secs/60;
    secs = secs - 60*mins;
    setna = setna - 10*secs;
    lcd.print(mins);
    lcd.print(":");
    if (secs < 10) {
      lcd.print(0);
    }
    lcd.print(secs);
    lcd.print(":");
    if(mins > 0){
      setna = setna % 60;
    }
    lcd.print(setna);
    last_displayed = current_time;
  }
  
}

void displayUpdatedTime() {
  static unsigned long temp_measurement = 0UL;
  if (millis() - temp_measurement > DEBOUNCE_DISPLAY) {
    temp_measurement = millis();
    if (isTimerRunning) {
      displayValue(millis() - last_time_break + previous_time_sum);
    }
    else {
      displayValue(previous_time_sum);
    }
  }
}

void startTimer() {
  last_time_break = millis();
  isTimerRunning = true;
}
 
void stopTimer() {
  unsigned long last_measurement = millis() - last_time_break;
  previous_time_sum += last_measurement;
  isTimerRunning = false;
}

void resetTimer() {
  isTimerRunning = false;
  previous_time_sum = 0UL;
}

void loop() {

  if (wasGreenPressedAndReleased()) { // mozna na sam "wcisk" teraz jest ze na "odcisk"
    isTimerRunning = !isTimerRunning;
    if(isTimerRunning) {
      startTimer();
      //displayString("start");
    }
    else {
      stopTimer();
      displayString("STOP");
      delay(1000);
    }
  }

  if (wasRedPressedAndReleased()) {// mozna na sam "wcisk" teraz jest ze na "odcisk"
    resetTimer();
    displayString("RESET");
    delay(1000);
  }
  
  displayUpdatedTime();
}