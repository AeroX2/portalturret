#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <Arduino.h>
#include <WebSocketsServer.h>

#include "utils.h"
#include "externs.h"
#include "statebehaviour.h"

class Websocket {
 public:
  void start();
  void update();

 private:
  bool started = false;
  unsigned long nextWebSocketUpdateTime = 0;
  WebSocketsServer webSocket = WebSocketsServer(81);
};

static Websocket webSocket;

#endif