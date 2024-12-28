#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <Arduino.h>
#include <WebSocketsServer.h>

typedef void (*WebSocketMessageHandler)(const String& message);

void initWebSocket(WebSocketMessageHandler handler);
void handleWebSocket();
void sendMessageToAll(const String& message);

#endif // WEBSOCKET_H
