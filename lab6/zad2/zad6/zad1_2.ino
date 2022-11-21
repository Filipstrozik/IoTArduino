#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4
#define DEBOUNCE_PERIOD 10UL

RgbDiode myDiode(6,5,3); // zastosowanie klasy RgbDiode ustawienie wyjsc
Color colors[] = {Color("RED"), Color("GREEN"), Color("BLUE"), Color("YELLOW"), Color("CYAN"), Color("MAGENTA"), Color("WHITE"), Color("BLACK"), Color(255, 153, 0), Color(0, 51, 0)};
int colors_len = 10;
int color_index = -1;
int led[] = {LED_RED, LED_GREEN, LED_BLUE};

// void initRGB()
// {
//     pinMode(LED_RED, OUTPUT);
//     digitalWrite(LED_RED, LOW);

//     pinMode(LED_GREEN, OUTPUT);
//     digitalWrite(LED_GREEN, LOW);

//     pinMode(LED_BLUE, OUTPUT);
//     digitalWrite(LED_BLUE, LOW);
// }

void initLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("lab 3:");
  lcd.setCursor(0,1);
  lcd.print("Zadanie 2");

}

void initBus()
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


void setup() {

    initBus();
    // initRGB(); depricated
    initLCD();
    myDiode.initialize();
}
bool isChanged = false;
void loop() {
    if(wasGreenPressedAndReleased())
    {
        color_index = ++color_index % 10;
        lcd.setCursor(15, 1);
        lcd.print(String(color_index));
        myDiode.setColor(colors[color_index]);
    }

    if (wasRedPressedAndReleased()) 
    {
      if(color_index == 9){
        if(isChanged){
          colors[color_index].setColor("BLUE");
          myDiode.setColor(colors[color_index]);
          isChanged = false;
        } else {
          colors[color_index].setColor("USER");
          myDiode.setColor(colors[color_index]);
          isChanged = true;
        }
      } else {
        myDiode.writeRgb(155,50,0); // no need to remember color only rgb
      }
    }
}