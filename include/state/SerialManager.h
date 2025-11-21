/*
 * SerialManager - Handle serial communication
 * Format: JSON messages like {"screen": 2} or {"data": "value"}
 */

#ifndef SERIAL_MANAGER_H
#define SERIAL_MANAGER_H

#include <Arduino.h>
#include "AppState.h"

class SerialManager {
private:
    String rxBuffer;
    AppState* appState;
    
    /**
     * Simple JSON parser for {"screen": N} format
     * @param json JSON string
     * @param key Key to find
     * @return Value as string, empty if not found
     */
    String parseJsonValue(String json, String key) {
        int keyIndex = json.indexOf("\"" + key + "\"");
        if (keyIndex == -1) return "";
        
        int colonIndex = json.indexOf(":", keyIndex);
        if (colonIndex == -1) return "";
        
        int valueStart = colonIndex + 1;
        while (valueStart < json.length() && (json[valueStart] == ' ' || json[valueStart] == '\t')) {
            valueStart++;
        }
        
        if (valueStart >= json.length()) return "";
        
        bool isString = json[valueStart] == '"';
        if (isString) valueStart++;
        
        int valueEnd = valueStart;
        if (isString) {
            valueEnd = json.indexOf('"', valueStart);
        } else {
            while (valueEnd < json.length() && 
                   json[valueEnd] != ',' && 
                   json[valueEnd] != '}' && 
                   json[valueEnd] != ' ') {
                valueEnd++;
            }
        }
        
        if (valueEnd == -1) valueEnd = json.length();
        
        return json.substring(valueStart, valueEnd);
    }
    
public:
    SerialManager() {
        appState = AppState::getInstance();
        rxBuffer = "";
    }
    
    void begin(unsigned long baud = 115200) {
        Serial.begin(baud);
        delay(100);
        Serial.println("SerialManager ready. Send JSON: {\"screen\": 1}");
    }
    
    void update() {
        while (Serial.available() > 0) {
            char c = Serial.read();
            
            if (c == '\n' || c == '\r') {
                if (rxBuffer.length() > 0) {
                    handleMessage(rxBuffer);
                    rxBuffer = "";
                }
            } else {
                rxBuffer += c;
                if (rxBuffer.length() > 512) {
                    Serial.println("ERR: Buffer overflow");
                    rxBuffer = "";
                }
            }
        }
    }
    
    void handleMessage(String msg) {
        msg.trim();
        
        // Simple number format: just "0" to "10"
        if (msg.length() <= 2 && msg.toInt() >= 0 && msg.toInt() <= 10) {
            int screenId = msg.toInt();
            appState->changeScreen((ScreenID)screenId);
            Serial.printf("OK: Switched to Screen %d\n", screenId);
            return;
        }
        
        // Parse nose tracker format: "X:233,Y:180"
        if (msg.indexOf("X:") != -1 && msg.indexOf("Y:") != -1) {
            int xStart = msg.indexOf("X:") + 2;
            int comma = msg.indexOf(",", xStart);
            int yStart = msg.indexOf("Y:", comma) + 2;
            
            if (comma != -1 && yStart > 2) {
                String xStr = msg.substring(xStart, comma);
                String yStr = msg.substring(yStart);
                
                int16_t x = xStr.toInt();
                int16_t y = yStr.toInt();
                
                // Validate bounds (466x466 display)
                if (x >= 0 && x < 466 && y >= 0 && y < 466) {
                    appState->updateTargetPosition(x, y);
                    // Don't spam serial with tracking updates
                    return;
                }
            }
        }
        
        // If not tracking data, print received message
        Serial.println("RX: " + msg);
        
        // Parse JSON format: {"screen": 2}
        if (msg.indexOf("screen") != -1) {
            String screenValue = parseJsonValue(msg, "screen");
            if (screenValue.length() > 0) {
                int screenId = screenValue.toInt();
                if (screenId >= 1 && screenId <= 10) {
                    appState->changeScreen((ScreenID)(screenId - 1));  // Convert 1-based to 0-based
                    Serial.println("OK: Screen changed to " + String(screenId));
                } else {
                    Serial.println("ERR: Screen must be 1-10");
                }
            }
        }
        
        // Parse other data fields
        if (msg.indexOf("data") != -1) {
            String dataValue = parseJsonValue(msg, "data");
            if (dataValue.length() > 0) {
                appState->updateFromSerial(dataValue);
                Serial.println("OK: Data updated");
            }
        }
        
        // Example: {"temp": 25, "humidity": 60}
        if (msg.indexOf("temp") != -1) {
            String tempValue = parseJsonValue(msg, "temp");
            // Handle temperature update
            Serial.println("OK: Temp = " + tempValue);
        }
    }
    
    void send(String msg) {
        Serial.println("TX: " + msg);
    }
    
    void sendJson(String key, String value) {
        Serial.println("{\"" + key + "\": \"" + value + "\"}");
    }
};

#endif

