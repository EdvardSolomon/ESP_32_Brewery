#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <Arduino.h>
#include <WebSocketsServer.h>

// Тип для колбэка обработки сообщений
typedef void (*WebSocketMessageHandler)(const String& message);

// Интерфейс WebSocket
void initWebSocket(WebSocketMessageHandler handler);
void handleWebSocket();
void sendMessageToAll(const String& message);

#endif // WEBSOCKET_H
