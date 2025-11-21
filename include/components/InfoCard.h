/*
 * Info Card Component - Display information in a card
 */

#ifndef COMPONENT_INFO_CARD_H
#define COMPONENT_INFO_CARD_H

#include <lvgl.h>
#include "../utils/AnimationHelpers.h"

/**
 * Create an info card component
 * @param parent Parent screen
 * @param title Card title
 * @param value Card value/content
 * @param x X position
 * @param y Y position
 * @param width Card width
 * @param animate If true, adds slide-in animation
 * @return Card object
 */
static inline lv_obj_t* component_info_card_create(lv_obj_t* parent, const char* title, const char* value, 
                                                     int16_t x, int16_t y, int16_t width, bool animate) {
    // Card container
    lv_obj_t* card = lv_obj_create(parent);
    lv_obj_set_size(card, width, 80);
    lv_obj_set_pos(card, x - width/2, y - 40);  // Center on position
    lv_obj_set_style_bg_color(card, lv_color_hex(0x1E1E1E), 0);
    lv_obj_set_style_border_color(card, lv_color_hex(0x404040), 0);
    lv_obj_set_style_border_width(card, 2, 0);
    lv_obj_set_style_radius(card, 15, 0);
    lv_obj_set_style_pad_all(card, 10, 0);
    
    // Title label (small, top)
    lv_obj_t* title_label = lv_label_create(card);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_color(title_label, lv_color_hex(0x888888), 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_LEFT, 0, 0);
    
    // Value label (large, bottom)
    lv_obj_t* value_label = lv_label_create(card);
    lv_label_set_text(value_label, value);
    lv_obj_set_style_text_color(value_label, lv_color_white(), 0);
    lv_obj_align(value_label, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    
    // Animation
    if (animate) {
        anim_slide_in_bottom(card, 400, 0);
    }
    
    return card;
}

/**
 * Update card value
 * @param card Card object
 * @param new_value New value to display
 */
static inline void component_info_card_update(lv_obj_t* card, const char* new_value) {
    // Find the value label (second child)
    lv_obj_t* value_label = lv_obj_get_child(card, 1);
    if (value_label) {
        lv_label_set_text(value_label, new_value);
    }
}

/**
 * Highlight card (change border color)
 * @param card Card object
 * @param highlight If true, highlight; if false, normal
 */
static inline void component_info_card_highlight(lv_obj_t* card, bool highlight) {
    lv_color_t color = highlight ? lv_color_hex(0x2196F3) : lv_color_hex(0x404040);
    lv_obj_set_style_border_color(card, color, 0);
}

#endif // COMPONENT_INFO_CARD_H

