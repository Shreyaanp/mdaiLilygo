/*
 * Main Controller - Orchestrates UI initialization and updates
 * This file is used by BOTH the device and simulator
 */

#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include <lvgl.h>
#include "utils/SmartTransition.h"

// Include fonts (only once here)
#include "fonts/stack_sans_semibold_48.c"

// Include components (only once here)
#include "components/logo.c"
#include "components/frame_3965.c"


// Include all screens (1.43" Circular - 466x466)
#include "screens/Screen0.h"
#include "screens/Screen1.h"
#include "screens/Screen2.h"
#include "screens/Screen3.h"
#include "screens/Screen4.h"
#include "screens/Screen5.h"
#include "screens/Screen6.h"
#include "screens/Screen7.h"
#include "screens/Screen8.h"
#include "screens/Screen9.h"
#include "screens/Screen10.h"
#include "screens/Screen11.h"
#include "screens/Screen12.h"
#include "screens/Screen13.h"

// Screen storage (14 screens: 0-13)
static lv_obj_t* all_screens[14] = {NULL};
static int current_screen_index = 0;

/**
 * Initialize UI - Create all screens
 * Call this once at startup
 */
static inline void init_ui() {
    // Create Screen 0 (Test screen)
    all_screens[0] = screen0_create();
    
    // Create Screen 1
    all_screens[1] = screen1_create();
    
    // Create Screen 2
    all_screens[2] = screen2_create();
    
    // Create Screen 3
    all_screens[3] = screen3_create();

    // Create Screen 4
    all_screens[4] = screen4_create();

    // Create Screen 5
    all_screens[5] = screen5_create();

    // Create Screen 6
    all_screens[6] = screen6_create();

    // Create Screen 7
    all_screens[7] = screen7_create();

    // Create Screen 8
    all_screens[8] = screen8_create();

    // Create Screen 9
    all_screens[9] = screen9_create();

    // Create Screen 10
    all_screens[10] = screen10_create();
    
    // Create Screen 11 (WiFi Change - Connecting)
    all_screens[11] = screen11_create();
    
    // Create Screen 12 (WiFi Change - Success)
    all_screens[12] = screen12_create();
    
    // Create Screen 13 (WiFi Change - Failed)
    all_screens[13] = screen13_create();
    
    // Load first screen (Screen 1 - Logo, skip test screen 0)
    if (all_screens[1] != NULL) {
#if LV_VERSION_CHECK(9, 0, 0)
        lv_screen_load(all_screens[1]);
#else
        lv_scr_load(all_screens[1]);
#endif
        current_screen_index = 1;
    }
}

/**
 * Switch to a specific screen with black overlay fade
 * Perfect for OLED - completely hides color rendering during transition!
 * @param screen_index Screen index (0-13)
 * @param animate If true, uses smooth black fade (400ms total)
 */
static inline void switch_to_screen(int screen_index, bool animate) {
    if (screen_index < 0 || screen_index >= 14) {  // 14 screens now (0-13)
        return;  // Invalid screen
    }
    
    if (all_screens[screen_index] == NULL) {
        return;  // Screen not created yet
    }
    
    current_screen_index = screen_index;
    
    if (animate) {
        // Black overlay fade: to black (200ms) → switch → from black (200ms)
        smooth_black_fade_transition(all_screens[screen_index]);
    } else {
        // Instant load
        #if LV_VERSION_CHECK(9, 0, 0)
        lv_screen_load(all_screens[screen_index]);
        #else
        lv_scr_load(all_screens[screen_index]);
        #endif
    }
}

/**
 * Update UI based on state changes
 * Call this in your main loop when state changes
 */
static inline void update_ui() {
    // Update current screen based on state
    // Example: Update Screen 1 status
    // screen1_update("Active");
    
    // Example: Update Screen 2 progress
    // screen2_update_progress(50);
    
    // Add your update logic here based on your AppState
}

/**
 * Get current screen index
 */
static inline int get_current_screen() {
    return current_screen_index;
}

#endif // MAIN_CONTROLLER_H

