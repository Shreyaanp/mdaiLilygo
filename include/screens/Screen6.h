/*
 * Screen 6 - Ready?
 */

#ifndef SCREEN_6_H
#define SCREEN_6_H

#include "lvgl.h"
#include "../components/TextScreen.h"

// Screen 6 objects
static text_screen_t* screen6 = NULL;

/**
 * Create Screen 6 - Ready?
 */
static inline lv_obj_t* screen6_create() {
    text_screen_config_t config = text_screen_default_config();
    config.text = "Ready?";
    config.bg_color = lv_color_hex(0x000000);  // Black background
    config.text_color = lv_color_white();
    
    screen6 = text_screen_create(&config);
    
    return screen6->screen;
}

/**
 * Update Screen 6 text
 */
static inline void screen6_update_text(const char* text) {
    text_screen_update(screen6, text);
}

#endif // SCREEN_6_H
