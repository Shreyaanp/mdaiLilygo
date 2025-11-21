/*
 * FONT USAGE EXAMPLE
 * 
 * This file shows how to use the generated Stack Sans fonts
 * in your UI Application screens.
 */

#ifndef FONT_USAGE_EXAMPLE_H
#define FONT_USAGE_EXAMPLE_H

#include "lvgl.h"

// Example: Using different weights and sizes

// 1. Include the fonts you need
#include "stack_sans_regular_24.c"
#include "stack_sans_bold_32.c"
#include "stack_sans_light_16.c"
#include "stack_sans_medium_20.c"

// 2. Declare them
LV_FONT_DECLARE(stack_sans_regular_24);
LV_FONT_DECLARE(stack_sans_bold_32);
LV_FONT_DECLARE(stack_sans_light_16);
LV_FONT_DECLARE(stack_sans_medium_20);

// 3. Usage example
static inline void font_example_screen(lv_obj_t* parent) {
    // Title - Bold, Large
    lv_obj_t* title = lv_label_create(parent);
    lv_label_set_text(title, "Welcome");
    lv_obj_set_style_text_font(title, &stack_sans_bold_32, 0);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);
    
    // Subtitle - Regular, Medium
    lv_obj_t* subtitle = lv_label_create(parent);
    lv_label_set_text(subtitle, "Your Application");
    lv_obj_set_style_text_font(subtitle, &stack_sans_regular_24, 0);
    lv_obj_set_style_text_color(subtitle, lv_color_make(200, 200, 200), 0);
    lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, 60);
    
    // Body text - Medium, Smaller
    lv_obj_t* body = lv_label_create(parent);
    lv_label_set_text(body, "This is some body text\nusing Medium weight");
    lv_obj_set_style_text_font(body, &stack_sans_medium_20, 0);
    lv_obj_align(body, LV_ALIGN_CENTER, 0, 0);
    
    // Caption - Light, Small
    lv_obj_t* caption = lv_label_create(parent);
    lv_label_set_text(caption, "Small caption text");
    lv_obj_set_style_text_font(caption, &stack_sans_light_16, 0);
    lv_obj_set_style_text_color(caption, lv_color_make(150, 150, 150), 0);
    lv_obj_align(caption, LV_ALIGN_BOTTOM_MID, 0, -20);
}

/*
 * AVAILABLE FONTS:
 * 
 * Weights: extralight, light, regular, medium, semibold, bold
 * Sizes: 12, 14, 16, 18, 20, 24, 28, 32, 40, 48, 64, 67
 * 
 * Naming pattern: stack_sans_<weight>_<size>
 * 
 * Examples:
 * - stack_sans_extralight_12
 * - stack_sans_light_16
 * - stack_sans_regular_24
 * - stack_sans_medium_28
 * - stack_sans_semibold_32
 * - stack_sans_bold_48
 * - stack_sans_bold_67
 */

#endif // FONT_USAGE_EXAMPLE_H
