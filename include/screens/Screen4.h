/*
 * Screen 4 - mDai Ready
 */

#ifndef SCREEN_4_H
#define SCREEN_4_H

#include "lvgl.h"
#include "../components/TextScreen.h"

// Screen 4 objects
static text_screen_t* screen4 = NULL;

/**
 * Create Screen 4 - mDai Ready
 */
static inline lv_obj_t* screen4_create() {
    text_screen_config_t config = text_screen_default_config();
    config.text = "mDai Ready";
    config.bg_color = lv_color_hex(0x000000);  // Black background
    config.text_color = lv_color_white();
    
    screen4 = text_screen_create(&config);
    
    return screen4->screen;
}

/**
 * Update Screen 4 text
 */
static inline void screen4_update_text(const char* text) {
    text_screen_update(screen4, text);
}

#endif // SCREEN_4_H
