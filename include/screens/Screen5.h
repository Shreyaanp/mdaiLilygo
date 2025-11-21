/*
 * Screen 5 - Frame 3965 image
 */

#ifndef SCREEN_5_H
#define SCREEN_5_H

#include <lvgl.h>
#include "../utils/DisplayUtils.h"

// Declare frame_3965 (defined in main.h)
extern const lv_img_dsc_t frame_3965;

// Screen 5 objects
static lv_obj_t* screen5 = NULL;
static lv_obj_t* screen5_frame_img = NULL;

/**
 * Create Screen 5 - Frame 3965
 */
static inline lv_obj_t* screen5_create() {
    // Create screen
    screen5 = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen5, lv_color_hex(0x000000), 0);  // Black background

    // Clear any default padding/border
    lv_obj_set_style_pad_all(screen5, 0, 0);
    lv_obj_set_style_border_width(screen5, 0, 0);

    // Add frame_3965 image (466x466px) centered
    screen5_frame_img = lv_img_create(screen5);
    lv_img_set_src(screen5_frame_img, &frame_3965);
    lv_obj_align(screen5_frame_img, LV_ALIGN_CENTER, 0, 0);
    
    // Ensure image is visible and on top
    lv_obj_clear_flag(screen5_frame_img, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(screen5_frame_img);

    return screen5;
}

#endif // SCREEN_5_H

