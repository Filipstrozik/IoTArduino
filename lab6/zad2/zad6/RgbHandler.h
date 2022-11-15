#ifndef RGBHANDLER_H_
#define RGBHANDLER_H_

#include "Arduino.h"


class Color {
    public:
        Color(String name);
        Color(int r, int g, int b);

        int red();
        int green();
        int blue();
    
    private:
        int r = -1;
        int g = -1;
        int b = -1;
        int rgbLen = 3;//
        int rgb[3] = {-1, -1, -1};//
        int RED[3] = {255, 0, 0};
        int GREEN[3] = {0, 255, 0};
        int BLUE[3] = {0, 0, 255};
        int YELLOW[3] = {255, 255, 0};
        int CYAN[3] = {0, 255, 255};
        int MAGENTA[3] = {255, 0, 255};
        int BLACK[3] = {0, 0, 0};
        int WHITE[3] = {255, 255, 255};
};




class RgbDiode
{
    public:
        RgbDiode(int pinR, int pinG, int pinB);

        void initialize();
        //ustawienie wartosci komponentow:
        void writeRed(int val);
        void writeGreen(int val);
        void writeBlue(int val);

        void writeRgb(int valR, int valG, int valB);
        //ustawienie colorem
        void writeColor(String color);

    private:
        int r, g, b;
};

#endif