// C++ code
#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4
int COUNTER = 0;
void initRGB()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

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

void setup()
{
    initRGB();
    initButtons();
}

void setColor(int R, int G, int B) {
  analogWrite(LED_RED,   R);
  analogWrite(LED_GREEN, G);
  analogWrite(LED_BLUE,  B);
}


void loop()
{
  
  if(digitalRead(GREEN_BUTTON) == LOW && COUNTER > 0){ //zamien na low
    COUNTER-=1;
    if(COUNTER>=510)
    {
      analogWrite(LED_BLUE, COUNTER-510);
    }
    else if(COUNTER>=255)
    {
      analogWrite(LED_GREEN, COUNTER-255);
    }
    else{
      analogWrite(LED_RED, COUNTER);
    }
    delay(10);
  }else if(digitalRead(RED_BUTTON) == LOW && COUNTER < 765){ // zamien na high
    COUNTER+=1;
    if(COUNTER<=255)
    {
      analogWrite(LED_RED, COUNTER);
    }
    else if(COUNTER<=510)
    {
      analogWrite(LED_GREEN, COUNTER-255);
    }
    else{
      analogWrite(LED_BLUE, COUNTER-510);
    }
    delay(10);
  }
}