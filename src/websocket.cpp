#include "websocket.h"
#include "pauses.h"
#include "sensors.h"
#include "control.h"
#include <WebSocketsServer.h>

extern Pause pauses[MAX_PAUSES]; // Массив пауз
extern int pauseCount;  

WebSocketsServer webSocket(81);

String operationMode = "manual";

void initWebSocket() {
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void handleWebSocket() {
    webSocket.loop();
}

// Обработка событий WebSocket
void webSocketEvent(uint8_t clientNum, WStype_t type, uint8_t *payload, size_t length) {
    if (type == WStype_TEXT) {
        String message = String((char *)payload);


        // Обработка других команд...
        sendDebugInfo();
    }
}

void sendDebugInfo() {
    String debugInfo = "{";
    debugInfo += "\"sensor1_address\": \"" + getSensorAddress(0) + "\",";
    debugInfo += "\"sensor2_address\": \"" + getSensorAddress(1) + "\",";
    debugInfo += "\"sensor1_temp\": " + String(getTemperature(0)) + ",";
    debugInfo += "\"sensor2_temp\": " + String(getTemperature(1)) + ",";
    debugInfo += "\"pump_enabled\": " + String(pumpEnabled) + ",";
    debugInfo += "\"pump_pwm\": " + String(pumpPWMLevel) + ",";
    debugInfo += "\"ssr_enabled\": " + String(ssrEnabled) + ",";
    debugInfo += "\"ssr_pwm\": " + String(ssrPWMLevel) + "}";
    debugInfo += "\"operationMode\": " + String(operationMode) + "}";
      debugInfo += "\"pause_count\": " + String(pauseCount) + ",";
    debugInfo += "\"pauses\": [";

    for (int i = 0; i < pauseCount; i++) {
        debugInfo += "{";
        debugInfo += "\"index\": " + String(i + 1) + ",";
        debugInfo += "\"temperature\": " + String(pauses[i].temperature, 1) + ",";
        debugInfo += "\"hysteresis\": " + String(pauses[i].hysteresis, 1) + ",";
        debugInfo += "\"time\": " + String(pauses[i].time);
        debugInfo += "}";
        if (i < pauseCount - 1) debugInfo += ",";
    }

    debugInfo += "]";
    debugInfo += "}";
    webSocket.broadcastTXT(debugInfo);
}

void webSocketEvent(uint8_t clientNum, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_CONNECTED) {
        sendDebugInfo();
    } else if (type == WStype_TEXT) {
        String message = String((char *)payload);
         if (message.startsWith("SET_PAUSE_COUNT:")) {
            int newPauseCount = message.substring(16).toInt();
            if (newPauseCount >= 1 && newPauseCount <= MAX_PAUSES) {
                pauseCount = newPauseCount;
            }
        }
        else if (message.startsWith("UPDATE_PAUSE:")) {
            int index, time;
            float temperature, hysteresis;
            
            // Пример команды: UPDATE_PAUSE:2:50.5:3.0:15
            sscanf(message.c_str(), "UPDATE_PAUSE:%d:%f:%f:%d", &index, &temperature, &hysteresis, &time);
            if (index > 0 && index <= pauseCount) {
                pauses[index - 1].temperature = temperature;
                pauses[index - 1].hysteresis = hysteresis;
                pauses[index - 1].time = time;
            }
        }
        
        if (message.startsWith("SET_MODE:")) {
            String newMode = message.substring(9);
            if (newMode == "automatic" || newMode == "manual") {
                operationMode = newMode;
            }
        }
        if (message == "TOGGLE_PUMP") togglePump();
        else if (message.startsWith("SET_PWM:")) setPumpPWMLevel(message.substring(8).toInt());
        else if (message == "TOGGLE_SSR") toggleSSR();
        else if (message.startsWith("SET_SSR_PWM:")) setSSRPWMLevel(message.substring(12).toInt());
        sendDebugInfo();
    }
}
