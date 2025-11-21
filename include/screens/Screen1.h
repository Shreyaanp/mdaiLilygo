/*
 * Screen 1 - Logo display
 */

#ifndef SCREEN_1_H
#define SCREEN_1_H

#include <lvgl.h>
#include "../utils/DisplayUtils.h"

// Declare the logo (defined in main.h)
extern const lv_img_dsc_t logo;

// Screen 1 objects
static lv_obj_t* screen1 = NULL;
static lv_obj_t* screen1_logo_img = NULL;

/**
 * Create Screen 1
 */
static inline lv_obj_t* screen1_create() {
    // Create screen
    screen1 = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen1, lv_color_hex(0x000000), 0);
    
    // Display logo in center
    screen1_logo_img = lv_img_create(screen1);
    lv_img_set_src(screen1_logo_img, &logo);
    lv_obj_align(screen1_logo_img, LV_ALIGN_CENTER, 0, 0);
    
    return screen1;
}

#endif // SCREEN_1_H

