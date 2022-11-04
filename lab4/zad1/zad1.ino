// Napisz program, który będzie wysyłał informację o stanie przycisków czerwonego i 
// zielonego przez łącze szeregowe do komputera PC. Sprawdź jaką maksymalną szybkość 
// transmisji danych możesz skonﬁgurować. Spróbuj zwizualizować stany
// przycisków na wykresie w Serial Plotter
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#define DEBOUNCE_PERIOD 10UL
#define RED_BUTTON 2
#define GREEN_BUTTON 4

void initLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Zadanie 1");
}


void initButtons()
{
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

void initSerial() {
    Serial.begin(9600);
    while (!Serial) {}
}

void setup() {
    initLCD();
    initButtons();
    initSerial();
}



double result=0;
int r = 0;
int g = 0;

void loop()
{
    // if(isGreenBuPressed()){
    //   g = 1;
    // } else  if(isGreenBuReleased()){
    //   g = 0;
    // }
    // if(isRedBuPressed()){
    //   r = 1;
    // } else  if(isRedBuReleased()){
    //   r = 0;
    // }


    if(digitalRead(GREEN_BUTTON) == LOW){
      g = 1;
    } else  if(digitalRead(GREEN_BUTTON) == HIGH){
      g = 0;
    }
    if(digitalRead(RED_BUTTON) == LOW){
      r = 1;
    } else  if(digitalRead(RED_BUTTON)==HIGH){
      r = 0;
    }
    result = r+g;
    delay(100);

    // String str = "RED: "+ String(r) + "  GREEN: " + String(g);

    // result = r+g;
    String str = String(result);

    // Serial.println(str);

  Serial.print(-1); // To freeze the lower limit
  Serial.print(" ");
  Serial.print(3); // To freeze the upper limit
  Serial.print(" ");
  Serial.println(result);
}