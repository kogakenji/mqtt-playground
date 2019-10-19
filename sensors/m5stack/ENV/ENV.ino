/*
    note: need add library Adafruit_BMP280 from library manage
    Github: https://github.com/adafruit/Adafruit_BMP280_Library
*/

#include <M5StickC.h>
#include "DHT12.h"
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "bmm150.h"
#include "bmm150_defs.h"

#include "EspMQTTClient.h"

DHT12 dht12; 
BMM150 bmm = BMM150();
bmm150_mag_data value_offset;
Adafruit_BMP280 bme;


//informações da rede WIFI
const char* ssid = "conexaolocal";                 //SSID da rede WIFI
const char* password =  "b0ltz1nh0";    //senha da rede wifi

//informações do broker MQTT - Verifique as informações geradas pelo CloudMQTT
const char* mqttServer = "soldier.cloudmqtt.com";   //server
//const char* mqttServer = "iot.eclipse.org";   //server
//const char* mqttServer = "broker.hivemq.com";   //server
const char* mqttUser = "ijuqmbhs";              //user
const char* mqttPassword = "QaqNMZ3yYdwE";      //password
const int mqttPort = 11515;                     //port 1883 (unencripted) or 8883 (encripted (8883)
//const char* mqttTopicSub = "casa/umidade-002";           //tópico que sera assinado
const char* mqttTopicTempSub = "/auscasa/m5stickc/temperature";           //tópico temp que sera assinado
const char* mqttTopicHumSub = "/auscasa/m5stickc/humidity";           //tópico humidity que sera assinado

EspMQTTClient client(
  ssid,
  password,
  mqttServer,  // MQTT Broker server ip
  mqttUser,   // Can be omitted if not needed
  mqttPassword,   // Can be omitted if not needed
  "M5stickC",     // Client name that uniquely identify your device
  mqttPort              // The MQTT port, default to 1883. this line can be omitted
);

void calibrate(uint32_t timeout)
{
  int16_t value_x_min = 0;
  int16_t value_x_max = 0;
  int16_t value_y_min = 0;
  int16_t value_y_max = 0;
  int16_t value_z_min = 0;
  int16_t value_z_max = 0;
  uint32_t timeStart = 0;

  bmm.read_mag_data();  
  value_x_min = bmm.raw_mag_data.raw_datax;
  value_x_max = bmm.raw_mag_data.raw_datax;
  value_y_min = bmm.raw_mag_data.raw_datay;
  value_y_max = bmm.raw_mag_data.raw_datay;
  value_z_min = bmm.raw_mag_data.raw_dataz;
  value_z_max = bmm.raw_mag_data.raw_dataz;
  delay(100);

  timeStart = millis();
  
  while((millis() - timeStart) < timeout)
  {
    bmm.read_mag_data();
    
    /* Update x-Axis max/min value */
    if(value_x_min > bmm.raw_mag_data.raw_datax)
    {
      value_x_min = bmm.raw_mag_data.raw_datax;
      // Serial.print("Update value_x_min: ");
      // Serial.println(value_x_min);

    } 
    else if(value_x_max < bmm.raw_mag_data.raw_datax)
    {
      value_x_max = bmm.raw_mag_data.raw_datax;
      // Serial.print("update value_x_max: ");
      // Serial.println(value_x_max);
    }

    /* Update y-Axis max/min value */
    if(value_y_min > bmm.raw_mag_data.raw_datay)
    {
      value_y_min = bmm.raw_mag_data.raw_datay;
      // Serial.print("Update value_y_min: ");
      // Serial.println(value_y_min);

    } 
    else if(value_y_max < bmm.raw_mag_data.raw_datay)
    {
      value_y_max = bmm.raw_mag_data.raw_datay;
      // Serial.print("update value_y_max: ");
      // Serial.println(value_y_max);
    }

    /* Update z-Axis max/min value */
    if(value_z_min > bmm.raw_mag_data.raw_dataz)
    {
      value_z_min = bmm.raw_mag_data.raw_dataz;
      // Serial.print("Update value_z_min: ");
      // Serial.println(value_z_min);

    } 
    else if(value_z_max < bmm.raw_mag_data.raw_dataz)
    {
      value_z_max = bmm.raw_mag_data.raw_dataz;
      // Serial.print("update value_z_max: ");
      // Serial.println(value_z_max);
    }
    
    Serial.print(".");
    delay(1);

  }

  value_offset.x = value_x_min + (value_x_max - value_x_min)/2;
  value_offset.y = value_y_min + (value_y_max - value_y_min)/2;
  value_offset.z = value_z_min + (value_z_max - value_z_min)/2;
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
//  client.subscribe("mytopic/test", [](const String & payload) {
//    Serial.println(payload);
//  });
//
//  // Publish a message to "mytopic/test"
//  client.publish("mytopic/test", "This is a message"); // You can activate the retain flag by setting the third parameter to true
//  Serial.println("Enviada mensagem no onConnectionEstablished!");
//  // Execute delayed instructions
//  client.executeDelayed(5 * 1000, []() {
//    client.publish("mytopic/test", "This is a message sent 5 seconds later");
//  });
}

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  Wire.begin(0,26);
  
      
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.println("ENV TEST");
  pinMode(M5_BUTTON_HOME, INPUT);

  if(bmm.initialize() == BMM150_E_ID_NOT_CONFORM) {
    Serial.println("Chip ID can not read!");
    while(1);
  } else {
    Serial.println("Initialize done!");
  }
  if (!bme.begin(0x76)){  
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      while (1);
  }
  calibrate(10);
  Serial.print("\n\rCalibrate done..");
}
uint8_t setup_flag = 1;
int count = 0;
void loop() {
  // put your main code here, to run repeatedly:
  float tmp = dht12.readTemperature();
  float hum = dht12.readHumidity();
  
  char MsgTemperaturaMQTT[10];
  sprintf(MsgTemperaturaMQTT, "%f", tmp);
  
  char MsgHumidityMQTT[10];
  sprintf(MsgHumidityMQTT, "%f", hum);
  
  if (count == 900){ // wait 15 minutes to send a message to the broker, as I put delay to be 1000 = 1second
    client.publish(mqttTopicTempSub, MsgTemperaturaMQTT); // publish temperature  
    client.publish(mqttTopicHumSub, MsgHumidityMQTT); // publish temperature  
    Serial.print("Enviada mensagem de temperatura ao mqtt cloud dentro do loop: ");
    Serial.println(MsgTemperaturaMQTT);
    Serial.print("Enviada mensagem de humidade ao mqtt cloud dentro do loop: ");
    Serial.println(MsgHumidityMQTT);
    count = 0;
  }
  count = count + 1;
//  Serial.println(count);
  
  
  
  M5.Lcd.setCursor(0, 20, 2);
  M5.Lcd.printf("Temp: %2.1f Humi: %2.0f%%", tmp, hum);

  client.loop();
  bmm150_mag_data value;
  bmm.read_mag_data();

  value.x = bmm.raw_mag_data.raw_datax - value_offset.x;
  value.y = bmm.raw_mag_data.raw_datay - value_offset.y;
  value.z = bmm.raw_mag_data.raw_dataz - value_offset.z;

  float xyHeading = atan2(value.x, value.y);
  float zxHeading = atan2(value.z, value.x);
  float heading = xyHeading;

  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
  float headingDegrees = heading * 180/M_PI; 
  float xyHeadingDegrees = xyHeading * 180 / M_PI;
  float zxHeadingDegrees = zxHeading * 180 / M_PI;

//  Serial.print("Heading: ");
//  Serial.println(headingDegrees);
//  Serial.print("xyHeadingDegrees: ");
//  Serial.println(xyHeadingDegrees);
//  Serial.print("zxHeadingDegrees: ");
//  Serial.println(zxHeadingDegrees);
  M5.Lcd.setCursor(0, 40, 2);
  M5.Lcd.printf("headingDegrees: %2.1f", headingDegrees);
  
  float pressure = bme.readPressure();
  M5.Lcd.setCursor(0, 60, 2);
  M5.Lcd.printf("pressure: %2.1f", pressure);
  delay(1000); //delay of 1 second between measurements.

  if(!setup_flag){
     setup_flag = 1;

     if(bmm.initialize() == BMM150_E_ID_NOT_CONFORM) {
    Serial.println("Chip ID can not read!");
    while(1);
  } else {
    Serial.println("Initialize done!");
  }
  if (!bme.begin(0x76)){  
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      while (1);
  }
  calibrate(10);
  Serial.print("\n\rCalibrate done..");
 }


 if(digitalRead(M5_BUTTON_HOME) == LOW){
  setup_flag = 0;
  while(digitalRead(M5_BUTTON_HOME) == LOW);
 }
}
