/*
 * Screen 11 - Connecting to New WiFi
 */

#ifndef SCREEN_11_H
#define SCREEN_11_H

#include "lvgl.h"
#include "../components/CircularLoader.h"
#include "../utils/TextHelpers.h"
#include "../utils/DisplayUtils.h"

// Declare the custom font (defined in main.h)
extern const lv_font_t stack_sans_semibold_48;

// Screen 11 objects
static lv_obj_t* screen11_loader = NULL;
static lv_obj_t* screen11_text = NULL;

lv_obj_t* screen11_create() {
    // Create screen
    lv_obj_t* screen = lv_obj_create(NULL);
    
    // Set black background
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);
    
    // Create circular loader
    circular_loader_config_t loader_config = circular_loader_default_config();
    loader_config.thickness = 30;
    loader_config.arc_length = 180;
    loader_config.color = lv_color_hex(0xFFFFFF);
    loader_config.animation_time = 20000;  // 20 seconds
    loader_config.rotate_continuously = true;
    
    screen11_loader = circular_loader_create(screen, loader_config);
    
    // Ensure loader is on top
    lv_obj_move_foreground(screen11_loader);
    
    // Start rotation animation
    circular_loader_start_rotation(screen11_loader, loader_config.animation_time, loader_config.rotate_continuously);
    
    // Add "Connecting to New WiFi..." text
    screen11_text = create_multiline_text(
        screen,
        "Connecting to New WiFi...",
        &stack_sans_semibold_48,
        DISPLAY_CENTER_Y - 40,
        0,
        lv_color_white()
    );
    
    return screen;
}

/**
 * Update Screen 11 text
 */
void screen11_update_text(const char* text) {
    if (screen11_text) {
        update_text(screen11_text, text);
    }
}

#endif // SCREEN_11_H

