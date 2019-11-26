#!/usr/bin/python3
# Copyright (c) 2010,2011 Roger Light <roger@atchoo.org>
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyrightras
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# 3. Neither the name of mosquitto nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

import paho.mqtt.client as mqtt
# import os
# from urllib.parse import urlparse
import random
import time
import LM75

# Define event callbacks


def on_connect(mosq, obj, rc, other_unkown_parameter):
    # print ("on_connect:: Connected with result code "+ str ( rc ) )
    # print("rc: " + str(rc))
    print("")


def on_message(mosq, obj, msg):
    # print ("on_message:: this means  I got a message from brokerfor this topic")
    # print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
    print("")


def on_publish(mosq, obj, mid):
    # print("mid: " + str(mid))
    print("")


def on_subscribe(mosq, obj, mid, granted_qos):
    print("This means broker has acknowledged my subscribe request")
    print("Subscribed: " + str(mid) + " " + str(granted_qos))


def on_log(mosq, obj, level, string):
    print(string)


client = mqtt.Client()
# Assign event callbacks
client.on_message = on_message
client.on_connect = on_connect
client.on_publish = on_publish
client.on_subscribe = on_subscribe

# Uncomment to enable debug messages
client.on_log = on_log


# user name has to be called before connect - my notes.
client.username_pw_set("ijuqmbhs", "QaqNMZ3yYdwE")

# client.connect('soldier.cloudmqtt.com', 11515, 60)
client.connect('soldier.cloudmqtt.com', 11515)
client.loop_start()
client.subscribe("/auscasa", 0)

sensor = LM75.LM75()

run = True
while run:
    # msg = random.randint(15, 50)
    celsius_temp = sensor.getCelsius()
    print(celsius_temp)
    client.publish("/auscasa/temperature", celsius_temp)
    time.sleep(15*60)  # wait 15 minutes between measurements
    # client.publish("/auscasa", "OFF")
    # time.sleep(2)
