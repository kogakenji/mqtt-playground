#!/usr/bin/python3

import paho.mqtt.client as mqtt
import LM75
import time


client = mqtt.Client()

# user name has to be called before connect - my notes.
client.username_pw_set("ijuqmbhs", "QaqNMZ3yYdwE")

client.connect('soldier.cloudmqtt.com', 11515)
client.loop_start()
sensor = LM75.LM75()

while True:
    celsius_temp = sensor.getCelsius()
    print(celsius_temp)
    client.publish("/auscasa/temperature", celsius_temp)
    time.sleep(15*60)  # wait 15 minutes 

