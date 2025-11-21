/*
 * Screen 12 - WiFi Connection Success
 */

#ifndef SCREEN_12_H
#define SCREEN_12_H

#include <lvgl.h>
#include "../utils/DisplayUtils.h"
#include "../utils/TextHelpers.h"

// Declare the custom font
extern const lv_font_t stack_sans_semibold_48;

// Screen 12 objects
static lv_obj_t* screen12_text = NULL;

/**
 * Create Screen 12 - WiFi Success
 */
static inline lv_obj_t* screen12_create() {
    // Create screen
    lv_obj_t* screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x006400), 0);  // Dark green background

    // Add "WiFi Connected" text
    screen12_text = create_multiline_text(
        screen,
        "✓ WiFi Connected",
        &stack_sans_semibold_48,
        DISPLAY_CENTER_Y - 40,
        0,
        lv_color_white()
    );

    return screen;
}

/**
 * Update Screen 12 text
 */
static inline void screen12_update_text(const char* text) {
    if (screen12_text) {
        update_text(screen12_text, text);
    }
}

#endif // SCREEN_12_H

