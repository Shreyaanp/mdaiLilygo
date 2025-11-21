/*
 * Circular Loader Component
 * A rotating arc loader along the circular display border
 */

#ifndef CIRCULAR_LOADER_H
#define CIRCULAR_LOADER_H

#include <lvgl.h>

// Loader configuration
typedef struct {
    lv_coord_t thickness;      // Border width (e.g., 20px)
    int16_t arc_length;        // Arc length in degrees (e.g., 90° = 1/4 circle)
    int16_t start_angle;       // Starting angle (0° = top, 90° = right)
    lv_color_t color;          // Arc color
    uint32_t animation_time;   // Rotation speed in ms (e.g., 2000ms for full rotation)
    bool rotate_continuously;  // If true, rotates forever
} circular_loader_config_t;

// Default configuration
static inline circular_loader_config_t circular_loader_default_config() {
    circular_loader_config_t config = {
        .thickness = 20,
        .arc_length = 90,      // 1/4 circle
        .start_angle = 0,      // Start at top
        .color = lv_color_white(),
        .animation_time = 2000,
        .rotate_continuously = true
    };
    return config;
}

/**
 * Create a circular loader arc
 * @param parent Parent screen object
 * @param config Loader configuration
 * @return The created arc object
 */
static inline lv_obj_t* circular_loader_create(lv_obj_t* parent, circular_loader_config_t config) {
    // Create arc widget
    lv_obj_t* arc = lv_arc_create(parent);
    
    // Set size - make it FULL SIZE for maximum visibility
    lv_obj_set_size(arc, 466, 466);  // Full display size
    lv_obj_center(arc);
    
    // CRITICAL: Set background arc to ONLY show where the indicator will be
    // This prevents a full circle from showing
    lv_arc_set_bg_angles(arc, config.start_angle, config.start_angle + config.arc_length);
    
    // Set the indicator angles (this is what will be visible and animate)
    lv_arc_set_angles(arc, config.start_angle, config.start_angle + config.arc_length);
    
    // IMPORTANT: Set the arc value to show the indicator
    lv_arc_set_range(arc, 0, 100);
    lv_arc_set_value(arc, 100);  // Set to max to show full arc length
    
    // HIDE the background arc completely - we only want the indicator
    lv_obj_set_style_arc_width(arc, 0, LV_PART_MAIN);
    lv_obj_set_style_arc_opa(arc, LV_OPA_TRANSP, LV_PART_MAIN);
    
    // Make background transparent
    lv_obj_set_style_bg_opa(arc, LV_OPA_TRANSP, 0);
    
    // Remove all padding to ensure arc reaches the edge
    lv_obj_set_style_pad_all(arc, 0, 0);
    lv_obj_set_style_pad_all(arc, 0, LV_PART_MAIN);
    
    // Style the indicator arc (the visible part) - MAXIMUM VISIBILITY
    lv_obj_set_style_arc_width(arc, config.thickness, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, config.color, LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(arc, LV_OPA_COVER, LV_PART_INDICATOR);  // Fully opaque
    lv_obj_set_style_arc_rounded(arc, false, LV_PART_INDICATOR);  // Flat ends, not rounded
    
    // Remove knob completely (we just want the arc, no interactive knob)
    lv_obj_set_style_opa(arc, LV_OPA_TRANSP, LV_PART_KNOB);
    lv_obj_set_style_pad_all(arc, 0, LV_PART_KNOB);
    lv_obj_set_style_radius(arc, 0, LV_PART_KNOB);
    lv_obj_set_style_width(arc, 0, LV_PART_KNOB);
    lv_obj_set_style_height(arc, 0, LV_PART_KNOB);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    
    // Make sure arc is visible and on top
    lv_arc_set_mode(arc, LV_ARC_MODE_NORMAL);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(arc);
    
    return arc;
}

/**
 * Animation callback for rotating the arc
 */
static inline void circular_loader_anim_cb(void* var, int32_t value) {
    lv_obj_t* arc = (lv_obj_t*)var;
    if (!arc) return;
    
    // Get current arc length (how much of the circle is covered)
    int16_t arc_length = lv_arc_get_angle_end(arc) - lv_arc_get_angle_start(arc);
    
    // Calculate end angle and wrap to 0-360 range
    int16_t start_angle = value % 360;
    int16_t end_angle = (value + arc_length) % 360;
    
    // Handle wrap-around case: if end < start, arc crosses 0°
    // LVGL needs the end angle to be >= start angle for proper rendering
    if (end_angle < start_angle) {
        end_angle += 360;
    }
    
    // Update the arc position with wrapped angles
    lv_arc_set_angles(arc, start_angle, end_angle);
}

/**
 * Start rotation animation
 * @param arc The arc object to animate
 * @param duration Animation duration in ms
 * @param repeat If true, repeats forever
 */
static inline void circular_loader_start_rotation(lv_obj_t* arc, uint32_t duration, bool repeat) {
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, arc);
    lv_anim_set_exec_cb(&anim, circular_loader_anim_cb);
    lv_anim_set_values(&anim, 0, 360);  // Rotate from 0° to 360°
    lv_anim_set_time(&anim, duration);
    lv_anim_set_repeat_count(&anim, repeat ? LV_ANIM_REPEAT_INFINITE : 0);
    lv_anim_start(&anim);
}

/**
 * Update arc length (how much of the circle is visible)
 * @param arc The arc object
 * @param length New arc length in degrees (0-360)
 */
static inline void circular_loader_set_arc_length(lv_obj_t* arc, int16_t length) {
    int16_t start = lv_arc_get_angle_start(arc);
    lv_arc_set_angles(arc, start, start + length);
}

/**
 * Update arc thickness
 * @param arc The arc object
 * @param thickness New thickness in pixels
 */
static inline void circular_loader_set_thickness(lv_obj_t* arc, lv_coord_t thickness) {
    lv_obj_set_style_arc_width(arc, thickness, LV_PART_INDICATOR);
}

/**
 * Update arc color
 * @param arc The arc object
 * @param color New color
 */
static inline void circular_loader_set_color(lv_obj_t* arc, lv_color_t color) {
    lv_obj_set_style_arc_color(arc, color, LV_PART_INDICATOR);
}

/**
 * Stop rotation animation
 * @param arc The arc object
 */
static inline void circular_loader_stop(lv_obj_t* arc) {
    lv_anim_del(arc, circular_loader_anim_cb);
}

#endif // CIRCULAR_LOADER_H

