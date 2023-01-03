#!/usr/bin/env python3
# developed by Filip Strózik 2022
import time
from abc import ABC, abstractmethod
import adafruit_bme280.advanced as adafruit_bme280
import board
import busio
import neopixel
import w1thermsensor
from config import *


# enumeracja kolorów dla każdego trybu
class Color:
    temperature_ds_color = (255, 10, 144)
    temperature_bme = (255, 144, 10)
    pressure_color = (255, 0, 0)
    altitude_color = (128, 128, 0)
    humidity_color = (0, 255, 120)


# sterownik paska led.
class WeatherDiods:
    def __init__(self) -> None:
        self.pixels = neopixel.NeoPixel(board.D18, 8, brightness=1.0 / 32, auto_write=False)

    # def humidity(self):
    #     hum = (0, 255, 120)
    #     self.pixels.fill(hum)
    #     self.pixels.show()
    #     time.sleep(1)
    #     pass

    # metoda wyświetlająca pomiar
    def display(self, val, start, end, color):
        max_one_led = (end - start) / 8
        leds_on_number = int((val - start) / max_one_led)

        # w petli odświezamy ile ledow ma sie zapalic
        for led_id in range(8):
            # bycmoze powinno być <=
            if led_id < leds_on_number:
                self.pixels[led_id] = color
            else:
                self.pixels[led_id] = (0, 0, 0)
        self.pixels.show()
        print(f'Display: {leds_on_number}  leds of 8, start: {start} end: {end} color: {color}')

    def display_menu(self, mode_index):
        self.pixels.fill((0, 0, 0))  # ciekawe czy dobrze krotka?
        self.pixels.show()
        print(f'Menu: {mode_index}(mode) leds of {4}')
        if mode_index == 0:
            self.pixels[mode_index] = Color.temperature_ds_color
            print(f'TEMPERATURE DS')
        elif mode_index == 1:
            self.pixels[mode_index] = Color.temperature_bme
            print(f'TEMPERATURE BME')
        elif mode_index == 2:
            self.pixels[mode_index] = Color.pressure_color
            print(f'PRESSURE')
        elif mode_index == 3:
            self.pixels[mode_index] = Color.humidity_color
            print(f'HUMIDITY')
        elif mode_index == 4:
            self.pixels[mode_index] = Color.altitude_color
            print(f'ALTITUDE')
        self.pixels.show()


# kontroller do stacji pogodowych
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


class MainController:
    def __init__(self):
        self.is_selected = False
        self.current_mode = 0
        self.diods = WeatherDiods()
        self.weather_station = Station()
        self.modes = [
            self.TemperatureDS(self.diods, self.weather_station),
            self.TemperatureBME(self.diods, self.weather_station),
            self.Pressure(self.diods, self.weather_station),
            self.Humidity(self.diods, self.weather_station),
            self.Altitude(self.diods, self.weather_station)
        ]

    class Mode(ABC):
        def __init__(self, leds, weather_station) -> None:
            self.leds = leds
            self.weather_station = weather_station

        @abstractmethod
        def run(self):
            pass

        @abstractmethod
        def set_min(self, value):
            pass

        @abstractmethod
        def set_max(self, value):
            pass

    class TemperatureDS(Mode):
        def __init__(self, leds, weather_station) -> None:
            super().__init__(leds, weather_station)
            self.current_temp_ds = self.weather_station.bme280.temperature
            self.max_temp_ds = 25
            self.min_temp_ds = 20

        def run(self):
            self.current_temp_ds = self.weather_station.ds18b20.get_temperature()
            self.leds.display(self.current_temp_ds, self.min_temp_ds, self.max_temp_ds, Color.temperature_ds_color)
            print(f'Temperature: {self.current_temp_ds:0.1f} ' + chr(176) + 'C')

        def set_min(self, value):
            self.min_temp_ds = value

        def set_max(self, value):
            self.max_temp_ds = value

    class TemperatureBME(Mode):
        def __init__(self, leds, weather_station) -> None:
            super().__init__(leds, weather_station)
            self.current_temp = self.weather_station.bme280.temperature
            self.max_temp = 25
            self.min_temp = 20

        def run(self):
            self.current_temp = self.weather_station.bme280.temperature
            self.leds.display(self.current_temp, self.min_temp, self.max_temp, Color.temperature_bme)
            print(f'Temperature: {self.current_temp:0.1f} ' + chr(176) + 'C')

        def set_min(self, value):
            self.min_temp = value

        def set_max(self, value):
            self.max_temp = value

    class Pressure(Mode):
        def __init__(self, leds, weather_station) -> None:
            super().__init__(leds, weather_station)
            self.current_pressure = self.weather_station.bme280.pressure
            self.max_pressure = 1000
            self.min_pressure = 900

        def run(self):
            self.current_pressure = self.weather_station.bme280.pressure
            self.leds.display(self.current_pressure, self.min_pressure, self.max_pressure, Color.pressure_color)
            print(f'Pressure: {self.current_pressure:0.1f} hPa')

        def set_min(self, value):
            self.min_pressure = value

        def set_max(self, value):
            self.max_pressure = value

    class Humidity(Mode):
        def __init__(self, leds, weather_station):
            super().__init__(leds, weather_station)
            self.current_humidity = self.weather_station.bme280.humidity
            self.max_humidity = 100
            self.min_humidity = 0

        def run(self):
            self.current_humidity = self.weather_station.bme280.humidity
            self.leds.display(self.current_humidity, self.min_humidity, self.max_humidity, Color.humidity_color)
            print(f'Humidity: {self.current_humidity:0.1f} %')

        def set_min(self, value):
            self.min_humidity = value

        def set_max(self, value):
            self.min_humidity = value

    class Altitude(Mode):
        def __init__(self, leds, weather_station):
            super().__init__(leds, weather_station)
            self.current_altitude = self.weather_station.bme280.altitude
            self.max_altitude = 130
            self.min_altitude = 110

        def run(self):
            self.current_altitude = self.weather_station.bme280.altitude
            self.leds.display(self.current_altitude, self.min_altitude, self.max_altitude, Color.altitude_color)
            print(f'Altitude: {self.current_altitude:0.2f} meters')

        def set_min(self, value):
            self.min_altitude = value

        def set_max(self, value):
            self.max_altitude = value

    def encoder_left_callback(self, c):
        if not self.is_selected:
            if GPIO.input(encoderRight) == 0:
                self.current_mode = (self.current_mode + 1) % len(self.modes)
                print(self.current_mode)
                print(c)

    def encoder_right_callback(self, c):
        if not self.is_selected:
            if GPIO.input(encoderLeft) == 0:
                self.current_mode = (self.current_mode + len(self.modes) - 1) % len(self.modes)
                print(self.current_mode)
                print(c)

    def green_button_callback(self, c):
        self.is_selected = True
        print(c)

    def red_button_callback(self, c):
        self.is_selected = False
        print(c)

    def current_activity(self):
        self.modes[self.current_mode].run()

    def display_menu(self):
        self.diods.display_menu(self.current_mode)

    # todo
    def set_params(self, command):
        print("min")
        print(command.split()[0])
        print("max")
        print(command.split()[1])
        self.modes[self.current_mode].set_min(command.split()[0])
        self.modes[self.current_mode].set_max(command.split()[1])


controller: MainController = MainController()


def setup():
    GPIO.add_event_detect(encoderLeft, GPIO.FALLING, callback=controller.encoder_left_callback, bouncetime=100)
    GPIO.add_event_detect(encoderRight, GPIO.FALLING, callback=controller.encoder_right_callback, bouncetime=100)
    GPIO.add_event_detect(buttonRed, GPIO.FALLING, callback=controller.red_button_callback, bouncetime=100)
    GPIO.add_event_detect(buttonGreen, GPIO.FALLING, callback=controller.green_button_callback, bouncetime=100)


if __name__ == "__main__":
    setup()
    while True:
        if controller.is_selected:
            # command = input('params')
            # if command != "":
            #     print(command)
            #     controller.set_params(command)
            controller.current_activity()
        else:
            controller.display_menu()
