#!/usr/bin/env python3
# developed by Filip Strózik 2022

import RPi.GPIO as GPIO
import time
from datetime import datetime
import neopixel
import board
import busio
import w1thermsensor
import adafruit_bme280.advanced as adafruit_bme280
from mfrc522 import MFRC522


def formated_print(mytime):
    return f'{mytime.hour}:{mytime.minute}:{mytime.second},{mytime.microsecond}'


class RFID:
    def __init__(self):
        self.MIFAREReader = MFRC522()
        self.initial_time = None
        self.is_being_read = False

    def read(self) -> bool:
        (status, TagType) = self.MIFAREReader.MFRC522_Request(self.MIFAREReader.PICC_REQIDL)
        if status == self.MIFAREReader.MI_OK:
            (status, uid) = self.MIFAREReader.MFRC522_Anticoll()
            if status == self.MIFAREReader.MI_OK:
                curr_time = datetime.now()
                if not self.is_being_read:
                    self.is_being_read = True
                    self.initial_time = curr_time
                    return True
                else:  # this is after sth called nesting
                    self.is_being_read = False
        else:
            self.is_being_read = False
        return False

    def print_read_rfid(self):
        print(f'card readed: {self.is_being_read} Last read time: {formated_print(self.initial_time)}')


# clas from lab9
class Color:
    white = (255, 255, 255)
    black = (0, 0, 0)
    red = (255, 0, 0)
    green = (0, 255, 0)


class LedController:
    def __init__(self):
        self.pixels = neopixel.NeoPixel(board.D18, 8, brightness=1.0 / 32, auto_write=False)
        self.animation_speed = 0.2

    def animate_read(self):
        for i in range(0, 3):
            self.pixels.fill(Color.black)
            self.pixels.show()
            time.sleep(self.animation_speed)

            self.pixels.fill(Color.green)
            self.pixels.show()
            time.sleep(self.animation_speed)


class MainController:
    def __init__(self):
        self.rfid = RFID()
        self.led = LedController()
        self.buzzer = None
        self.time_period = 1

    def run(self):
        if self.rfid.read():
            self.rfid.print_read_rfid()
            GPIO.output(buzzerPin, False)
            self.led.animate_read()

        if self.rfid.initial_time + self.time_period < datetime.now():
            GPIO.output(buzzerPin, True)

        if not self.rfid.is_being_read:
            self.led.pixels.fill(Color.black)
            self.led.pixels.show()


if __name__ == '__main__':
    controller = MainController()
    while True:
        controller.run()