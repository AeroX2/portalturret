#ifndef SERVER_H
#define SERVER_H

#include <ESPAsyncWebServer.h>
#include <Wifi.h>

#include "LittleFS.h"
#include "externs.h"
#include "statebehaviour.h"

inline void setupWebserver(AsyncWebServer* server) {
  Serial.println("Start webserver");
  server->serveStatic("/", LittleFS, "/");
  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html");
  });

  server->onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      request->send(404);
    }
  });

  server->on("/set_mode", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("mode", true)) {
      AsyncWebParameter *modeParam = request->getParam("mode", true);
      state.currentTurretMode = (TurretMode)modeParam->value().toInt();
      currentRotateAngle = 90;
      request->send(200, "text/html", "State set");
    } else {
      request->send(200, "text/html", "Failed to set mode");
    }
  });

  server->on("/set_state", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("state", true)) {
      AsyncWebParameter *stateParam = request->getParam("state", true);
      int turretState = stateParam->value().toInt();
      state.setState((TurretState)turretState);
      request->send(200, "text/html", "State set");
    } else {
      request->send(200, "text/html", "No state sent");
    }
  });

  server->on("/diagnose", HTTP_GET, [](AsyncWebServerRequest *request) {
    diagnosing = true;
    request->send(LittleFS, "/diagnose.html");
  });

  server->on("/diagnose", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("action", true)) {
      AsyncWebParameter *stateParam = request->getParam("action", true);
      diagnoseAction = stateParam->value().toInt();
      request->send(200, "text/html", "Diagnose");
    } else {
      request->send(200, "text/html", "No Action Sent");
    }
  });

  server->on("/set_open", HTTP_POST, [](AsyncWebServerRequest *request) {
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

  server->on("/set_angle", HTTP_POST, [](AsyncWebServerRequest *request) {
    // Serial.println("Set angle request");
    if (request->hasParam("angle", true)) {
      AsyncWebParameter *angleParam = request->getParam("angle", true);
      AsyncWebParameter *servoParam = request->getParam("servo", true);
      int angle = angleParam->value().toInt();
      int servo = servoParam->value().toInt();
      currentMoveSpeed = angle;
      if (servo == 0) {
        wingServo.write(STATIONARY_ANGLE + angle);
      } else {
        rotateServo.write(90 + angle);
      }

      request->send(200, "text/html", "Angle set");
    } else {
      request->send(200, "text/html", "No angle sent");
    }
  });

  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  Serial.println("server.begin()");
  server->begin();
}

#endif