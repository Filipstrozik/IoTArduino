#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import tkinter
import sqlite3
import time

# The broker name or IP address.
broker = "localhost"
# broker = "127.0.0.1"
# broker = "10.0.0.1"

# The MQTT client.
client = mqtt.Client()

# Thw main window.
# window = tkinter.Tk()

def process_message(client, userdata, message):
    # Decode message.
    # message_decoded = (str(message.payload.decode("utf-8"))).split(".")
    card_id, log_time = str(message.payload.decode('utf-8')).split('@')

    # Print message to console.
    if card_id != "Client connected" and card_id != "Client disconnected":
        print(log_time + ", " + card_id + " used the RFID card.")

        # Save to sqlite database.
        connention = sqlite3.connect("logs.db")
        cursor = connention.cursor()
        cursor.execute("INSERT INTO logs (card_id, time) VALUES (?, ?)",
                       (card_id, log_time))
        connention.commit()
        connention.close()
    else:
        print(card_id + " : " + log_time)


def print_log_to_window():
    connention = sqlite3.connect("logs.db")
    cursor = connention.cursor()
    cursor.execute("SELECT * FROM logs")
    log_entries = cursor.fetchall()
    labels_log_entry = []
    print_log_window = tkinter.Tk()

    for log_entry in log_entries:
        labels_log_entry.append(tkinter.Label(print_log_window, text=(
            "On %s, %s used the terminal" % (log_entry[0], log_entry[1]))))

    for label in labels_log_entry:
        label.pack(side="top")

    connention.commit()
    connention.close()

    # Display this window.
    print_log_window.mainloop()


def create_main_window():
    window.geometry("250x100")
    window.title("RECEIVER")
    label = tkinter.Label(window, text="Listening to the MQTT")
    exit_button = tkinter.Button(window, text="Stop", command=window.quit)
    print_log_button = tkinter.Button(
        window, text="Print log", command=print_log_to_window)

    label.pack()
    exit_button.pack(side="right")
    print_log_button.pack(side="right")


def connect_to_broker():
    # Connect to the broker.
    client.connect(broker)
    # Send message about conenction.
    client.on_message = process_message
    # Starts client and subscribe.
    client.loop_start()
    client.subscribe("id/card")


def disconnect_from_broker():
    # Disconnet the client.
    client.loop_stop()
    client.disconnect()


def run_receiver():
    connect_to_broker()
    #this
    inp = ""
    while inp != "exit":
        inp = input()
   # or
    # create_main_window()
    # Start to display window (It will stay here until window is displayed)
    # window.mainloop()
    disconnect_from_broker()


if __name__ == "__main__":
    run_receiver()
