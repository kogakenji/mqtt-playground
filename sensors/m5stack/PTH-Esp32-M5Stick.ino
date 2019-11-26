#include <WiFi.h>
#include <M5StickC.h>
#include <ArduinoJson.h>
#include <Adafruit_BMP280.h>
#include "DHT12.h"
#include <PubSubClient.h>

#define DEBUG
#define INTERVALO_ENVIO       20000
#define MQTT_KEEPALIVE 10

DHT12 dht12;
//BMM150 bmm = BMM150();
//bmm150_mag_data value_offset;
Adafruit_BMP280 bme;

//informações da rede WIFI
const char* ssid = "toi";                 //SSID da rede WIFI
const char* password =  "YourPassword";    //senha da rede wifi

//informações do broker MQTT - Verifique as informações geradas pelo CloudMQTT
const char* mqttServer = "192.168.0.169";   //server
//const char* mqttServer = "iot.eclipse.org";   //server
//const char* mqttServer = "broker.hivemq.com";   //server
const char* mqttUser = "eugenio";              //user
const char* mqttPassword = "eugenio.";      //password
const int mqttPort = 1883;                     //port 1883 (unencripted) or 8883 (encripted (8883)
const char* mqttTopicSub = "DC/PTH-002";           //tópico que sera assinado

int ultimoEnvioMQTT = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Initialize the M5StickC object
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(1);

  Wire.begin(0,26);

  WiFi.begin(ssid, password);
  Serial.println(WiFi.status());

  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
    delay(2000);
#ifdef DEBUG
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,0);
    M5.Lcd.println("Conectando ao WiFi..");
    Serial.println("Conectando ao WiFi..");
#endif
  }
#ifdef DEBUG
  M5.Lcd.print("Conectado ao WiFi");
  Serial.println("Conectado na rede WiFi");
#endif

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
#ifdef DEBUG
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,0);
    M5.Lcd.println("Conectando ao Broker MQTT...");
    Serial.println("Conectando ao Broker MQTT...");
#endif

    if (client.connect("ESP32Client-001", mqttUser, mqttPassword )) {
#ifdef DEBUG
      M5.Lcd.print("Conectado ao Broker");
      Serial.println("....Conectado");
#endif
    }
    else {
#ifdef DEBUG
      Serial.print("falha estado  ");
      Serial.print(client.state());
#endif
      delay(2000);
    }
  }
  //dht.begin();
  if (!bme.begin(0x76)){  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

}

//função pra reconectar ao servido MQTT
void reconect() {
  //Enquanto estiver desconectado
  while (!client.connected()) {
#ifdef DEBUG
    Serial.println("Tentando conectar ao servidor MQTT");
#endif

    bool conectado = strlen(mqttUser) > 0 ?
                     client.connect("ESP32Client-001", mqttUser, mqttPassword) :
                     client.connect("ESP32Client-001");

    if (conectado) {
#ifdef DEBUG
      Serial.println("Conectado!");
#endif
      //subscreve no tópico
      client.subscribe(mqttTopicSub, 1); //nivel de qualidade: QoS 1
    } else {
#ifdef DEBUG
      Serial.println("Falha durante a conexão.Code: ");
      Serial.println( String(client.state()).c_str());
      Serial.println("Tentando novamente em 10 s");
#endif
      //Aguarda 10 segundos
      delay(10000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  //armazena msg recebida em uma sring
  payload[length] = '\0';
  String strMSG = String((char*)payload);

#ifdef DEBUG
  Serial.print("Mensagem chegou do tópico: ");
  Serial.println(topic);
  Serial.print("Mensagem:");
  Serial.print(strMSG);
  Serial.println();
  Serial.println("-----------------------");
#endif
}
void loop() {
  //envia a cada X segundos
  if (!client.connected()) {
    reconect();
  }

  if ((millis() - ultimoEnvioMQTT) > INTERVALO_ENVIO)
  {
    enviaDHT();
    ultimoEnvioMQTT = millis();
    Serial.print("ultimoEnvioMQTT: ");
    Serial.println(ultimoEnvioMQTT);
  }
  client.loop();
}

//função para leitura do DHT11
void enviaDHT() {

  char MsgUmidadeMQTT[10];
  char MsgTemperaturaMQTT[10];
  char MsgPressureMQTT[20];

  float umidade = dht12.readHumidity();
  float temperatura = dht12.readTemperature();
  float pressure = bme.readPressure();

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0,0);
    
  // Se as variáveis temperatura e umidade não forem valores válidos, acusará falha de leitura.
  if (isnan(temperatura) || isnan(umidade) || isnan(pressure) )
  {
#ifdef DEBUG
    Serial.println("Falha na leitura dos sensores...");
#endif
  }
  else
  {
    //Imprime os dados no monitor serial
#ifdef DEBUG
    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.println(" %");
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPA");
    M5.Lcd.print("Temperatura: ");
    M5.Lcd.println(temperatura );
    M5.Lcd.print("Umidade: ");
    M5.Lcd.println(umidade );
#endif
    
    if (client.connected()) {
      sprintf(MsgTemperaturaMQTT, "%f", temperatura);
      if (client.publish("DC/SensorT-003", MsgTemperaturaMQTT)) {
        Serial.println("Publish temperatura ok");
      }
      else {
        Serial.println("Topic DC/SensorT-003: Publish failed");
      }
    }

    if (client.connected()) {
      sprintf(MsgUmidadeMQTT, "%f", umidade);
      if (client.publish("DC/SensorU-003", MsgUmidadeMQTT)) {
        Serial.println("Publish umidade ok");
      }
      else {
        Serial.println("Topic DC/SensorU-003: Publish failed");
      }
    }
    
    if (client.connected()) {
      sprintf(MsgPressureMQTT, "%f", pressure);
      if (client.publish("DC/SensorP-003", MsgPressureMQTT)) {
        Serial.println("Publish pressao ok");
      }
      else {
        Serial.println("Topic DC/SensorP-003: Publish failed");
      }
    }
   Serial.println("Antes bme ...");
    if (!bme.begin(0x76)){  
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      while (1);
    }
    Serial.println("Antes bme ...");

  
  }
}
