#include "websocket.h"

static void event(uint8_t num, WStype_t type, uint8_t *payload,
                    size_t length) {
  uint8_t transferType;

  // When a WebSocket message is received
  switch (type) {
    case WStype_ERROR:
      // Serial.printf("Error: [%f]", payload);
      break;
    case WStype_BIN:
      switch (payload[0]) {
        case 0:
          if (isOpen()) {
            rotateServo.write(payload[1]);
          }
          break;
        case 1:
          state.setManualState(ManualState::Firing);
          break;
        case 2:
          if (payload[1] == 0) {
            currentRotateDirection = 0;
          } else if (payload[1] == 1) {
            currentRotateDirection = 1;
          } else if (payload[1] == 2) {
            currentRotateDirection = -1;
          }
          break;
      }
      break;
    case WStype_DISCONNECTED:  // if the websocket is disconnected
      break;
    case WStype_CONNECTED:  // if a new websocket connection is established
    //   IPAddress ip = webSocket.remoteIP(num);
      // Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0],
      // ip[1], ip[2], ip[3], payload);
      break;
  }
}

void Websocket::start() {
  // Start a WebSocket server
  webSocket.begin();
  webSocket.onEvent(event);  // if there's an incomming websocket message, go to
                        // function 'webSocketEvent'
  started = true;
  // Serial.println("WebSocket server started.");
}

void Websocket::update() {
  if (started && millis() > nextWebSocketUpdateTime) {
    nextWebSocketUpdateTime = millis() + 30;
    int a = 0; //analogRead(A0);

    int16_t x = accelerometer.getRealX();
    int16_t y = accelerometer.getRealY();
    int16_t z = accelerometer.getRealZ();

    uint8_t values[] = {
        (x >> 8),
        (x & 0xFF),
        (y >> 8),
        (y & 0xFF),
        (z >> 8),
        (z & 0xFF),
        (!isOpen() ? 1 : 0),
        0,
        // (digitalRead(D7) == HIGH ? 1 : 0),
        ((uint8_t)(a >> 8)) & 0xFF,
        ((uint8_t)a) & 0xFF,
        (uint8_t)state.currentState,
        (isPlayingAudio() ? 1 : 0),
    };
    webSocket.broadcastBIN(values, 12);
  }

  webSocket.loop();
}

