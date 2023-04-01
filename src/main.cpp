// General
#include "Arduino.h"

// Network
#include <AsyncElegantOTA.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsync_WiFiManager.h>
#include <Wifi.h>

// Devices
#include <FastLED.h>
// #include "DFRobotDFPlayerMini.h"

// Storage
#include "LittleFS.h"

#include "consts.h"
#include "externs.h"
#include "statebehaviour.h"
#include "server.h"
#include "websocket.h"

AsyncWebServer server = AsyncWebServer(80);

void updateLEDPreloader() {
  int t = floor(millis() / 10);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB((i + t) % 8 == 0 ? 255 : 0, 0, 0);
    FastLED.show();
  }
}

void setup() {
  Serial.begin(9600);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS storage failure");
  }

  Serial.println("Trying WifiManager");

  AsyncDNSServer dnsServer;
  ESPAsync_WiFiManager ESPAsync_wifiManager(&server, &dnsServer, "portal-turret");
  ESPAsync_wifiManager.setDebugOutput(true);
  if (!ESPAsync_wifiManager.startConfigPortal("PortalTurretSetup", "areyoustillthere?")) {
    Serial.println("Not connected to WiFi but continuing anyway.");
  } else {
    Serial.println("WiFi connected...yeey :)");
    Serial.println(WiFi.localIP());
  }

  FastLED.addLeds<WS2812, RING_LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(84);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
    FastLED.show();
  }

  pinMode(GUNS_LED_PIN, OUTPUT);

  wingServo.attach(WING_SERVO_PIN);
  rotateServo.attach(ROTATE_SERVO_PIN);

  rotateServo.write(90);
  delay(250);
  wingServo.write(STATIONARY_ANGLE + 90);
  while (isOpen()) {
    delay(10);
  }
  delay(CLOSE_STOP_DELAY);
  wingServo.write(STATIONARY_ANGLE);

  pinMode(WING_SWITCH_PIN, INPUT_PULLUP);

  updateLEDPreloader();

  state.wasOpen = isOpen();

  AsyncElegantOTA.begin(&server);

  updateLEDPreloader();

  setupWebserver(&server);
  webSocket.start();
  accelerometer.setup();

  updateLEDPreloader();

  updateLEDPreloader();

#ifdef USE_AUDIO
  mySoftwareSerial.begin(9600);
  delay(200);
  myDFPlayerSetup = myDFPlayer.begin(mySoftwareSerial);
  if (myDFPlayerSetup) myDFPlayer.volume(15);
#endif

  updateLEDPreloader();

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 0, 0);
    FastLED.show();
  }

  state.init();
}

void loop() {
  state.wingsOpen = isOpen();

  if (!state.diagnoseMode) {
    state.update();
  } else {
    switch (state.diagnoseAction) {
      case 0:
        wingServo.write(STATIONARY_ANGLE - 90);
        delay(250);
        wingServo.write(STATIONARY_ANGLE);
        break;
      case 1:
        wingServo.write(STATIONARY_ANGLE + 90);
        delay(250);
        wingServo.write(STATIONARY_ANGLE);
        break;
      case 2:
        rotateServo.write(50);
        delay(1000);
        rotateServo.write(90);
        break;
      case 3:
        rotateServo.write(130);
        delay(1000);
        rotateServo.write(90);
        break;
      case 4:
        analogWrite(GUNS_LED_PIN, 255);
        delay(1000);
        analogWrite(GUNS_LED_PIN, 0);
        break;
      case 5:
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        FastLED.show();
        delay(1000);
        fill_solid(leds, NUM_LEDS, CRGB::Green);
        FastLED.show();
        delay(1000);
        fill_solid(leds, NUM_LEDS, CRGB::Blue);
        FastLED.show();
        delay(1000);
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
        break;
#ifdef USE_AUDIO
      case 6:
        myDFPlayer.playFolder(1, random(1, 9));
        break;
#endif
    }
    state.diagnoseAction = -1;
  }

  if (currentMoveSpeed > 0 && state.wasOpen && !state.wingsOpen) {
    currentMoveSpeed = 0;
    delay(CLOSE_STOP_DELAY);
    wingServo.write(STATIONARY_ANGLE);
  }

  state.wasOpen = state.wingsOpen;

  accelerometer.update();
  webSocket.update();
}
