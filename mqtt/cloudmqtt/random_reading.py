#!/usr/bin/python3
import paho.mqtt.client as mqtt
import time
import random

client = mqtt.Client()

# sets the username and password of the mqtt broker
client.username_pw_set("ijuqmbhs", "QaqNMZ3yYdwE")
client.connect('soldier.cloudmqtt.com', 11515)
client.loop_start()

while True:
    # generate random fake temperature
    fake_temp = random.randint(25, 50)
    print(fake_temp)
    
    # publish to the topic
    client.publish("/auscasa/test", fake_temp)
    time.sleep(1*60)  # wait 1 between readings
