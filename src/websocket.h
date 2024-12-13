#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <Arduino.h>
#include <WebSocketsServer.h> 

extern String operationMode;

void initWebSocket();
void handleWebSocket();
void sendDebugInfo();
void webSocketEvent(uint8_t clientNum, WStype_t type, uint8_t *payload, size_t length);

#endif // WEBSOCKET_H
