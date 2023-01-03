#!/usr/bin/env python3
# developed by Filip Strózik 2022
import time
import board
import busio
import w1thermsensor
import adafruit_bme280.advanced as adafruit_bme280
import lib.oled.SSD1331 as SSD1331
from PIL import Image, ImageDraw, ImageFont
from config import *

ARIAL = ImageFont.truetype('fonts/arial.ttf')


class Oled:
    def __init__(self, background_file):
        self.display = SSD1331.SSD1331()
        self.display.Init()
        self.display.clear()
        self.background = Image.open(background_file)
        self.printer = ImageDraw.Draw(self.background)

    def show(self):
        self.display.ShowImage(self.background)

    def clear(self):
        self.display.clear()

    def reset(self):
        self.display.reset()

    def get_width(self):
        return self.display.width

    def get_height(self):
        return self.display.heigth

    def print_text(self, coordinates, text, color=None, font=ARIAL):
        self.printer.text(coordinates, text, fill=color, font=font)

    def clear_xy(self, coordinates):
        self.printer.rectangle(coordinates, fill=(255, 255, 255))


class Station:
    def __init__(self):
        # ds18b20
        self.ds18b20 = w1thermsensor.W1ThermSensor()
        # bme280
        self.i2c = busio.I2C(board.SCL, board.SDA)
        self.bme280 = adafruit_bme280.Adafruit_BME280_I2C(self.i2c, 0x76)
        self.bme280.sea_level_pressure = 1013.25
        # self.bme280.mode = adafruit_bme280.MODE_NORMAL
        self.bme280.standby_period = adafruit_bme280.STANDBY_TC_500
        self.bme280.iir_filter = adafruit_bme280.IIR_FILTER_X16
        self.bme280.overscan_pressure = adafruit_bme280.OVERSCAN_X16
        self.bme280.overscan_humidity = adafruit_bme280.OVERSCAN_X1
        self.bme280.overscan_temperature = adafruit_bme280.OVERSCAN_X2

    def temperature(self):
        return self.bme280.temperature

    def altitude(self):
        return self.bme280.altitude

    def pressure(self):
        return self.bme280.pressure

    def humidity(self):
        return self.bme280.humidity


class MainController:
    TEMPERATURE_DELTA = 0.1
    HUMIDITY_DELTA = 0.1
    ALTITUDE_DELTA = 0.1
    PRESSURE_DELTA = 1
    PIXEL_RIGHT = 30
    MAX_RIGHT = 96

    # TODO add alone pictograms on left of every measure
    def __init__(self):
        self.weather_station = Station()
        self.oled_display = Oled('images/background_dark.png')
        self.measures = [
            self.MeasureHandler(self.oled_display,
                                self.weather_station.temperature,
                                (self.PIXEL_RIGHT, 3),
                                ((self.PIXEL_RIGHT, 0), (self.MAX_RIGHT, 20)),
                                self.TEMPERATURE_DELTA,
                                'Temp:', 'C'),
            self.MeasureHandler(self.oled_display,
                                self.weather_station.humidity,
                                (self.PIXEL_RIGHT, 19),
                                ((self.PIXEL_RIGHT, 15), (self.MAX_RIGHT, 35)),
                                self.HUMIDITY_DELTA,
                                'Hum:', '%'),
            self.MeasureHandler(self.oled_display,
                                self.weather_station.altitude,
                                (self.PIXEL_RIGHT, 35),
                                ((self.PIXEL_RIGHT, 30), (self.MAX_RIGHT, 50)),
                                self.ALTITUDE_DELTA,
                                'Alt:', 'm'),
            self.MeasureHandler(self.oled_display,
                                self.weather_station.pressure(),
                                (self.PIXEL_RIGHT, 50),
                                ((self.PIXEL_RIGHT, 45), (self.MAX_RIGHT, 65)),
                                self.PRESSURE_DELTA,
                                'Press:', 'hPa')
        ]

    def initial_print_measures(self):
        for measure in self.measures:
            measure.print()
        self.oled_display.show()

    def print_measures(self):
        for measure in self.measures:
            measure.difference_print()
        self.oled_display.show()

    class MeasureHandler:
        def __init__(self, oled, function, pixel_tuple, clear_tuple, value_delta, extra_text, unit):
            self.oled = oled
            self.current_value = function()
            self.function = function
            self.print_tuple = pixel_tuple
            self.clear_tuple = clear_tuple
            self.value_delta = value_delta
            self.extra_text = extra_text
            self.unit = unit

        def difference_print(self):
            new_value = self.function()
            if abs(self.current_value - new_value) > self.value_delta:
                self.current_value = new_value
                self.print()

        def print(self):
            self.oled.clear_xy(self.clear_tuple)
            text_to_print = f'{self.extra_text} {round(self.current_value, 1)} {self.unit}'
            self.oled.print_text(self.print_tuple, text_to_print)  # may i need 4 args


if __name__ == '__main__':
    controller: MainController = MainController()
    controller.initial_print_measures()
    while True:
        controller.print_measures()
