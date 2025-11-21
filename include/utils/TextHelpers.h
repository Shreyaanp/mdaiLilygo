/*
 * Text Helpers - HTML5-like text components with box model support
 * Features: padding, margin, borders, text styling like HTML/CSS
 */

#ifndef TEXT_HELPERS_H
#define TEXT_HELPERS_H

#include <lvgl.h>
#include "DisplayUtils.h"

// ============================================================================
// HTML5-LIKE BOX MODEL STRUCTURES
// ============================================================================

/**
 * Box model configuration (like CSS box model)
 */
typedef struct {
    int16_t padding_top;
    int16_t padding_right;
    int16_t padding_bottom;
    int16_t padding_left;
    int16_t margin_top;
    int16_t margin_right;
    int16_t margin_bottom;
    int16_t margin_left;
    int16_t border_width;
    lv_color_t border_color;
    lv_color_t bg_color;
    lv_opa_t bg_opa;
    int16_t border_radius;
} box_model_t;

/**
 * Text style configuration (like CSS text properties)
 */
typedef struct {
    const lv_font_t* font;
    lv_color_t color;
    lv_text_align_t align;
    int16_t line_height;  // 0 = default
    int16_t letter_spacing;  // 0 = default
    bool bold;
    bool italic;
} text_style_t;

// ============================================================================
// HELPER FUNCTIONS FOR BOX MODEL
// ============================================================================

/**
 * Create default box model (all zeros)
 */
static inline box_model_t box_model_default() {
    box_model_t box = {0};
    box.bg_opa = LV_OPA_TRANSP;
    return box;
}

/**
 * Set uniform padding for all sides
 */
static inline void box_set_padding(box_model_t* box, int16_t padding) {
    box->padding_top = box->padding_right = box->padding_bottom = box->padding_left = padding;
}

/**
 * Set uniform margin for all sides
 */
static inline void box_set_margin(box_model_t* box, int16_t margin) {
    box->margin_top = box->margin_right = box->margin_bottom = box->margin_left = margin;
}

/**
 * Create default text style
 */
static inline text_style_t text_style_default() {
    text_style_t style = {0};
    style.font = NULL;  // Use LVGL default
    style.color = lv_color_white();
    style.align = LV_TEXT_ALIGN_LEFT;
    return style;
}

// ============================================================================
// HTML5-LIKE TEXT COMPONENTS
// ============================================================================

/**
 * Create a <div> like container with box model support
 * @param parent Parent object
 * @param width Width (0 = auto, LV_PCT for percentage)
 * @param height Height (0 = auto, LV_PCT for percentage)
 * @param box Box model configuration
 * @return Container object
 */
static inline lv_obj_t* create_div(lv_obj_t* parent, int16_t width, int16_t height, box_model_t* box) {
    lv_obj_t* div = lv_obj_create(parent);
    
    // Remove default styling
    lv_obj_clear_flag(div, LV_OBJ_FLAG_SCROLLABLE);
    
    // Set size
    if (width > 0) lv_obj_set_width(div, width);
    else lv_obj_set_width(div, LV_SIZE_CONTENT);
    
    if (height > 0) lv_obj_set_height(div, height);
    else lv_obj_set_height(div, LV_SIZE_CONTENT);
    
    // Apply box model if provided
    if (box) {
        // Padding
        lv_obj_set_style_pad_top(div, box->padding_top, 0);
        lv_obj_set_style_pad_right(div, box->padding_right, 0);
        lv_obj_set_style_pad_bottom(div, box->padding_bottom, 0);
        lv_obj_set_style_pad_left(div, box->padding_left, 0);
        
        // Border
        if (box->border_width > 0) {
            lv_obj_set_style_border_width(div, box->border_width, 0);
            lv_obj_set_style_border_color(div, box->border_color, 0);
        } else {
            lv_obj_set_style_border_width(div, 0, 0);
        }
        
        // Border radius
        lv_obj_set_style_radius(div, box->border_radius, 0);
        
        // Background
        lv_obj_set_style_bg_color(div, box->bg_color, 0);
        lv_obj_set_style_bg_opa(div, box->bg_opa, 0);
    } else {
        // Transparent by default
        lv_obj_set_style_bg_opa(div, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(div, 0, 0);
        lv_obj_set_style_pad_all(div, 0, 0);
    }
    
    return div;
}

/**
 * Create a <p> like paragraph with box model
 * @param parent Parent object
 * @param text Text content
 * @param style Text style
 * @param box Box model (padding, margin, etc)
 * @param max_width Maximum width (0 = parent width)
 * @return Paragraph container
 */
static inline lv_obj_t* create_paragraph(lv_obj_t* parent, const char* text, 
                                          text_style_t* style, box_model_t* box, 
                                          int16_t max_width) {
    // Create container div
    lv_obj_t* container = create_div(parent, max_width, 0, box);
    
    // Create label inside
    lv_obj_t* label = lv_label_create(container);
    lv_label_set_text(label, text);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    
    // Apply text style
    if (style) {
        if (style->font) lv_obj_set_style_text_font(label, style->font, 0);
        lv_obj_set_style_text_color(label, style->color, 0);
        lv_obj_set_style_text_align(label, style->align, 0);
        
        if (style->line_height > 0) {
            lv_obj_set_style_text_line_space(label, style->line_height, 0);
        }
        if (style->letter_spacing != 0) {
            lv_obj_set_style_text_letter_space(label, style->letter_spacing, 0);
        }
    }
    
    // Make label fill container
    lv_obj_set_width(label, LV_PCT(100));
    
    return container;
}

/**
 * Create a smart text label that handles wrapping and centering
 * @param parent Parent screen
 * @param text Text content
 * @param font Font to use (pass NULL for default)
 * @param x X position (center)
 * @param y Y position (center/top of text)
 * @param max_width Maximum width before wrapping (0 = screen width)
 * @param color Text color
 * @param align Alignment (LV_TEXT_ALIGN_LEFT, CENTER, RIGHT)
 * @return Label object
 */
static inline lv_obj_t* create_smart_text(lv_obj_t* parent, const char* text, 
                                           const lv_font_t* font, 
                                           int16_t x, int16_t y, 
                                           int16_t max_width,
                                           lv_color_t color,
                                           lv_text_align_t align) {
    // Create label
    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text(label, text);
    
    // Set font if provided
    if (font != NULL) {
        lv_obj_set_style_text_font(label, font, 0);
    }
    
    // Set color
    lv_obj_set_style_text_color(label, color, 0);
    
    // Calculate max width (leave margins for circular display)
    int16_t effective_max_width = max_width;
    if (effective_max_width == 0) {
        effective_max_width = DISPLAY_WIDTH - 40;  // 20px margin on each side
    }
    
    // Enable text wrapping if needed
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label, effective_max_width);
    
    // Set text alignment
    lv_obj_set_style_text_align(label, align, 0);
    
    // Position the label
    if (align == LV_TEXT_ALIGN_CENTER) {
        lv_obj_align(label, LV_ALIGN_TOP_MID, 0, y);
    } else if (align == LV_TEXT_ALIGN_LEFT) {
        lv_obj_set_pos(label, x, y);
    } else if (align == LV_TEXT_ALIGN_RIGHT) {
        int16_t right_x = x - lv_obj_get_width(label);
        lv_obj_set_pos(label, right_x, y);
    }
    
    return label;
}

/**
 * Create centered text with custom font
 * @param parent Parent screen
 * @param text Text content
 * @param font Font to use
 * @param y Y position (top of text)
 * @param color Text color
 * @return Label object
 */
static inline lv_obj_t* create_centered_text(lv_obj_t* parent, const char* text, 
                                              const lv_font_t* font,
                                              int16_t y,
                                              lv_color_t color) {
    return create_smart_text(parent, text, font, DISPLAY_CENTER_X, y, 
                            DISPLAY_WIDTH - 60, color, LV_TEXT_ALIGN_CENTER);
}

/**
 * Create multiline centered text that wraps automatically
 * @param parent Parent screen
 * @param text Text content
 * @param font Font to use (NULL for default)
 * @param y Y position
 * @param max_width Maximum width (0 = auto)
 * @param color Text color
 * @return Label object
 */
static inline lv_obj_t* create_multiline_text(lv_obj_t* parent, const char* text,
                                                const lv_font_t* font,
                                                int16_t y,
                                                int16_t max_width,
                                                lv_color_t color) {
    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text(label, text);
    
    if (font != NULL) {
        lv_obj_set_style_text_font(label, font, 0);
    }
    
    lv_obj_set_style_text_color(label, color, 0);
    
    // Calculate safe width for circular display
    int16_t safe_width = max_width;
    if (safe_width == 0) {
        // Calculate width based on Y position for circular screen
        int16_t center_y = DISPLAY_CENTER_Y;
        int16_t radius = DISPLAY_RADIUS;
        int16_t y_offset = abs(y - center_y);
        
        // Calculate available width at this Y position (Pythagorean theorem)
        if (y_offset < radius) {
            int16_t chord_half = (int16_t)sqrt((radius * radius) - (y_offset * y_offset));
            safe_width = (chord_half * 2) - 40;  // 20px margin on each side
        } else {
            safe_width = DISPLAY_WIDTH - 60;
        }
        
        // Minimum width constraint
        if (safe_width < 100) {
            safe_width = 100;
        }
    }
    
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label, safe_width);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, y);
    
    return label;
}

/**
 * Update text of an existing label
 * @param label Label object
 * @param text New text
 */
static inline void update_text(lv_obj_t* label, const char* text) {
    if (label) {
        lv_label_set_text(label, text);
    }
}

/**
 * Check if text fits within circular display boundaries
 * @param label Label object
 * @return true if text is fully visible
 */
static inline bool text_fits_in_circle(lv_obj_t* label) {
    if (!label) return false;
    
    lv_coord_t x = lv_obj_get_x(label);
    lv_coord_t y = lv_obj_get_y(label);
    lv_coord_t w = lv_obj_get_width(label);
    lv_coord_t h = lv_obj_get_height(label);
    
    // Check all four corners
    return is_within_circular_bounds(x, y) &&
           is_within_circular_bounds(x + w, y) &&
           is_within_circular_bounds(x, y + h) &&
           is_within_circular_bounds(x + w, y + h);
}

// ============================================================================
// ADDITIONAL HTML5-LIKE TEXT COMPONENTS
// ============================================================================

/**
 * Create <h1> to <h6> heading styles
 */
static inline lv_obj_t* create_heading(lv_obj_t* parent, const char* text, 
                                        int level, lv_color_t color) {
    text_style_t style = text_style_default();
    style.color = color;
    style.align = LV_TEXT_ALIGN_CENTER;
    
    box_model_t box = box_model_default();
    box_set_padding(&box, 10);
    box_set_margin(&box, 5);
    
    return create_paragraph(parent, text, &style, &box, DISPLAY_WIDTH - 60);
}

/**
 * Create a <span> like inline text
 */
static inline lv_obj_t* create_span(lv_obj_t* parent, const char* text, 
                                     const lv_font_t* font, lv_color_t color) {
    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text(label, text);
    if (font) lv_obj_set_style_text_font(label, font, 0);
    lv_obj_set_style_text_color(label, color, 0);
    return label;
}

/**
 * Create a <button> like clickable element with box model
 */
static inline lv_obj_t* create_button(lv_obj_t* parent, const char* text,
                                       int16_t width, int16_t height,
                                       box_model_t* box) {
    lv_obj_t* btn = lv_btn_create(parent);
    
    if (width > 0) lv_obj_set_width(btn, width);
    if (height > 0) lv_obj_set_height(btn, height);
    
    // Apply box model
    if (box) {
        lv_obj_set_style_pad_top(btn, box->padding_top, 0);
        lv_obj_set_style_pad_right(btn, box->padding_right, 0);
        lv_obj_set_style_pad_bottom(btn, box->padding_bottom, 0);
        lv_obj_set_style_pad_left(btn, box->padding_left, 0);
        lv_obj_set_style_radius(btn, box->border_radius, 0);
        
        if (box->bg_opa != LV_OPA_TRANSP) {
            lv_obj_set_style_bg_color(btn, box->bg_color, 0);
            lv_obj_set_style_bg_opa(btn, box->bg_opa, 0);
        }
    }
    
    // Add label
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    
    return btn;
}

/**
 * Create a <card> like component (combination of div with nice styling)
 */
static inline lv_obj_t* create_card(lv_obj_t* parent, int16_t width, int16_t height) {
    box_model_t box = box_model_default();
    box_set_padding(&box, 15);
    box.border_width = 2;
    box.border_color = lv_color_hex(0x333333);
    box.border_radius = 10;
    box.bg_color = lv_color_hex(0x1a1a1a);
    box.bg_opa = LV_OPA_80;
    
    return create_div(parent, width, height, &box);
}

/**
 * Create a <badge> like component (small labeled pill)
 */
static inline lv_obj_t* create_badge(lv_obj_t* parent, const char* text, lv_color_t bg_color) {
    box_model_t box = box_model_default();
    box_set_padding(&box, 4);
    box.bg_color = bg_color;
    box.bg_opa = LV_OPA_COVER;
    box.border_radius = 12;
    
    text_style_t style = text_style_default();
    style.align = LV_TEXT_ALIGN_CENTER;
    style.color = lv_color_white();
    
    lv_obj_t* badge = create_paragraph(parent, text, &style, &box, 0);
    lv_obj_set_height(badge, 24);
    
    return badge;
}

/**
 * Create multiline text with line spacing control (like CSS line-height)
 */
static inline lv_obj_t* create_multiline_styled(lv_obj_t* parent, const char* text,
                                                  const lv_font_t* font, lv_color_t color,
                                                  int16_t line_spacing, int16_t letter_spacing) {
    text_style_t style = text_style_default();
    style.font = font;
    style.color = color;
    style.align = LV_TEXT_ALIGN_CENTER;
    style.line_height = line_spacing;
    style.letter_spacing = letter_spacing;
    
    box_model_t box = box_model_default();
    box_set_padding(&box, 10);
    
    return create_paragraph(parent, text, &style, &box, DISPLAY_WIDTH - 60);
}

#endif // TEXT_HELPERS_H

