#include "Arduino.h"
#include "LittleFS.h"
#include <Adafruit_PWMServoDriver.h>
#include <FastLED.h>
#include <Wire.h>

// #include <WiFiManager.h>
#include <ESPAsync_WiFiManager_Lite.h>
#include <AsyncElegantOTA.h>
#include <FS.h>
// #include <WebSocketsServer.h>

#include "consts.h"
#include "accelerometer.h"
#include "statebehaviour.h"

using namespace ace_routine;

CRGB leds[NUM_LEDS];

// SoftwareSerial mySoftwareSerial(D5, D6);  // RX, TX
// DFRobotDFPlayerMini myDFPlayer;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

AsyncWebServer server = AsyncWebServer(80);
// DNSServer dns;
// WebSocketsServer webSocket = WebSocketsServer(81);
// AsyncWiFiManager wifiManager(&server, &dns);

bool websocketStarted;
unsigned long nextWebSocketUpdateTime = 0;

int currentMoveSpeed = 0;
TurretStateBehaviour state;

void preloader(uint8_t led) {
  FastLED.clear();
  leds[led] = CRGB(255, 0, 0);
  FastLED.show();
}

void setup() {
  pwm.begin();
  pwm.setPWMFreq(FREQ);
  pwm.setPWM(ROTATE_SERVO_PIN, 0, map(90, 0, 180, FREQ_MINIMUM, FREQ_MAXIMUM));
  pwm.setPWM(WING_SERVO_PIN, 0,
             map(STATIONARY_ANGLE, 0, 180, FREQ_MINIMUM, FREQ_MAXIMUM));
  pwm.setPWM(CENTER_LED_PIN, 4096, 0);

  Serial.begin(19200);

  Serial.println("Trying WifiManager");
  ESPAsync_WiFiManager_Lite* wifiManager = new ESPAsync_WiFiManager_Lite();
  wifiManager->begin();

  // if (isConnected) {

  FastLED.addLeds<WS2812, RING_LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(84);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
    FastLED.show();
  }

  // currentState = TurretState::Idle;
  // currentManualState = ManualState::Idle;
  // currentTurretMode = TurretMode::Automatic;

  pinMode(WING_SWITCH_PIN, INPUT);
  // pinMode(D7, INPUT);

  state.wasOpen = isOpen();

  preloader(0);

  Serial.println(WiFi.localIP());

  AsyncElegantOTA.begin(&server);

  // MDNS.begin("portal");
  // MDNS.addService("http", "tcp", 80);

  // Initialize LittleFS
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  preloader(1);

  // startWebServer();
  preloader(2);

  // startWebSocket();
  preloader(3);

  setupAccelerometer();

  preloader(4);

  // delay(1000);
  // myDFPlayer.volume(15);

  preloader(5);

  preloader(6);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 0, 0);
    FastLED.show();
  }

  // previousTime = millis();
  //}
}

void loop() {
  // if (!isConnected) return;

  state.wingsOpen = isOpen();

  // webSocket.loop();

  // stateBehaviour();

  if (currentMoveSpeed > 0 && state.wasOpen && !state.wingsOpen) {
    currentMoveSpeed = 0;
    pwm.setPWM(WING_SERVO_PIN, 0,
               map(STATIONARY_ANGLE, 0, 180, FREQ_MINIMUM, FREQ_MAXIMUM));
  }

  state.wasOpen = state.wingsOpen;

  // if (websocketStarted && millis() > nextWebSocketUpdateTime) {
  //   nextWebSocketUpdateTime = millis() + 30;
  //   int a = analogRead(A0);

  //   updateAccelerometer();

  //   int16_t x = smoothX / measurements;
  //   int16_t y = smoothY / measurements;
  //   int16_t z = smoothZ / measurements;

  //   uint8_t values[] = {
  //       (x >> 8),
  //       (x & 0xFF),
  //       (y >> 8),
  //       (y & 0xFF),
  //       (z >> 8),
  //       (z & 0xFF),
  //       (!isOpen() ? 1 : 0),
  //       (digitalRead(D7) == HIGH ? 1 : 0),
  //       ((uint8_t)(a >> 8)) & 0xFF,
  //       ((uint8_t)a) & 0xFF,
  //       (uint8_t)currentState,
  //       (isPlayingAudio() ? 1 : 0),
  //   };
  //   webSocket.broadcastBIN(values, 12);
  // }
}

void startWebServer(AsyncWebServer server) {
  server.serveStatic("/", LittleFS, "/");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "index.html", String(), false, processor);
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      request->send(404);
    }
  });

  server.on("/set_mode", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("mode", true)) {
      AsyncWebParameter *modeParam = request->getParam("mode", true);
      state.currentTurretMode = (TurretMode)modeParam->value().toInt();
      currentRotateAngle = 90;
      request->send(200, "text/html", "State set");
    } else {
      request->send(200, "text/html", "Failed to set mode");
    }
  });

  server.on("/set_state", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("state", true)) {
      AsyncWebParameter *stateParam = request->getParam("state", true);
      int state = stateParam->value().toInt();
      // setState((TurretState)state);
      request->send(200, "text/html", "State set");
    } else {
      request->send(200, "text/html", "No state sent");
    }
  });

  server.on("/set_open", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (state.currentTurretMode == TurretMode::Manual) {
      if (request->hasParam("open", true)) {
        AsyncWebParameter *openParam = request->getParam("open", true);
        if (openParam->value().toInt() == 1) {
          state.setManualState(ManualState::Opening);
          request->send(200, "text/html", "Opening");
        } else {
          state.setManualState(ManualState::Closing);
          request->send(200, "text/html", "Closing");
        }
      } else {
        request->send(200, "text/html", "No state sent");
      }
    } else {
      request->send(200, "text/html", "Not in Manual mode");
    }
  });

  server.on("/set_angle", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("Set angle request");
    if (request->hasParam("angle", true)) {
      AsyncWebParameter *angleParam = request->getParam("angle", true);
      AsyncWebParameter *servoParam = request->getParam("servo", true);
      int angle = angleParam->value().toInt();
      int servo = servoParam->value().toInt();
      currentMoveSpeed = angle;
      if (servo == 0) {
        pwm.setPWM(
            WING_SERVO_PIN, 0,
            map(STATIONARY_ANGLE + angle, 0, 180, FREQ_MINIMUM, FREQ_MAXIMUM));
      } else {
        pwm.setPWM(ROTATE_SERVO_PIN, 0,
                   map(90 + angle, 0, 180, FREQ_MINIMUM, FREQ_MAXIMUM));
      }

      request->send(200, "text/html", "Angle set");
    } else {
      request->send(200, "text/html", "No angle sent");
    }
  });

  // server.on("/reset_wifi", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   wifiManager.resetSettings();
  //   WiFi.disconnect();
  //   request->send(200, "text/html", "Wifi reset");
  // });

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
}

String processor(const String &var) {
  if (var == "IP") return WiFi.localIP().toString();
  return String();
}