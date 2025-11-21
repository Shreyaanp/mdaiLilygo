/*
 * Screen 9 - Error
 */

#ifndef SCREEN_9_H
#define SCREEN_9_H

#include <lvgl.h>
#include "../utils/DisplayUtils.h"
#include "../utils/TextHelpers.h"

// Declare the custom font
extern const lv_font_t stack_sans_semibold_48;

// Screen 9 objects
static lv_obj_t* screen9_text = NULL;

/**
 * Create Screen 9 - Error
 */
static inline lv_obj_t* screen9_create() {
    // Create screen
    lv_obj_t* screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x8B0000), 0);  // Dark red background

    // Add "Error" text with Stack Sans font
    screen9_text = create_multiline_text(
        screen,
        "✗ Error",
        &stack_sans_semibold_48,
        DISPLAY_CENTER_Y - 40,
        0,
        lv_color_white()
    );

    return screen;
}

/**
 * Update Screen 9 text
 */
static inline void screen9_update_text(const char* text) {
    if (screen9_text) {
        update_text(screen9_text, text);
    }
}

#endif // SCREEN_9_H


