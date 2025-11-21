/*
 * Screen 10 - Thank You / Logout
 */

#ifndef SCREEN_10_H
#define SCREEN_10_H

#include <lvgl.h>
#include "../utils/DisplayUtils.h"
#include "../utils/TextHelpers.h"

// Declare the custom font
extern const lv_font_t stack_sans_semibold_48;

// Screen 10 objects
static lv_obj_t* screen10_text = NULL;

/**
 * Create Screen 10 - Thank You
 */
static inline lv_obj_t* screen10_create() {
    // Create screen
    lv_obj_t* screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);  // Black background

    // Add "Thank You" text with Stack Sans font
    screen10_text = create_multiline_text(
        screen,
        "Thank You",
        &stack_sans_semibold_48,
        DISPLAY_CENTER_Y - 40,
        0,
        lv_color_white()
    );

    return screen;
}

/**
 * Update Screen 10 text
 */
static inline void screen10_update_text(const char* text) {
    if (screen10_text) {
        update_text(screen10_text, text);
    }
}

#endif // SCREEN_10_H


