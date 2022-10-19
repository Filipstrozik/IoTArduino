#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

int red = 255;
int blue = 0;
int green = 0;



void setup()
{
}


void setColor(int R, int G, int B) {
  	analogWrite(LED_RED,   R);
  	analogWrite(LED_GREEN, G);
  	analogWrite(LED_BLUE,  B);
}


void loop()
{
	#define delayTime 5

	for(int i = 0; i < 255; i += 1)
	{
  		red -= 1;
  		green+= 1;
  		setColor(red, green, blue);
  		delay(delayTime);
	}

    red = 0;
    green = 255;
    blue = 0;

    for(int i = 0; i < 255; i += 1)
    {
      green -= 1;
      blue += 1;
      setColor(red, green, blue);
      delay(delayTime);
    }
  
    red = 0;
    green = 0;
    blue = 255;

    for(int i = 0; i < 255; i += 1)
    {
      blue -= 1;
      red += 1;
      setColor(red, green, blue);
      delay(delayTime);
    }
}