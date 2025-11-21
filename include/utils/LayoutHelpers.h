/*
 * Layout Helpers - HTML5/CSS Flexbox-like positioning utilities
 * Features: flexbox, grid, responsive layouts
 */

#ifndef LAYOUT_HELPERS_H
#define LAYOUT_HELPERS_H

#include <lvgl.h>
#include "DisplayUtils.h"

// ============================================================================
// FLEXBOX-LIKE LAYOUT SYSTEM
// ============================================================================

/**
 * Flex direction (like CSS flex-direction)
 */
typedef enum {
    FLEX_ROW,              // Horizontal, left to right
    FLEX_ROW_REVERSE,      // Horizontal, right to left
    FLEX_COLUMN,           // Vertical, top to bottom
    FLEX_COLUMN_REVERSE    // Vertical, bottom to top
} flex_direction_t;

/**
 * Flex alignment (like CSS justify-content and align-items)
 */
typedef enum {
    FLEX_ALIGN_START,      // Start of container
    FLEX_ALIGN_END,        // End of container
    FLEX_ALIGN_CENTER,     // Center of container
    FLEX_ALIGN_SPACE_BETWEEN,  // Space between items
    FLEX_ALIGN_SPACE_AROUND,   // Space around items
    FLEX_ALIGN_SPACE_EVENLY    // Even space distribution
} flex_align_t;

/**
 * Flex wrap (like CSS flex-wrap)
 */
typedef enum {
    FLEX_NOWRAP,
    FLEX_WRAP,
    FLEX_WRAP_REVERSE
} flex_wrap_t;

/**
 * Flex container configuration
 */
typedef struct {
    flex_direction_t direction;
    flex_align_t main_align;      // justify-content
    flex_align_t cross_align;     // align-items
    flex_wrap_t wrap;
    int16_t gap;                   // Gap between items
} flex_config_t;

/**
 * Create default flex config
 */
static inline flex_config_t flex_config_default() {
    flex_config_t config = {0};
    config.direction = FLEX_ROW;
    config.main_align = FLEX_ALIGN_START;
    config.cross_align = FLEX_ALIGN_START;
    config.wrap = FLEX_NOWRAP;
    config.gap = 0;
    return config;
}

/**
 * Apply flexbox layout to a container
 * @param container Container object
 * @param config Flex configuration
 */
static inline void layout_flex(lv_obj_t* container, flex_config_t* config) {
    if (!container || !config) return;
    
    // Set LVGL flex layout
    lv_obj_set_layout(container, LV_LAYOUT_FLEX);
    
    // Set direction
    lv_flex_flow_t flow;
    switch (config->direction) {
        case FLEX_ROW:
            flow = LV_FLEX_FLOW_ROW;
            break;
        case FLEX_ROW_REVERSE:
            flow = LV_FLEX_FLOW_ROW_REVERSE;
            break;
        case FLEX_COLUMN:
            flow = LV_FLEX_FLOW_COLUMN;
            break;
        case FLEX_COLUMN_REVERSE:
            flow = LV_FLEX_FLOW_COLUMN_REVERSE;
            break;
        default:
            flow = LV_FLEX_FLOW_ROW;
    }
    
    // Add wrap if needed
    if (config->wrap == FLEX_WRAP) {
        flow |= LV_FLEX_FLOW_WRAP;
    } else if (config->wrap == FLEX_WRAP_REVERSE) {
        flow |= LV_FLEX_FLOW_WRAP_REVERSE;
    }
    
    lv_obj_set_flex_flow(container, flow);
    
    // Set main axis alignment (justify-content)
    lv_flex_align_t main_place;
    switch (config->main_align) {
        case FLEX_ALIGN_START:
            main_place = LV_FLEX_ALIGN_START;
            break;
        case FLEX_ALIGN_END:
            main_place = LV_FLEX_ALIGN_END;
            break;
        case FLEX_ALIGN_CENTER:
            main_place = LV_FLEX_ALIGN_CENTER;
            break;
        case FLEX_ALIGN_SPACE_BETWEEN:
            main_place = LV_FLEX_ALIGN_SPACE_BETWEEN;
            break;
        case FLEX_ALIGN_SPACE_AROUND:
            main_place = LV_FLEX_ALIGN_SPACE_AROUND;
            break;
        case FLEX_ALIGN_SPACE_EVENLY:
            main_place = LV_FLEX_ALIGN_SPACE_EVENLY;
            break;
        default:
            main_place = LV_FLEX_ALIGN_START;
    }
    lv_obj_set_flex_align(container, main_place, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    
    // Set gap
    if (config->gap > 0) {
        if (config->direction == FLEX_ROW || config->direction == FLEX_ROW_REVERSE) {
            lv_obj_set_style_pad_column(container, config->gap, 0);
        } else {
            lv_obj_set_style_pad_row(container, config->gap, 0);
        }
    }
}

// ============================================================================
// GRID-LIKE LAYOUT SYSTEM
// ============================================================================

/**
 * Create a grid layout (like CSS Grid)
 * @param container Container object
 * @param columns Number of columns
 * @param rows Number of rows
 * @param gap Gap between cells
 */
static inline void layout_grid(lv_obj_t* container, int16_t columns, int16_t rows, int16_t gap) {
    lv_obj_set_layout(container, LV_LAYOUT_GRID);
    
    // Create column template
    static lv_coord_t col_dsc[10];  // Max 10 columns
    for (int i = 0; i < columns && i < 10; i++) {
        col_dsc[i] = LV_GRID_FR(1);  // Equal fractions
    }
    col_dsc[columns < 10 ? columns : 9] = LV_GRID_TEMPLATE_LAST;
    
    // Create row template
    static lv_coord_t row_dsc[10];  // Max 10 rows
    for (int i = 0; i < rows && i < 10; i++) {
        row_dsc[i] = LV_GRID_FR(1);  // Equal fractions
    }
    row_dsc[rows < 10 ? rows : 9] = LV_GRID_TEMPLATE_LAST;
    
    lv_obj_set_grid_dsc_array(container, col_dsc, row_dsc);
    
    if (gap > 0) {
        lv_obj_set_style_pad_column(container, gap, 0);
        lv_obj_set_style_pad_row(container, gap, 0);
    }
}

/**
 * Place item in grid cell
 * @param item Item to place
 * @param col Column index (0-based)
 * @param row Row index (0-based)
 * @param col_span Column span (1 = single cell)
 * @param row_span Row span (1 = single cell)
 */
static inline void layout_grid_cell(lv_obj_t* item, int16_t col, int16_t row, 
                                     int16_t col_span, int16_t row_span) {
    lv_obj_set_grid_cell(item, LV_GRID_ALIGN_STRETCH, col, col_span,
                               LV_GRID_ALIGN_STRETCH, row, row_span);
}

// ============================================================================
// ORIGINAL LAYOUT HELPERS (ENHANCED)
// ============================================================================

/**
 * Center an object on screen
 */
static inline void layout_center(lv_obj_t* obj) {
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
}

/**
 * Position object at top center (with safe margin)
 */
static inline void layout_top_center(lv_obj_t* obj, int16_t offset_y) {
    lv_obj_align(obj, LV_ALIGN_TOP_MID, 0, display_get_safe_margin() + offset_y);
}

/**
 * Position object at bottom center (with safe margin)
 */
static inline void layout_bottom_center(lv_obj_t* obj, int16_t offset_y) {
    lv_obj_align(obj, LV_ALIGN_BOTTOM_MID, 0, -display_get_safe_margin() + offset_y);
}

/**
 * Create a vertical stack layout
 * @param parent Parent object
 * @param children Array of child objects
 * @param count Number of children
 * @param spacing Spacing between items
 * @param start_y Starting Y position
 */
static inline void layout_vertical_stack(lv_obj_t* parent, lv_obj_t** children, int count, int16_t spacing, int16_t start_y) {
    int16_t current_y = start_y;
    for (int i = 0; i < count; i++) {
        lv_obj_align(children[i], LV_ALIGN_TOP_MID, 0, current_y);
        current_y += lv_obj_get_height(children[i]) + spacing;
    }
}

/**
 * Create a horizontal layout (centered)
 * @param parent Parent object
 * @param children Array of child objects
 * @param count Number of children
 * @param spacing Spacing between items
 * @param y Y position
 */
static inline void layout_horizontal_center(lv_obj_t* parent, lv_obj_t** children, int count, int16_t spacing, int16_t y) {
    // Calculate total width
    int16_t total_width = 0;
    for (int i = 0; i < count; i++) {
        total_width += lv_obj_get_width(children[i]);
        if (i < count - 1) total_width += spacing;
    }
    
    // Start position (centered)
    int16_t start_x = (DISPLAY_WIDTH - total_width) / 2;
    
    // Position children
    for (int i = 0; i < count; i++) {
        lv_obj_set_pos(children[i], start_x, y);
        start_x += lv_obj_get_width(children[i]) + spacing;
    }
}

// ============================================================================
// RESPONSIVE & CIRCULAR DISPLAY HELPERS
// ============================================================================

/**
 * Create responsive layout that adapts to circular display
 * @param container Container object
 * @param y_position Y position to calculate safe width
 * @return Safe width for content at this Y position
 */
static inline int16_t layout_responsive_width(int16_t y_position) {
    return display_max_width_at_y(y_position) - (display_get_safe_margin() * 2);
}

/**
 * Position elements in circular arc pattern
 * @param parent Parent object
 * @param children Array of child objects
 * @param count Number of children
 * @param radius Arc radius from center
 * @param start_angle Starting angle in degrees (0 = right, 90 = bottom)
 * @param arc_length Total arc span in degrees
 */
static inline void layout_circular_arc(lv_obj_t* parent, lv_obj_t** children, 
                                       int count, int16_t radius, 
                                       int16_t start_angle, int16_t arc_length) {
    for (int i = 0; i < count; i++) {
        float angle_deg = start_angle + (arc_length * i / (float)(count - 1));
        float angle_rad = angle_deg * M_PI / 180.0f;
        
        int16_t x = DISPLAY_CENTER_X + (int16_t)(radius * cos(angle_rad));
        int16_t y = DISPLAY_CENTER_Y + (int16_t)(radius * sin(angle_rad));
        
        lv_obj_set_pos(children[i], 
                      x - lv_obj_get_width(children[i]) / 2,
                      y - lv_obj_get_height(children[i]) / 2);
    }
}

/**
 * Create stack layout with automatic spacing
 * @param container Container to add children to
 * @param children Array of child objects
 * @param count Number of children
 * @param total_height Total available height
 * @param start_y Starting Y position
 */
static inline void layout_auto_stack(lv_obj_t* container, lv_obj_t** children, 
                                     int count, int16_t total_height, int16_t start_y) {
    // Calculate total content height
    int16_t content_height = 0;
    for (int i = 0; i < count; i++) {
        content_height += lv_obj_get_height(children[i]);
    }
    
    // Calculate spacing
    int16_t spacing = (count > 1) ? (total_height - content_height) / (count - 1) : 0;
    
    // Position items
    int16_t current_y = start_y;
    for (int i = 0; i < count; i++) {
        lv_obj_align(children[i], LV_ALIGN_TOP_MID, 0, current_y);
        current_y += lv_obj_get_height(children[i]) + spacing;
    }
}

/**
 * Create absolute positioning helper (like CSS position: absolute)
 * @param obj Object to position
 * @param x X coordinate
 * @param y Y coordinate
 * @param anchor_x X anchor (0.0 = left, 0.5 = center, 1.0 = right)
 * @param anchor_y Y anchor (0.0 = top, 0.5 = center, 1.0 = bottom)
 */
static inline void layout_absolute(lv_obj_t* obj, int16_t x, int16_t y, 
                                    float anchor_x, float anchor_y) {
    int16_t offset_x = -(int16_t)(lv_obj_get_width(obj) * anchor_x);
    int16_t offset_y = -(int16_t)(lv_obj_get_height(obj) * anchor_y);
    lv_obj_set_pos(obj, x + offset_x, y + offset_y);
}

/**
 * Z-index simulation (LVGL doesn't have true z-index, use layer movement)
 * @param obj Object to move
 * @param to_front true to move to front, false to back
 */
static inline void layout_z_index(lv_obj_t* obj, bool to_front) {
    if (to_front) {
        lv_obj_move_foreground(obj);
    } else {
        lv_obj_move_background(obj);
    }
}

#endif // LAYOUT_HELPERS_H

