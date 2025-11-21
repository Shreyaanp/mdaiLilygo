/*
 * Two Arc Circular Loader Component
 * Two rotating arcs along the circular display border
 */

#ifndef TWO_ARC_CIRCULAR_LOADER_H
#define TWO_ARC_CIRCULAR_LOADER_H

#include <lvgl.h>

// Two Arc Loader configuration
typedef struct {
    lv_coord_t thickness;      // Border width (e.g., 20px)
    int16_t arc_length;        // Arc length in degrees for each arc (e.g., 90° = 1/4 circle)
    int16_t arc_gap;           // Gap between the two arcs in degrees (e.g., 90°)
    lv_color_t color;          // Arc color
    uint32_t animation_time;   // Rotation speed in ms (e.g., 20000ms for full rotation)
    bool rotate_continuously;  // If true, rotates forever
} two_arc_loader_config_t;

// Default configuration
static inline two_arc_loader_config_t two_arc_loader_default_config() {
    two_arc_loader_config_t config = {
        .thickness = 30,
        .arc_length = 90,      // 1/4 circle per arc
        .arc_gap = 90,         // 90° gap between arcs
        .color = lv_color_white(),
        .animation_time = 20000,
        .rotate_continuously = true
    };
    return config;
}

/**
 * Create two circular loader arcs
 * @param parent Parent screen object
 * @param config Loader configuration
 * @param arc1_out Output pointer for first arc object
 * @param arc2_out Output pointer for second arc object
 */
static inline void two_arc_loader_create(lv_obj_t* parent, two_arc_loader_config_t config, 
                                         lv_obj_t** arc1_out, lv_obj_t** arc2_out) {
    // Create first arc
    lv_obj_t* arc1 = lv_arc_create(parent);
    lv_obj_set_size(arc1, 466, 466);
    lv_obj_center(arc1);
    
    // Set background arc to only show where indicator will be
    lv_arc_set_bg_angles(arc1, 0, config.arc_length);
    lv_arc_set_angles(arc1, 0, config.arc_length);
    
    // Set arc value
    lv_arc_set_range(arc1, 0, 100);
    lv_arc_set_value(arc1, 100);
    
    // Hide background arc
    lv_obj_set_style_arc_width(arc1, 0, LV_PART_MAIN);
    lv_obj_set_style_arc_opa(arc1, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(arc1, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(arc1, 0, 0);
    lv_obj_set_style_pad_all(arc1, 0, LV_PART_MAIN);
    
    // Style the indicator arc
    lv_obj_set_style_arc_width(arc1, config.thickness, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc1, config.color, LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(arc1, LV_OPA_COVER, LV_PART_INDICATOR);
    
    // Remove knob
    lv_obj_set_style_opa(arc1, LV_OPA_TRANSP, LV_PART_KNOB);
    lv_obj_set_style_pad_all(arc1, 0, LV_PART_KNOB);
    lv_obj_set_style_radius(arc1, 0, LV_PART_KNOB);
    lv_obj_set_style_width(arc1, 0, LV_PART_KNOB);
    lv_obj_set_style_height(arc1, 0, LV_PART_KNOB);
    lv_obj_clear_flag(arc1, LV_OBJ_FLAG_CLICKABLE);
    
    lv_arc_set_mode(arc1, LV_ARC_MODE_NORMAL);
    lv_obj_clear_flag(arc1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(arc1);
    
    // Create second arc (opposite side)
    lv_obj_t* arc2 = lv_arc_create(parent);
    lv_obj_set_size(arc2, 466, 466);
    lv_obj_center(arc2);
    
    // Position second arc opposite to first (arc_length + arc_gap degrees away)
    int16_t arc2_start = config.arc_length + config.arc_gap;
    lv_arc_set_bg_angles(arc2, arc2_start, arc2_start + config.arc_length);
    lv_arc_set_angles(arc2, arc2_start, arc2_start + config.arc_length);
    
    lv_arc_set_range(arc2, 0, 100);
    lv_arc_set_value(arc2, 100);
    
    // Hide background arc
    lv_obj_set_style_arc_width(arc2, 0, LV_PART_MAIN);
    lv_obj_set_style_arc_opa(arc2, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(arc2, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(arc2, 0, 0);
    lv_obj_set_style_pad_all(arc2, 0, LV_PART_MAIN);
    
    // Style the indicator arc
    lv_obj_set_style_arc_width(arc2, config.thickness, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc2, config.color, LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(arc2, LV_OPA_COVER, LV_PART_INDICATOR);
    
    // Remove knob
    lv_obj_set_style_opa(arc2, LV_OPA_TRANSP, LV_PART_KNOB);
    lv_obj_set_style_pad_all(arc2, 0, LV_PART_KNOB);
    lv_obj_set_style_radius(arc2, 0, LV_PART_KNOB);
    lv_obj_set_style_width(arc2, 0, LV_PART_KNOB);
    lv_obj_set_style_height(arc2, 0, LV_PART_KNOB);
    lv_obj_clear_flag(arc2, LV_OBJ_FLAG_CLICKABLE);
    
    lv_arc_set_mode(arc2, LV_ARC_MODE_NORMAL);
    lv_obj_clear_flag(arc2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(arc2);
    
    // Return both arcs
    *arc1_out = arc1;
    *arc2_out = arc2;
}

// Structure to hold both arc pointers for animation
typedef struct {
    lv_obj_t* arc1;
    lv_obj_t* arc2;
    int16_t arc_length;
    int16_t arc_gap;
} two_arc_anim_data_t;

/**
 * Animation callback for rotating both arcs together
 */
static inline void two_arc_loader_anim_cb(void* var, int32_t value) {
    two_arc_anim_data_t* data = (two_arc_anim_data_t*)var;
    lv_obj_t* arc1 = data->arc1;
    lv_obj_t* arc2 = data->arc2;
    int16_t arc_length = data->arc_length;
    int16_t arc_gap = data->arc_gap;
    
    // Update first arc with angle wrapping
    int16_t start_angle1 = value % 360;
    int16_t end_angle1 = (value + arc_length) % 360;
    if (end_angle1 < start_angle1) {
        end_angle1 += 360;
    }
    lv_arc_set_angles(arc1, start_angle1, end_angle1);
    
    // Update second arc (offset by arc_length + gap)
    int16_t start_angle2 = (value + arc_length + arc_gap) % 360;
    int16_t end_angle2 = (value + arc_length + arc_gap + arc_length) % 360;
    if (end_angle2 < start_angle2) {
        end_angle2 += 360;
    }
    lv_arc_set_angles(arc2, start_angle2, end_angle2);
}

/**
 * Start rotation animation for both arcs
 * @param arc1 First arc object
 * @param arc2 Second arc object
 * @param duration Animation duration in ms
 * @param repeat If true, repeats forever
 */
static inline void two_arc_loader_start_rotation(lv_obj_t* arc1, lv_obj_t* arc2, 
                                                  uint32_t duration, bool repeat,
                                                  int16_t arc_length, int16_t arc_gap) {
    // Allocate animation data structure (will be managed by arc1's user data)
    two_arc_anim_data_t* anim_data = (two_arc_anim_data_t*)malloc(sizeof(two_arc_anim_data_t));
    if (anim_data == NULL) return;
    
    anim_data->arc1 = arc1;
    anim_data->arc2 = arc2;
    anim_data->arc_length = arc_length;
    anim_data->arc_gap = arc_gap;
    
    // Store in arc1's user data so it can be freed later
    lv_obj_set_user_data(arc1, anim_data);
    
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, anim_data);
    lv_anim_set_exec_cb(&anim, two_arc_loader_anim_cb);
    lv_anim_set_values(&anim, 0, 360);  // Rotate from 0° to 360°
    lv_anim_set_time(&anim, duration);
    lv_anim_set_repeat_count(&anim, repeat ? LV_ANIM_REPEAT_INFINITE : 0);
    lv_anim_start(&anim);
}

/**
 * Stop rotation animation
 * @param arc1 First arc object (used as animation reference)
 */
static inline void two_arc_loader_stop(lv_obj_t* arc1) {
    lv_anim_del(arc1, two_arc_loader_anim_cb);
}

#endif // TWO_ARC_CIRCULAR_LOADER_H

