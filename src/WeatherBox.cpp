/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/admin/CTD_2023/WeatherBox/src/WeatherBox.ino"
#include "MQTT.h"
#include "oled-wing-adafruit.h"
#include "env.h"
#include "blynk.h"

void setup();
void callback2();
void loop();
void connectingAnimation();
void resetDisplay();
void callback(char *topic, byte *payload, unsigned int length);
void displayWeatherDataOutSide();
#line 6 "/Users/admin/CTD_2023/WeatherBox/src/WeatherBox.ino"
SYSTEM_THREAD(ENABLED);

void callback(char* topic, byte* payload, unsigned int length);

MQTT client("lab.thewcl.com", 1883, callback);

OledWingAdafruit display;

int currentTempF_outside = 0.0;
float currentWindSpeed_outside = 0.0;
int currentPrecipitation_outside = 0.0;
int currentHumidity_outside = 0.0;
int iAmount = 0;
char *currentPrecipitationType_outside = "";
bool updateData = false;

Timer getWeatherData(3600000, callback2, false);
Timer startAnimation(500, connectingAnimation, false);

BlynkTimer timer;

void setup() {
  display.setup();
  resetDisplay();
  display.display();

  startAnimation.start();

  //Blynk.begin(BLYNK_AUTH_TOKEN);

  //Serial.begin(9600);
  //while (!Serial.isConnected()) {}

  getWeatherData.start();
  updateData = true;

  client.connect(System.deviceID());
  client.subscribe("WeatherBox/PostData/+");
}

void callback2() {
  updateData = true;
}

void loop() {
  //Blynk.run();
  //timer.run();
  display.loop();

  if (client.isConnected()) {
    startAnimation.stop();
    client.loop();

    if (updateData) {
      client.publish("WeatherBox/GetData", "GetData");
      updateData = false;
    }

    displayWeatherDataOutSide();
  } else {
    client.connect(System.deviceID());
    client.subscribe("WeatherBox/PostData/+");
  }
}

void connectingAnimation() {
  resetDisplay();
  display.print("Connecting");
  for (int i = 0; i < iAmount; i++) {
    display.print(".");
  }
  iAmount++;
  if (iAmount > 3) {
    iAmount = 0;
  }
  display.display();
}

void resetDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
}

void callback(char *topic, byte *payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;


  if (String(topic).equals("WeatherBox/PostData/Temperature")) {
    int rf = atof(p);

    currentTempF_outside = rf;
  }
  if (String(topic).equals("WeatherBox/PostData/WindSpeed")) {
    float rf = atof(p);

    currentWindSpeed_outside = rf;
  }
  if (String(topic).equals("WeatherBox/PostData/Precipitation")) {
    int rf = atof(p);

    currentPrecipitation_outside = rf;
  }
  if (String(topic).equals("WeatherBox/PostData/Humidity")) {
    float rf = atof(p);

    currentHumidity_outside = rf;
  }
}

void displayWeatherDataOutSide() {
  resetDisplay();
  display.print("Temperature: ");
  display.print(currentTempF_outside);
  display.println("F");
  display.print("Wind Speed: ");
  display.print(currentWindSpeed_outside);
  display.println("mph");
  display.print("Chance for Rain: ");
  display.print(currentPrecipitation_outside);
  display.println("%");
  display.print("Humidity: ");
  display.print(currentHumidity_outside);
  display.print("%");
  display.display();
}