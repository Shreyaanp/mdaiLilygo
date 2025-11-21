/*
 * TextScreen - Reusable text-only screen component
 * For screens that display simple text messages
 */

#ifndef TEXTSCREEN_H
#define TEXTSCREEN_H

#include "lvgl.h"
#include "../utils/TextHelpers.h"
#include "../utils/DisplayUtils.h"

// Declare the custom font
extern const lv_font_t stack_sans_semibold_48;

/**
 * Configuration for TextScreen
 */
typedef struct {
    const char* text;               // Text to display
    lv_color_t bg_color;           // Background color
    lv_color_t text_color;         // Text color
    const lv_font_t* font;         // Font to use (defaults to stack_sans_semibold_48)
    int16_t y_offset;              // Y position offset from center (default: -40)
} text_screen_config_t;

/**
 * Default configuration
 */
static inline text_screen_config_t text_screen_default_config() {
    text_screen_config_t config;
    config.text = "Text";
    config.bg_color = lv_color_hex(0x000000);  // Black
    config.text_color = lv_color_white();
    config.font = &stack_sans_semibold_48;
    config.y_offset = -40;
    return config;
}

/**
 * Create a text-only screen
 * Returns: screen object and text label
 */
typedef struct {
    lv_obj_t* screen;
    lv_obj_t* text_label;
} text_screen_t;

static inline text_screen_t* text_screen_create(text_screen_config_t* config) {
    text_screen_t* ts = (text_screen_t*)malloc(sizeof(text_screen_t));
    
    // Create screen
    ts->screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(ts->screen, config->bg_color, 0);
    
    // Create text label
    ts->text_label = create_multiline_text(
        ts->screen,
        config->text,
        config->font,
        DISPLAY_CENTER_Y + config->y_offset,
        0,  // Auto-calculate width
        config->text_color
    );
    
    return ts;
}

/**
 * Update text on existing text screen
 */
static inline void text_screen_update(text_screen_t* ts, const char* new_text) {
    if (ts && ts->text_label) {
        update_text(ts->text_label, new_text);
    }
}

/**
 * Update background color
 */
static inline void text_screen_set_bg_color(text_screen_t* ts, lv_color_t color) {
    if (ts && ts->screen) {
        lv_obj_set_style_bg_color(ts->screen, color, 0);
    }
}

/**
 * Update text color
 */
static inline void text_screen_set_text_color(text_screen_t* ts, lv_color_t color) {
    if (ts && ts->text_label) {
        lv_obj_set_style_text_color(ts->text_label, color, 0);
    }
}

#endif // TEXTSCREEN_H

