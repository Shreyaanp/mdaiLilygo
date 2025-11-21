/*
 * Display Utilities - Responsive display helpers
 * Supports: LilyGo 1.43" AMOLED (466x466px circular display)
 * Features: boundary checking, responsive calculations, viewport helpers
 */

#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

#include <lvgl.h>
#include <math.h>

// ============================================================================
// DISPLAY CONSTANTS - LilyGo 1.43" Circular Display
// ============================================================================

// 1.43" circular: 466x466
#define DISPLAY_WIDTH 466
#define DISPLAY_HEIGHT 466
#define DISPLAY_IS_CIRCULAR 1

#define DISPLAY_RADIUS (DISPLAY_WIDTH / 2)
#define DISPLAY_CENTER_X (DISPLAY_WIDTH / 2)
#define DISPLAY_CENTER_Y (DISPLAY_HEIGHT / 2)

// Responsive breakpoints (distance from center)
#define VIEWPORT_INNER_RADIUS (DISPLAY_RADIUS * 0.5)
#define VIEWPORT_MIDDLE_RADIUS (DISPLAY_RADIUS * 0.75)
#define VIEWPORT_OUTER_RADIUS (DISPLAY_RADIUS * 0.95)

// ============================================================================
// VIEWPORT & RESPONSIVE ZONES
// ============================================================================

/**
 * Viewport zones (like media queries for circular display)
 */
typedef enum {
    VIEWPORT_CENTER,   // 0% - 50% from center
    VIEWPORT_MIDDLE,   // 50% - 75% from center
    VIEWPORT_EDGE,     // 75% - 95% from center
    VIEWPORT_OUTER     // 95% - 100% from center (may clip)
} viewport_zone_t;

/**
 * Get viewport zone for a given radius
 * @param radius Distance from center
 * @return Viewport zone
 */
static inline viewport_zone_t display_get_viewport_zone(int16_t radius) {
    if (radius < VIEWPORT_INNER_RADIUS) return VIEWPORT_CENTER;
    if (radius < VIEWPORT_MIDDLE_RADIUS) return VIEWPORT_MIDDLE;
    if (radius < VIEWPORT_OUTER_RADIUS) return VIEWPORT_EDGE;
    return VIEWPORT_OUTER;
}

/**
 * Get recommended font size for viewport zone (responsive typography)
 * @param zone Viewport zone
 * @return Recommended font size in pixels
 */
static inline int16_t display_responsive_font_size(viewport_zone_t zone) {
    switch (zone) {
        case VIEWPORT_CENTER: return 24;
        case VIEWPORT_MIDDLE: return 18;
        case VIEWPORT_EDGE: return 14;
        case VIEWPORT_OUTER: return 12;
        default: return 16;
    }
}

/**
 * Get recommended padding for viewport zone
 * @param zone Viewport zone
 * @return Recommended padding in pixels
 */
static inline int16_t display_responsive_padding(viewport_zone_t zone) {
    switch (zone) {
        case VIEWPORT_CENTER: return 20;
        case VIEWPORT_MIDDLE: return 15;
        case VIEWPORT_EDGE: return 10;
        case VIEWPORT_OUTER: return 5;
        default: return 10;
    }
}

// ============================================================================
// CIRCULAR BOUNDARY CHECKING
// ============================================================================

/**
 * Check if a rectangular element fits within the circular display
 * @param x X coordinate (top-left)
 * @param y Y coordinate (top-left)
 * @param width Element width
 * @param height Element height
 * @return true if element fits, false if it will be clipped
 */
static inline bool display_element_fits(int16_t x, int16_t y, int16_t width, int16_t height) {
    // Check all 4 corners of the rectangle
    int16_t corners[4][2] = {
        {x, y},                      // Top-left
        {x + width, y},              // Top-right
        {x, y + height},             // Bottom-left
        {x + width, y + height}      // Bottom-right
    };
    
    for (int i = 0; i < 4; i++) {
        int16_t dx = corners[i][0] - DISPLAY_CENTER_X;
        int16_t dy = corners[i][1] - DISPLAY_CENTER_Y;
        float distance = sqrt(dx * dx + dy * dy);
        
        if (distance > DISPLAY_RADIUS) {
            return false;  // Corner is outside circle
        }
    }
    
    return true;
}

/**
 * Get safe margin from edge (recommended padding from circle edge)
 * @return Safe margin in pixels
 */
static inline int16_t display_get_safe_margin() {
    return 20;  // 20px from edge
}

/**
 * Get maximum width at a given Y position (for circular display)
 * @param y Y coordinate
 * @return Maximum width at that Y position
 */
static inline int16_t display_max_width_at_y(int16_t y) {
    int16_t dy = abs(y - DISPLAY_CENTER_Y);
    if (dy >= DISPLAY_RADIUS) {
        return 0;
    }
    return 2 * sqrt(DISPLAY_RADIUS * DISPLAY_RADIUS - dy * dy);
}

/**
 * Check if a point is within the circular display
 * @param x X coordinate
 * @param y Y coordinate
 * @return true if point is visible
 */
static inline bool display_point_visible(int16_t x, int16_t y) {
    int16_t dx = x - DISPLAY_CENTER_X;
    int16_t dy = y - DISPLAY_CENTER_Y;
    return (dx * dx + dy * dy) <= (DISPLAY_RADIUS * DISPLAY_RADIUS);
}

/**
 * Alias for display_point_visible (for compatibility)
 */
static inline bool is_within_circular_bounds(int16_t x, int16_t y) {
    return display_point_visible(x, y);
}

/**
 * Calculate position for circular layout (items around edge)
 * @param index Item index
 * @param total Total number of items
 * @param radius Distance from center
 * @param x_out Output X coordinate
 * @param y_out Output Y coordinate
 */
static inline void display_circular_position(int index, int total, int16_t radius, int16_t* x_out, int16_t* y_out) {
    float angle = (2.0f * M_PI * index) / total;
    *x_out = DISPLAY_CENTER_X + (int16_t)(radius * cos(angle));
    *y_out = DISPLAY_CENTER_Y + (int16_t)(radius * sin(angle));
}

// ============================================================================
// RESPONSIVE CALCULATIONS
// ============================================================================

/**
 * Convert pixels to percentage of display width
 * @param pixels Pixel value
 * @return Percentage (0-100)
 */
static inline int16_t display_px_to_percent(int16_t pixels) {
    return (pixels * 100) / DISPLAY_WIDTH;
}

/**
 * Convert percentage to pixels
 * @param percent Percentage (0-100)
 * @return Pixel value
 */
static inline int16_t display_percent_to_px(int16_t percent) {
    return (percent * DISPLAY_WIDTH) / 100;
}

/**
 * Clamp value to safe display bounds (prevents overflow)
 * @param value Value to clamp
 * @param min Minimum value
 * @param max Maximum value
 * @return Clamped value
 */
static inline int16_t display_clamp(int16_t value, int16_t min, int16_t max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

/**
 * Calculate distance from center to point
 * @param x X coordinate
 * @param y Y coordinate
 * @return Distance in pixels
 */
static inline int16_t display_distance_from_center(int16_t x, int16_t y) {
    int16_t dx = x - DISPLAY_CENTER_X;
    int16_t dy = y - DISPLAY_CENTER_Y;
    return (int16_t)sqrt(dx * dx + dy * dy);
}

/**
 * Get angle from center to point (in degrees, 0 = right, 90 = bottom)
 * @param x X coordinate
 * @param y Y coordinate
 * @return Angle in degrees
 */
static inline float display_angle_from_center(int16_t x, int16_t y) {
    int16_t dx = x - DISPLAY_CENTER_X;
    int16_t dy = y - DISPLAY_CENTER_Y;
    return atan2(dy, dx) * 180.0f / M_PI;
}

/**
 * Scale value based on distance from center (responsive scaling)
 * @param base_value Base value at center
 * @param distance Distance from center
 * @param scale_factor Scaling factor (1.0 = no scaling)
 * @return Scaled value
 */
static inline int16_t display_scale_by_distance(int16_t base_value, int16_t distance, float scale_factor) {
    float factor = 1.0f - (distance / (float)DISPLAY_RADIUS * scale_factor);
    if (factor < 0.1f) factor = 0.1f;  // Minimum 10%
    return (int16_t)(base_value * factor);
}

/**
 * Get safe content rectangle (largest rectangle that fits in circle)
 * @param x_out Output X coordinate
 * @param y_out Output Y coordinate
 * @param w_out Output width
 * @param h_out Output height
 */
static inline void display_safe_rect(int16_t* x_out, int16_t* y_out, int16_t* w_out, int16_t* h_out) {
    // Square inscribed in circle: side = radius * sqrt(2)
    int16_t side = (int16_t)(DISPLAY_RADIUS * 1.414f);
    int16_t margin = display_get_safe_margin();
    
    *w_out = side - (margin * 2);
    *h_out = side - (margin * 2);
    *x_out = (DISPLAY_WIDTH - *w_out) / 2;
    *y_out = (DISPLAY_HEIGHT - *h_out) / 2;
}

/**
 * Check if object is in viewport (visible area)
 * @param obj Object to check
 * @return true if visible
 */
static inline bool display_is_visible(lv_obj_t* obj) {
    if (!obj) return false;
    
    lv_coord_t x = lv_obj_get_x(obj);
    lv_coord_t y = lv_obj_get_y(obj);
    lv_coord_t w = lv_obj_get_width(obj);
    lv_coord_t h = lv_obj_get_height(obj);
    
    // Check if any part is visible
    return display_point_visible(x, y) ||
           display_point_visible(x + w, y) ||
           display_point_visible(x, y + h) ||
           display_point_visible(x + w, y + h) ||
           display_point_visible(x + w/2, y + h/2);  // Center check
}

#endif // DISPLAY_UTILS_H

