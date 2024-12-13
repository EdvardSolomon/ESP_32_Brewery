#include <WiFi.h>
#include <WebSocketsServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Wi-Fi credentials
const char* ssid = "agent_007 2G";
const char* password = "24031996";

// DS18B20
#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1, sensor2;

// GPIO for pump and SSR control
#define PUMP_PIN 16
#define SSR_PIN 19

// WebSocket server
WebSocketsServer webSocket(81);

// Pump control variables
int pumpPWMLevel = 0; // PWM level (0-100)
bool pumpEnabled = false; // Pump state (ON/OFF)

// SSR control variables
int ssrPWMLevel = 0; // SSR PWM level (0-100)
bool ssrEnabled = false; // SSR state (ON/OFF)

// Function to get DS18B20 address as a string
String getAddressString(DeviceAddress address) {
    String addr = "";
    for (uint8_t i = 0; i < 8; i++) {
        if (address[i] < 16) addr += "0";
        addr += String(address[i], HEX);
    }
    return addr;
}

// Update PWM output for the pump
void updatePumpPWM() {
    if (pumpEnabled) {
        int dutyCycle = map(pumpPWMLevel, 0, 100, 0, 255); // Map 0-100% to 0-255
        ledcWrite(0, dutyCycle); // Set PWM level
    } else {
        ledcWrite(0, 0); // Turn pump off (PWM = 0)
    }
}

void updateSSRPWM() {
    if (ssrEnabled) {
        int dutyCycle = map(ssrPWMLevel, 0, 100, 0, 255); // Map 0-100% to 0-255
        ledcWrite(1, dutyCycle); // Set SSR output
    } else {
        ledcWrite(1, 0); // Turn SSR off (PWM = 0)
    }
}


// Send debug and sensor data via WebSocket
void sendDebugInfo() {
    String debugInfo = "{";

    // Check and send sensor 1 address
    if (sensors.getAddress(sensor1, 0)) {
        debugInfo += "\"sensor1_address\": \"" + getAddressString(sensor1) + "\",";
    } else {
        debugInfo += "\"sensor1_address\": \"not_found\",";
    }

    // Check and send sensor 2 address
    if (sensors.getAddress(sensor2, 1)) {
        debugInfo += "\"sensor2_address\": \"" + getAddressString(sensor2) + "\",";
    } else {
        debugInfo += "\"sensor2_address\": \"not_found\",";
    }

    // Read temperatures
    sensors.requestTemperatures();
    float temp1 = sensors.getTempC(sensor1);
    float temp2 = sensors.getTempC(sensor2);

    debugInfo += "\"sensor1_temp\": " + String(temp1) + ",";
    debugInfo += "\"sensor2_temp\": " + String(temp2) + ",";

    // Add pump state and PWM level
    debugInfo += "\"pump_enabled\": " + String(pumpEnabled) + ",";
    debugInfo += "\"pump_pwm\": " + String(pumpPWMLevel) + ",";

    // Add SSR state and PWM level
    debugInfo += "\"ssr_enabled\": " + String(ssrEnabled) + ",";
    debugInfo += "\"ssr_pwm\": " + String(ssrPWMLevel) + "}";

    // Broadcast data to all clients
    webSocket.broadcastTXT(debugInfo);
}

// Handle WebSocket events
void webSocketEvent(uint8_t clientNum, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_CONNECTED) {
        // Send initial debug info when a client connects
        sendDebugInfo();
    } else if (type == WStype_TEXT) {
        // Handle incoming messages
        String message = String((char *)payload);

        if (message == "TOGGLE_PUMP") {
            pumpEnabled = !pumpEnabled;
            updatePumpPWM();
        } else if (message.startsWith("SET_PWM:")) {
            int newPWM = message.substring(8).toInt();
            if (newPWM >= 0 && newPWM <= 100) {
                pumpPWMLevel = newPWM;
                updatePumpPWM();
            }
        } else if (message == "TOGGLE_SSR") {
            ssrEnabled = !ssrEnabled;
            updateSSRPWM();
        } else if (message.startsWith("SET_SSR_PWM:")) {
            int newSSRPWM = message.substring(12).toInt();
            if (newSSRPWM >= 0 && newSSRPWM <= 100) {
                ssrPWMLevel = newSSRPWM;
                updateSSRPWM();
            }
        }

        // Send updated state back to all clients
        sendDebugInfo();
    }
}

void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }

    // Initialize DS18B20 sensors
    sensors.begin();

    // Setup PWM for pump and SSR
    ledcSetup(0, 5000, 8); // Channel 0, 5kHz, 8-bit resolution for pump
    ledcAttachPin(PUMP_PIN, 0);
    updatePumpPWM();

    ledcSetup(1, 5000, 8); // Channel 1, 5kHz, 8-bit resolution for SSR
    ledcAttachPin(SSR_PIN, 1);
    updateSSRPWM();

    // Start WebSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();

    // Send debug info every 5 seconds
    static unsigned long lastSendTime = 0;
    if (millis() - lastSendTime > 1000) {
        lastSendTime = millis();
        sendDebugInfo();
    }
}
