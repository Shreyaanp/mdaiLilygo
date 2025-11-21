/*
 * Screen 13 - WiFi Connection Failed
 */

#ifndef SCREEN_13_H
#define SCREEN_13_H

#include <lvgl.h>
#include "../utils/DisplayUtils.h"
#include "../utils/TextHelpers.h"

// Declare the custom font
extern const lv_font_t stack_sans_semibold_48;

// Screen 13 objects
static lv_obj_t* screen13_text = NULL;

/**
 * Create Screen 13 - WiFi Failed
 */
static inline lv_obj_t* screen13_create() {
    // Create screen
    lv_obj_t* screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x8B0000), 0);  // Dark red background

    // Add "WiFi Failed - Reverting" text
    screen13_text = create_multiline_text(
        screen,
        "✗ WiFi Failed\nReverting...",
        &stack_sans_semibold_48,
        DISPLAY_CENTER_Y - 40,
        0,
        lv_color_white()
    );

    return screen;
}

/**
 * Update Screen 13 text
 */
static inline void screen13_update_text(const char* text) {
    if (screen13_text) {
        update_text(screen13_text, text);
    }
}

#endif // SCREEN_13_H

