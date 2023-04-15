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
  delay(1000);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS storage failure");
  }

  Serial.println("Trying WifiManager");
  AsyncDNSServer dnsServer;
  ESPAsync_WiFiManager ESPAsync_wifiManager(&server, &dnsServer, "portal-turret");
  if (!ESPAsync_wifiManager.autoConnect("PortalTurretSetup", "areyoustillthere?")) {
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

  pinMode(MOTION_SENSOR_PIN, INPUT);
  // pinMode(WING_SWITCH_PIN, INPUT_PULLUP);
  pinMode(GUNS_LED_PIN, OUTPUT);
  pinMode(CENTER_LED_PIN, OUTPUT);

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

  state.wasOpen = isOpen();

  updateLEDPreloader();

  AsyncElegantOTA.begin(&server);

  updateLEDPreloader();

  setupWebserver(&server);
  webSocket.start();
  accelerometer.setup();

  updateLEDPreloader();

#ifdef USE_AUDIO
  hardwareSerial.begin(9600, SERIAL_8N1, MUSIC_RX_PIN, MUSIC_TX_PIN);
  delay(200);
  bool musicPlayerSetup = musicPlayer.begin(hardwareSerial);
  if (musicPlayerSetup) musicPlayer.volume(15);
#endif

  updateLEDPreloader();

  digitalWrite(CENTER_LED_PIN, HIGH);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 0, 0);
    FastLED.show();
  }

  state.init();
}

void loop() {
  if (!diagnosing) {
    state.update();
  } else {
    switch (diagnoseAction) {
      case 0:
        Serial.println("Open wings");
        wingServo.write(STATIONARY_ANGLE - 90);
        delay(250);
        wingServo.write(STATIONARY_ANGLE);
        break;
      case 1:
        Serial.println("Close wings");
        wingServo.write(STATIONARY_ANGLE + 90);
        delay(250);
        wingServo.write(STATIONARY_ANGLE);
        break;
      case 2:
        Serial.println("Rotate left");
        rotateServo.write(50);
        delay(1000);
        rotateServo.write(90);
        break;
      case 3:
        Serial.println("Rotate right");
        rotateServo.write(130);
        delay(1000);
        rotateServo.write(90);
        break;
      case 4:
        Serial.println("Blink gun leds");
        analogWrite(GUNS_LED_PIN, 255);
        delay(1000);
        analogWrite(GUNS_LED_PIN, 0);
        break;
      case 5:
        Serial.println("Animate led ring");
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
        Serial.println("Play sound");
        musicPlayer.playFolder(1, random(1, 9));
        break;
#endif
    }
    diagnoseAction = -1;
  }

  // TODO: This could be moved into state
  state.wingsOpen = isOpen();
  if (currentMoveSpeed > 0 && state.wasOpen && !state.wingsOpen) {
    currentMoveSpeed = 0;
    delay(CLOSE_STOP_DELAY);
    wingServo.write(STATIONARY_ANGLE);
  }
  state.wasOpen = state.wingsOpen;

  accelerometer.update();
  webSocket.update();
}
