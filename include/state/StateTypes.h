/*
 * State Types - Define all application state structures
 */

#ifndef STATE_TYPES_H
#define STATE_TYPES_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <stdint.h>
#include <stdbool.h>
typedef struct { const char* data; int len; } String; // Dummy for simulator
#endif

// Screen identifiers
enum ScreenID {
    SCREEN_0 = 0,  // Test screen (first screen on startup)
    SCREEN_1 = 1,
    SCREEN_2 = 2,
    SCREEN_3 = 3,
    SCREEN_4 = 4,
    SCREEN_5 = 5,
    SCREEN_6 = 6,
    SCREEN_7 = 7,
    SCREEN_8 = 8,
    SCREEN_9 = 9,
    SCREEN_10 = 10
};

// Application state
struct AppStateData {
    ScreenID currentScreen;
    ScreenID previousScreen;
    
    bool serialConnected;
    unsigned long lastSerialActivity;
    
    float batteryLevel;
    bool isCharging;
    
    // Nose tracking / target position (for Screen 7)
    int16_t targetX;
    int16_t targetY;
    bool trackingActive;
    unsigned long lastTrackingUpdate;
    
    // Add your custom data fields
    String customData1;
    float customData2;
    int customData3;
};

#endif

