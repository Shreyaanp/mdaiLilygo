/*
 * Target Icon Component - Pure LVGL implementation
 * 30x30px light green circle with 8px black diagonal cross on top
 */

#ifndef TARGET_ICON_H
#define TARGET_ICON_H

#include <lvgl.h>

/**
 * Create target icon: 30x30px light green circle with diagonal cross (8px black lines)
 */
static inline lv_obj_t* target_icon_create(lv_obj_t* parent, lv_coord_t x, lv_coord_t y) {
    // Container for the target icon
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_size(container, 30, 30);
    lv_obj_set_pos(container, x, y);
    
    // Style as light green circle
    lv_obj_set_style_bg_color(container, lv_color_hex(0x00FF88), 0);  // Light green/cyan
    lv_obj_set_style_bg_opa(container, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_set_style_radius(container, LV_RADIUS_CIRCLE, 0);  // Make it circular
    
    // Create diagonal line 1 (top-left to bottom-right)
    static lv_point_t line1_points[] = {{0, 0}, {30, 30}};
    lv_obj_t* line1 = lv_line_create(container);
    lv_line_set_points(line1, line1_points, 2);
    lv_obj_set_style_line_width(line1, 8, 0);
    lv_obj_set_style_line_color(line1, lv_color_hex(0x000000), 0);  // Black
    lv_obj_set_style_line_rounded(line1, true, 0);
    
    // Create diagonal line 2 (top-right to bottom-left)
    static lv_point_t line2_points[] = {{30, 0}, {0, 30}};
    lv_obj_t* line2 = lv_line_create(container);
    lv_line_set_points(line2, line2_points, 2);
    lv_obj_set_style_line_width(line2, 8, 0);
    lv_obj_set_style_line_color(line2, lv_color_hex(0x000000), 0);  // Black
    lv_obj_set_style_line_rounded(line2, true, 0);
    
    return container;
}

#endif // TARGET_ICON_H

