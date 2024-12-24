#include "websocket.h"

WebSocketsServer webSocket(80);
WebSocketMessageHandler messageHandler = nullptr;

void initWebSocket(WebSocketMessageHandler handler) {
    messageHandler = handler; // Устанавливаем колбэк
    webSocket.begin();
    webSocket.onEvent([](uint8_t clientNum, WStype_t type, uint8_t* payload, size_t length) {
        if (type == WStype_TEXT && messageHandler) {
            String message = String((char*)payload);
            messageHandler(message); // Вызываем обработчик
        }
    });
    Serial.println("WebSocket сервер запущен");
}

void handleWebSocket() {
    webSocket.loop();
}

void sendMessageToAll(const String& message) {
    webSocket.broadcastTXT(message.c_str());
}
