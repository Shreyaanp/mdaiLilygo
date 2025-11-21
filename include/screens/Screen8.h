/*
 * Screen 8 - Success
 */

#ifndef SCREEN_8_H
#define SCREEN_8_H

#include <lvgl.h>
#include "../utils/DisplayUtils.h"
#include "../utils/TextHelpers.h"

// Declare the custom font
extern const lv_font_t stack_sans_semibold_48;

// Screen 8 objects
static lv_obj_t* screen8_text = NULL;

/**
 * Create Screen 8 - Success
 */
static inline lv_obj_t* screen8_create() {
    // Create screen
    lv_obj_t* screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x006400), 0);  // Dark green background

    // Add "Success" text with Stack Sans font
    screen8_text = create_multiline_text(
        screen,
        "✓ Success",
        &stack_sans_semibold_48,
        DISPLAY_CENTER_Y - 40,
        0,
        lv_color_white()
    );

    return screen;
}

/**
 * Update Screen 8 text
 */
static inline void screen8_update_text(const char* text) {
    if (screen8_text) {
        update_text(screen8_text, text);
    }
}

#endif // SCREEN_8_H


