/*
 * AppState - Global state manager
 */

#ifndef APP_STATE_H
#define APP_STATE_H

#include "StateTypes.h"
#include "ArduinoCompat.h"

class AppState {
private:
    static AppState* instance;
    AppStateData state;
    void (*screenChangeCallback)(ScreenID newScreen) = nullptr;
    
    AppState() {
        state.currentScreen = SCREEN_0;  // Start with test screen
        state.previousScreen = SCREEN_0;
        state.serialConnected = false;
        state.lastSerialActivity = 0;
        state.batteryLevel = 0.0f;
        state.isCharging = false;
        state.targetX = 233;  // Center X
        state.targetY = 233;  // Center Y
        state.trackingActive = false;
        state.lastTrackingUpdate = 0;
    }
    
public:
    static AppState* getInstance() {
        if (!instance) {
            instance = new AppState();
        }
        return instance;
    }
    
    AppStateData* getState() { return &state; }
    
    void setScreenChangeCallback(void (*callback)(ScreenID)) {
        screenChangeCallback = callback;
    }
    
    void changeScreen(ScreenID newScreen) {
        state.previousScreen = state.currentScreen;
        state.currentScreen = newScreen;
        Serial.printf("Screen changed: %d -> %d\n", state.previousScreen, state.currentScreen);
        
        // Call the callback to actually load the screen
        if (screenChangeCallback) {
            screenChangeCallback(newScreen);
        }
    }
    
    ScreenID getCurrentScreen() { return state.currentScreen; }
    
    void updateTargetPosition(int16_t x, int16_t y) {
        state.targetX = x;
        state.targetY = y;
        state.trackingActive = true;
        state.lastTrackingUpdate = millis();
    }
    
    int16_t getTargetX() { return state.targetX; }
    int16_t getTargetY() { return state.targetY; }
    bool isTrackingActive() { 
        // Consider tracking inactive if no update for 2 seconds
        return state.trackingActive && (millis() - state.lastTrackingUpdate < 2000);
    }
    
    void updateFromSerial(String data) {
        // Parse and update state from serial data
#ifdef ARDUINO
        Serial.println("State updated from serial: " + data);
#else
        Serial.println("State updated from serial");
#endif
    }
};

AppState* AppState::instance = nullptr;

#endif

