#include "websocket.h"

WebSocketsServer webSocket(8080);
WebSocketMessageHandler messageHandler = nullptr;

void initWebSocket(WebSocketMessageHandler handler) {
    messageHandler = handler;

    webSocket.begin();
    webSocket.onEvent([](uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
        if (type == WStype_TEXT) {
            String message = String((char*)payload);
            if (messageHandler) {
                messageHandler(message);
            }
        }
    });

    Serial.println("WebSocket server initialized");
}

void handleWebSocket() {
    webSocket.loop();
}

void sendMessageToAll(const String& message) {
    webSocket.broadcastTXT(message);
}
