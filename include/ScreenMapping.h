/*
 * Screen Mapping for 2.41" Display
 * Based on State IDs from Serial Protocol:
 * 
 * Screen 0: Idle/Boot
 * Screen 1: Scan Admin QR (State 1)
 * Screen 2: WiFi (State 2)
 * Screen 3: Mdai Ready (State 3)
 * Screen 4: Scan User QR (State 4)
 * Screen 5: Warmup (State 5)
 * Screen 6: Align (State 6)
 * Screen 7: Processing (State 7)
 * Screen 8: Success (State 8)
 * Screen 9: Error (State 9)
 * Screen 10: Nose Tracking
 */

#ifndef SCREEN_MAPPING_H
#define SCREEN_MAPPING_H

// Map State IDs to Screen IDs
#define STATE_SCAN_ADMIN_QR  1
#define STATE_WIFI           2
#define STATE_MDAI_READY     3
#define STATE_SCAN_USER_QR   4
#define STATE_WARMUP         5
#define STATE_ALIGN          6
#define STATE_PROCESSING     7
#define STATE_SUCCESS        8
#define STATE_ERROR          9

// Screen to State mapping
inline ScreenID stateToScreen(uint8_t stateID) {
    if (stateID >= 1 && stateID <= 9) {
        return (ScreenID)stateID;  // Screen 1-9 map directly to states 1-9
    }
    return SCREEN_0;  // Default to boot screen
}

#endif


