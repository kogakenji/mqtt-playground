#!/usr/bin/python3
# 
# test-messages.py - This script publish a random MQTT messages every 2 s.
#
# Copyright (c) 2013-2017, Fabian Affolter <fabian@affolter-engineering.ch>
# Released under the MIT license. See LICENSE file for details.
#
import random
import paho.mqtt.client as mqtt
import time


broker = '192.168.20.35'
port = 1883

mqttclient = mqtt.Client(client_id="mqtt-msg-generator",
                         protocol=mqtt.MQTTv311)
mqttclient.connect(broker, port=int(port))


def publish_humidity():
    """
    Publishes a random temperature between 15 and 40
    """
    humidity = random.randrange(10, 80)
    mqttclient.publish("humidity", humidity)
    time.sleep(3)


def publish_temperature():
    """
    Publishes a random temperature between 15 and 40
    """
    while True:
        temperature = random.randrange(15, 40)
        mqttclient.publish("temperature", temperature)
        time.sleep(3)
        publish_humidity()

# def publish_random():
#     element = 'home'
#     areas = ['front', 'back', 'kitchen', 'basement', 'living']
#     entrances = ['door', 'window']
#     states = ['true', 'false']

#     while True:
#         area = random.choice(areas)
#         if (area in ['basement', 'living']):
#             # topic = 'Casa'
#             topic = element + '/' + area + '/temp'
#             message = random.randrange(0, 30, 1)
#         else:
#             topic = element + '/' + area + '/' + random.choice(entrances)
#             message = random.choice(states)

#     mqttclient.publish(topic, message)
#     time.sleep(1)

if __name__ == "__main__":
    element = "home"
    print('Messages are published on topic %s/#... -> CTRL + C to shutdown' \
    % element)
    publish_temperature()
    publish_humidity()
