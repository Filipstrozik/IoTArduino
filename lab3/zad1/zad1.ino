#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

int led[] = {LED_RED, LED_GREEN, LED_BLUE};

void initRGB()
{
    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, LOW);

    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_GREEN, LOW);

    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, LOW);
}

void initButtons()
{
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

#define DEBOUNCE_PERIOD 10UL

bool isButtonPressed(int button, int& debounced_button_state, 
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


bool isButtonPressed(int button) {
    static int debounced_button_state = HIGH;
    static int previous_reading = HIGH;
    static unsigned long last_change_time = 0UL;
    return isButtonPressed(button, debounced_button_state, 
    previous_reading, last_change_time);
}



bool isButtonReleased(int button, int& debounced_button_state,
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



bool isButtonReleased(int button)
{
    static int debounced_button_state = LOW;
    static int prev_reading = LOW;
    static unsigned long last_change_time = 0UL;
  	return isButtonReleased(button, debounced_button_state, prev_reading,
                            last_change_time);
}

bool wasButtonPressedAndReleased(int button)
{
    static bool prev = false;
    if (isButtonPressed(button)) {
        prev = true;
    }
    if (prev & isButtonReleased(button)) {
        prev = false;
        return true;
    }
    return false;
}


void setup()
{
    initRGB();
    initButtons();
}


int led_index = 0;
void loop()
{
	if (wasButtonPressedAndReleased(RED_BUTTON) || wasButtonPressedAndReleased(GREEN_BUTTON))
	{
		digitalWrite(led[led_index], LOW);
		led_index = ++led_index % 3;
		digitalWrite(led[led_index], HIGH);
	}
}