/*
 * Font Demo Screen
 * 
 * Demonstrates using Stack Sans fonts with different weights and sizes
 * Shows visual hierarchy and typography best practices
 */

#ifndef FONT_DEMO_H
#define FONT_DEMO_H

#include "lvgl.h"

// Include different fonts - only include what you actually use!
#include "../fonts/stack_sans_extralight_14.c"
#include "../fonts/stack_sans_light_16.c"
#include "../fonts/stack_sans_regular_20.c"
#include "../fonts/stack_sans_medium_24.c"
#include "../fonts/stack_sans_semibold_28.c"
#include "../fonts/stack_sans_bold_40.c"
#include "../fonts/stack_sans_bold_67.c"

// Declare fonts
LV_FONT_DECLARE(stack_sans_extralight_14);
LV_FONT_DECLARE(stack_sans_light_16);
LV_FONT_DECLARE(stack_sans_regular_20);
LV_FONT_DECLARE(stack_sans_medium_24);
LV_FONT_DECLARE(stack_sans_semibold_28);
LV_FONT_DECLARE(stack_sans_bold_40);
LV_FONT_DECLARE(stack_sans_bold_67);

/**
 * Create a Font Demo screen showing typography hierarchy
 */
static inline lv_obj_t* font_demo_create(void) {
    // Create screen
    lv_obj_t* screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    
    // Create scrollable container
    lv_obj_t* container = lv_obj_create(screen);
    lv_obj_set_size(container, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_color(container, lv_color_black(), 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_AUTO);
    
    int16_t y_pos = 20;
    
    // Hero Text - Bold 67 (Largest, for main display numbers/time)
    lv_obj_t* hero = lv_label_create(container);
    lv_label_set_text(hero, "12:34");
    lv_obj_set_style_text_font(hero, &stack_sans_bold_67, 0);
    lv_obj_set_style_text_color(hero, lv_color_white(), 0);
    lv_obj_set_pos(hero, 10, y_pos);
    y_pos += 80;
    
    // Title - Bold 40 (Page titles, main headings)
    lv_obj_t* title = lv_label_create(container);
    lv_label_set_text(title, "Welcome");
    lv_obj_set_style_text_font(title, &stack_sans_bold_40, 0);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_pos(title, 10, y_pos);
    y_pos += 50;
    
    // Subtitle - SemiBold 28 (Section headers)
    lv_obj_t* subtitle = lv_label_create(container);
    lv_label_set_text(subtitle, "Typography Demo");
    lv_obj_set_style_text_font(subtitle, &stack_sans_semibold_28, 0);
    lv_obj_set_style_text_color(subtitle, lv_color_make(200, 200, 200), 0);
    lv_obj_set_pos(subtitle, 10, y_pos);
    y_pos += 40;
    
    // Divider line
    lv_obj_t* line1 = lv_obj_create(container);
    lv_obj_set_size(line1, LV_HOR_RES - 40, 1);
    lv_obj_set_style_bg_color(line1, lv_color_make(80, 80, 80), 0);
    lv_obj_set_style_border_width(line1, 0, 0);
    lv_obj_set_pos(line1, 20, y_pos);
    y_pos += 20;
    
    // Body Emphasis - Medium 24 (Important body text)
    lv_obj_t* body_emphasis = lv_label_create(container);
    lv_label_set_text(body_emphasis, "Battery: 87%");
    lv_obj_set_style_text_font(body_emphasis, &stack_sans_medium_24, 0);
    lv_obj_set_style_text_color(body_emphasis, lv_color_make(100, 200, 100), 0);
    lv_obj_set_pos(body_emphasis, 10, y_pos);
    y_pos += 35;
    
    // Body Text - Regular 20 (Standard text, main content)
    lv_obj_t* body = lv_label_create(container);
    lv_label_set_text(body, "Temperature: 23°C\nHumidity: 45%");
    lv_obj_set_style_text_font(body, &stack_sans_regular_20, 0);
    lv_obj_set_style_text_color(body, lv_color_make(180, 180, 180), 0);
    lv_obj_set_pos(body, 10, y_pos);
    y_pos += 60;
    
    // Caption - Light 16 (Secondary information)
    lv_obj_t* caption = lv_label_create(container);
    lv_label_set_text(caption, "Last updated: 2 min ago");
    lv_obj_set_style_text_font(caption, &stack_sans_light_16, 0);
    lv_obj_set_style_text_color(caption, lv_color_make(120, 120, 120), 0);
    lv_obj_set_pos(caption, 10, y_pos);
    y_pos += 30;
    
    // Fine Print - ExtraLight 14 (Legal, disclaimers)
    lv_obj_t* fine_print = lv_label_create(container);
    lv_label_set_text(fine_print, "Swipe for more info");
    lv_obj_set_style_text_font(fine_print, &stack_sans_extralight_14, 0);
    lv_obj_set_style_text_color(fine_print, lv_color_make(100, 100, 100), 0);
    lv_obj_set_pos(fine_print, 10, y_pos);
    y_pos += 40;
    
    // Divider line
    lv_obj_t* line2 = lv_obj_create(container);
    lv_obj_set_size(line2, LV_HOR_RES - 40, 1);
    lv_obj_set_style_bg_color(line2, lv_color_make(80, 80, 80), 0);
    lv_obj_set_style_border_width(line2, 0, 0);
    lv_obj_set_pos(line2, 20, y_pos);
    y_pos += 20;
    
    // Weight comparison section
    lv_obj_t* comp_title = lv_label_create(container);
    lv_label_set_text(comp_title, "Font Weights");
    lv_obj_set_style_text_font(comp_title, &stack_sans_semibold_28, 0);
    lv_obj_set_style_text_color(comp_title, lv_color_white(), 0);
    lv_obj_set_pos(comp_title, 10, y_pos);
    y_pos += 40;
    
    // Show same text in different weights (size 20)
    #include "../fonts/stack_sans_extralight_20.c"
    #include "../fonts/stack_sans_light_20.c"
    #include "../fonts/stack_sans_medium_20.c"
    #include "../fonts/stack_sans_semibold_20.c"
    #include "../fonts/stack_sans_bold_20.c"
    
    LV_FONT_DECLARE(stack_sans_extralight_20);
    LV_FONT_DECLARE(stack_sans_light_20);
    LV_FONT_DECLARE(stack_sans_medium_20);
    LV_FONT_DECLARE(stack_sans_semibold_20);
    LV_FONT_DECLARE(stack_sans_bold_20);
    
    const char* text = "Stack Sans Text";
    const lv_font_t* fonts[] = {
        &stack_sans_extralight_20,
        &stack_sans_light_20,
        &stack_sans_regular_20,
        &stack_sans_medium_20,
        &stack_sans_semibold_20,
        &stack_sans_bold_20
    };
    const char* labels[] = {
        "ExtraLight",
        "Light",
        "Regular",
        "Medium",
        "SemiBold",
        "Bold"
    };
    
    for (int i = 0; i < 6; i++) {
        lv_obj_t* weight_sample = lv_label_create(container);
        lv_label_set_text(weight_sample, text);
        lv_obj_set_style_text_font(weight_sample, fonts[i], 0);
        lv_obj_set_style_text_color(weight_sample, lv_color_white(), 0);
        lv_obj_set_pos(weight_sample, 10, y_pos);
        
        // Add weight label
        lv_obj_t* weight_label = lv_label_create(container);
        lv_label_set_text(weight_label, labels[i]);
        lv_obj_set_style_text_font(weight_label, &stack_sans_extralight_14, 0);
        lv_obj_set_style_text_color(weight_label, lv_color_make(100, 100, 100), 0);
        lv_obj_align_to(weight_label, weight_sample, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
        
        y_pos += 28;
    }
    
    return screen;
}

/**
 * Typography Guide - Best practices
 * 
 * VISUAL HIERARCHY RECOMMENDATIONS:
 * 
 * 1. Hero/Display Text (67, 64)
 *    - Large numbers (time, temperature)
 *    - Use: Bold
 *    - Color: White or accent color
 * 
 * 2. Page Titles (40, 48)
 *    - Main screen headers
 *    - Use: Bold, SemiBold
 *    - Color: White
 * 
 * 3. Section Headers (28, 32)
 *    - Group labels, subsections
 *    - Use: SemiBold, Medium
 *    - Color: Light gray (200, 200, 200)
 * 
 * 4. Body Text (20, 24)
 *    - Main content, values
 *    - Use: Regular, Medium (for emphasis)
 *    - Color: Gray (180, 180, 180)
 * 
 * 5. Captions (16, 18)
 *    - Supporting text, labels
 *    - Use: Light, Regular
 *    - Color: Dark gray (120, 120, 120)
 * 
 * 6. Fine Print (12, 14)
 *    - Disclaimers, hints
 *    - Use: ExtraLight, Light
 *    - Color: Very dark gray (100, 100, 100)
 * 
 * MEMORY TIP: Only include the fonts you actually use!
 * Each font file adds to your binary size.
 */

#endif // FONT_DEMO_H

