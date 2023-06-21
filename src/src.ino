#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <arduino.h>
#include <BlynkSimpleEsp8266.h>

#define DHT_PIN 2              // Pino de dados do sensor DHT11
#define DHT_TYPE DHT11         // Tipo do sensor DHT11
#define SOIL_MOISTURE_PIN A0   // Pino do sensor de umidade de solo
#define RAIN_SENSOR_PIN A0     // Pino do sensor de chuva
#define PRESENCE_SENSOR_PIN 3  // Pino do sensor de presença
#define RELAY_PIN 4            // Pino do relé para acionar a bomba

char auth[] = "z_lwBInXPXKQwPYvPPQNV4cce8Q2amsD";
char ssid[] = "251241VCT3_2.4G"; //wifi
char pass[] = "03997438200"; //senha do wifi

DHT dht(DHT_PIN, DHT_TYPE);

BlynkTimer timer;

void sendSensorData()
{
  // Leitura dos sensores
  float humidity = dht.read();
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  int rainLevel = analogRead(RAIN_SENSOR_PIN);
  int presence = digitalRead(PRESENCE_SENSOR_PIN);

  // Envio dos dados para o aplicativo Blynk
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, soilMoisture);
  Blynk.virtualWrite(V3, rainLevel);
  Blynk.virtualWrite(V4, presence);
}

void checkSoilMoisture()
{
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);

  if (soilMoisture < 500)
  {
    // Solo está quase seco, acionar a bomba
    char high_temperature_warning[] = "Solo quase seco! Bomba acionada";

    digitalWrite(RELAY_PIN, HIGH);
    Blynk.logEvent(high_temperature_warning);
    // Blynk.notify("Solo quase seco! Bomba acionada.");
  }
  else
  {
    // Solo está úmido, desligar a bomba
    digitalWrite(RELAY_PIN, LOW);
  }
}

void setup()
{
  // Inicialização do monitor serial
  Serial.begin(9600);

  // Inicialização do sensor DHT11
  dht.begin();

  // Conexão com o Wi-Fi
  Blynk.begin(auth, ssid, pass);

  // Configuração dos pinos
  pinMode(PRESENCE_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Configuração dos intervalos de leitura e verificação
  timer.setInterval(1000L, sendSensorData);
  timer.setInterval(5000L, checkSoilMoisture);
}

void loop()
{
  Blynk.run();
  timer.run();
}