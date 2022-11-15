#include "RgbHandler.h"
//-------------RGBDIODE
RgbDiode::RgbDiode(int pinR, int pinG, int pinB)
{
    r = pinR;
    g = pinG;
    b = pinB;
}

void RgbDiode::initialize()
{
    pinMode(r, OUTPUT);
    pinMode(g, OUTPUT);
    pinMode(b, OUTPUT);

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
    Color setColor(color);
    analogWrite(r, setColor.red());
    analogWrite(g, setColor.green());
    analogWrite(b, setColor.blue());
}


//-------------COLOR---------------

void setRgb(int color[]){
    this.r = color[0];
    this.g = color[1];
    this.b = color[2];
}


Color::Color(String color) {
    if(color == "red") {
        setRgb(RED);
    }
    else if(color == "green") {
        copyArray(rgb, GREEN, rgbLen);
    }
    else if(color == "blue") {
        copyArray(rgb, BLUE, rgbLen);
    }
    else if(color == "yellow") {
        copyArray(rgb, YELLOW, rgbLen);
    }
    else if(color == "cyan") {
        copyArray(rgb, CYAN, rgbLen);
    }
    else if(color == "magenta") {
        copyArray(rgb, MAGENTA, rgbLen);
    }
    else if(color == "white") {
        copyArray(rgb, WHITE, rgbLen);
    }
    else if(color == "black") {
        copyArray(rgb, BLACK, rgbLen);
    }
};


Color::Color(int r, int g, int b) {
    this.r = r;
    this.g = g;
    this.b = b;
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

