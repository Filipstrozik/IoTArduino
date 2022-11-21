#include "RgbHandler.h"

//-------------RGBDIODE---------------
RgbDiode::RgbDiode(int pinR, int pinG, int pinB)
{
    r = pinR;
    g = pinG;
    b = pinB;
}

void RgbDiode::initialize()
{
    //set mode
    pinMode(r, OUTPUT);
    pinMode(g, OUTPUT);
    pinMode(b, OUTPUT);
    //set low state
    digitalWrite(r, LOW);
    digitalWrite(g, LOW);
    digitalWrite(b, LOW);
}

void RgbDiode::writeRed(int val) 
{
    analogWrite(r, val);
}

void RgbDiode::writeGreen(int val) 
{
    analogWrite(g, val);
}

void RgbDiode::writeBlue(int val) 
{
    analogWrite(b, val);
}

void RgbDiode::writeRgb(int valR, int valG, int valB)
{
    analogWrite(r, valR);
    analogWrite(g, valG);
    analogWrite(b, valB);
}

void RgbDiode::writeColor(String color)
{
    Color userColor(color);
    analogWrite(r, userColor.red());
    analogWrite(g, userColor.green());
    analogWrite(b, userColor.blue());
}

void RgbDiode::setColor(Color color)
{
    analogWrite(r, color.red());
    analogWrite(g, color.green());
    analogWrite(b, color.blue());
}


//-------------COLOR---------------

Color::Color(int r, int g, int b) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->USER[0] = r;
    this->USER[1] = g;
    this->USER[2] = b;
};

void Color::setRgb(int color[]){
    this->r = color[0];
    this->g = color[1];
    this->b = color[2];
    this->USER[0] = color[0];
    this->USER[1] = color[1];
    this->USER[2] = color[2];
}

void Color::setColor(String color){
    if (color == "RED"){
        setRgb(RED);
    } else if(color == "GREEN"){
        setRgb(GREEN);
    } else if(color == "BLUE"){
        setRgb(BLUE);
    } else if(color == "YELLOW"){
        setRgb(YELLOW);
    } else if(color == "CYAN"){
        setRgb(CYAN);
    } else if(color == "MAGENTA"){
        setRgb(MAGENTA);
    } else if(color == "WHITE"){
        setRgb(WHITE);
    } else if(color == "BLACK"){
        setRgb(BLACK);
    } else if(color == "USER"){
        setRgb(USER);
    }
}


Color::Color(String color) {

    if (color == "RED"){
        setRgb(RED);
    } else if(color == "GREEN"){
        setRgb(GREEN);
    } else if(color == "BLUE"){
        setRgb(BLUE);
    } else if(color == "YELLOW"){
        setRgb(YELLOW);
    } else if(color == "CYAN"){
        setRgb(CYAN);
    } else if(color == "MAGENTA"){
        setRgb(MAGENTA);
    } else if(color == "WHITE"){
        setRgb(WHITE);
    } else if(color == "BLACK"){
        setRgb(BLACK);
    } else if(color == "USER"){
        setRgb(USER);
    } 
};

int Color::red() {
    return r;
}

int Color::green() {
    return g;
}

int Color::blue() {
    return b;
}

