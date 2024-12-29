#include "SystemManager.h"
#include "WebSocket.h"

SystemManager* SystemManager::instance = nullptr;

SystemManager::SystemManager() : isAutomatic(true), brewStatus("Ожидание, ошибок нет"), lastBroadcastTime(0), broadcastInterval(1000) {
    instance = this;
}

void SystemManager::init() {
    // Инициализация насоса
    pump.init(12, 14); // controlPin = 7, pwmPin = 8

    // Инициализация SSR
    ssr.init(9, 10); // controlPin = 9, pwmPin = 10

    // Инициализация сенсоров
    sensors.init();

    // Инициализация WebSocket
    initWebSocket(staticWebSocketHandler);
}


void SystemManager::handleWebSocket() {
    ::handleWebSocket();
}

void SystemManager::staticWebSocketHandler(const String& message) {
    if (instance) {
        instance->handleWebSocketMessage(message);
    }
}

void SystemManager::handleWebSocketMessage(const String& message) {
    StaticJsonDocument<512> doc;
    
    if (deserializeJson(doc, message) == DeserializationError::Ok) {
        // Управление насосом
        if (doc.containsKey("pumpState")) {
            bool pumpEnabled = doc["pumpState"]["enabled"];
            pump.setState(pumpEnabled);

            if (doc["pumpState"].containsKey("pwm")) {
                int pwmLevel = doc["pumpState"]["pwm"];
                pump.setPWM(pwmLevel);
            }
        }

        // Управление SSR
        if (doc.containsKey("ssrState")) {
            bool ssrEnabled = doc["ssrState"]["enabled"];
            ssr.setState(ssrEnabled);

            if (doc["ssrState"].containsKey("pwm")) {
                int pwmLevel = doc["ssrState"]["pwm"];
                ssr.setPWM(pwmLevel);
            }
        }

        // Управление паузами
        if (doc.containsKey("pauses")) {
            JsonArray pausesArray = doc["pauses"].as<JsonArray>();
            setPauseCount(pausesArray.size());
            for (size_t i = 0; i < pausesArray.size(); i++) {
                const JsonObject& pauseObj = pausesArray[i];
                Pause newPause = {
                    pauseObj["temperature"],
                    pauseObj["hysteresis"],
                    pauseObj["time"]
                };
                updatePause(i, newPause);
            }
        }

        // Управление автоматическим режимом
        if (doc.containsKey("isAutomatic")) {
            isAutomatic = doc["isAutomatic"].as<bool>();
        }

        // Обновление статуса варки
        if (doc.containsKey("brewStatus")) {
            brewStatus = doc["brewStatus"].as<String>();
            Serial.println("Brew status updated: " + brewStatus);
        }
    }
}

void SystemManager::handlePeriodicTasks() {
    unsigned long currentTime = millis();

    // Периодическая отправка состояния
    if (currentTime - lastBroadcastTime >= broadcastInterval) {
        lastBroadcastTime = currentTime;

        // Генерация JSON состояния и отправка
        String stateJSON = getStateJSON();
        Serial.println("Broadcasting state: " + stateJSON);

        // Ваша функция для отправки данных WebSocket клиентам
        sendMessageToAll(stateJSON);
    }
}

String SystemManager::getStateJSON() {
    StaticJsonDocument<512> doc;

    // Информация о насосе
    JsonObject pumpState = doc.createNestedObject("pumpState");
    pumpState["enabled"] = pump.isEnabled();
    pumpState["pwm"] = map(pump.getPWMLevel(), 0, 255, 0, 100);

    // Информация о SSR
    JsonObject ssrState = doc.createNestedObject("ssrState");
    ssrState["enabled"] = ssr.isEnabled();
    ssrState["pwm"] = map(ssr.getPWMLevel(), 0, 255, 0, 100);

    // Информация о паузах
    JsonArray pausesArray = doc.createNestedArray("pauses");
    for (int i = 0; i < pauseCount; i++) {
        JsonObject pauseData = pausesArray.createNestedObject();
        pauseData["index"] = i + 1;
        pauseData["temperature"] = pauses[i].temperature;
        pauseData["hysteresis"] = pauses[i].hysteresis;
        pauseData["time"] = pauses[i].time;
    }

    // Информация о сенсорах
    JsonObject sensorsState = doc.createNestedObject("sensors");
    sensors.appendToJSON(sensorsState);

    // Информация об автоматическом режиме
    doc["isAutomatic"] = isAutomatic;

    // Информация о статусе варки
    doc["brewStatus"] = brewStatus;

    // Преобразование в строку JSON
    String jsonString;
    serializeJson(doc, jsonString);

    return jsonString;
}

void SystemManager::setAutomaticMode(bool mode) {
    isAutomatic = mode;
}

bool SystemManager::getAutomaticMode() const {
    return isAutomatic;
}

void SystemManager::setBrewStatus(const String& status) {
    brewStatus = status;
}

String SystemManager::getBrewStatus() const {
    return brewStatus;
}

void SystemManager::setPauseCount(int count) {
    if (count >= 0 && count <= 10) {
        pauseCount = count;
    }
}

void SystemManager::addPause(const Pause& pause) {
    if (pauseCount < 10) {
        pauses[pauseCount] = pause;
        pauseCount++;
    }
}

void SystemManager::removeLastPause() {
    if (pauseCount > 0) {
        pauseCount--;
    }
}

void SystemManager::updatePause(int index, const Pause& pause) {
    if (index >= 0 && index < pauseCount) {
        pauses[index] = pause;
    }
}
