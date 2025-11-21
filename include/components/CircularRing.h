/*
 * Circular Ring Component - Face Scanning Style Progress Ring
 * 
 * Features:
 * - Tick-based circular progress indicator
 * - Static ticks that change color based on progress
 * - Perfect for face scanning, biometric auth, loading indicators
 * - Smooth animation support
 */

#ifndef CIRCULAR_RING_H
#define CIRCULAR_RING_H

#include "lvgl.h"
#include <math.h>
#include <stdlib.h>  // For malloc/free

// ============================================================================
// CIRCULAR RING CONFIGURATION
// ============================================================================

/**
 * Circular ring configuration
 */
typedef struct {
    int16_t center_x;           // Center X position
    int16_t center_y;           // Center Y position
    int16_t radius;             // Distance from center to ticks
    int16_t tick_count;         // Number of ticks around the circle
    int16_t tick_length;        // Length of each tick bar
    int16_t tick_width;         // Thickness/width of each tick
    
    lv_color_t active_color;    // Color for active/scanned ticks
    lv_color_t inactive_color;  // Color for inactive/pending ticks
    lv_opa_t active_opa;        // Opacity for active ticks
    lv_opa_t inactive_opa;      // Opacity for inactive ticks
    
    float progress;             // Progress value (0.0 - 1.0, where 1.0 = 100%)
    int16_t start_angle;        // Starting angle in degrees (0 = top, 90 = right)
    bool clockwise;             // Direction of progress sweep
    
    bool rounded_caps;          // Use rounded line caps
} circular_ring_config_t;

/**
 * Circular ring object structure
 */
typedef struct {
    lv_obj_t* container;        // Container object
    lv_obj_t** tick_lines;      // Array of tick line objects
#if LV_VERSION_CHECK(9, 0, 0)
    lv_point_precise_t** tick_points; // LVGL 9: precise points (must persist!)
#else
    lv_point_t** tick_points;   // LVGL 8: integer points (must persist!)
#endif
    circular_ring_config_t config;  // Current configuration
    lv_timer_t* anim_timer;     // Animation timer (for placeholder demo)
    float target_progress;      // Target progress for animation
    int16_t tick_count;         // Store tick count for cleanup
} circular_ring_t;

// ============================================================================
// DEFAULT CONFIGURATION
// ============================================================================

/**
 * Create default circular ring config (face scanning style)
 */
static inline circular_ring_config_t circular_ring_config_default() {
    circular_ring_config_t config = {0};
    config.center_x = 233;              // Center of 466x466 display
    config.center_y = 233;
    config.radius = 200;                // Ring radius
    config.tick_count = 60;             // 60 ticks (like seconds)
    config.tick_length = 20;            // Tick length
    config.tick_width = 3;              // Tick thickness
    
    config.active_color = lv_color_hex(0x00FF00);    // Green (like Face ID)
    config.inactive_color = lv_color_hex(0x333333);  // Dark grey
    config.active_opa = LV_OPA_COVER;
    config.inactive_opa = LV_OPA_50;
    
    config.progress = 0.0f;
    config.start_angle = -90;           // Start at top (12 o'clock)
    config.clockwise = true;
    config.rounded_caps = true;
    
    return config;
}

// ============================================================================
// CIRCULAR RING CREATION & MANAGEMENT
// ============================================================================

/**
 * Draw/update tick colors based on current progress
 */
static inline void circular_ring_update_ticks(circular_ring_t* ring) {
    if (!ring || !ring->tick_lines) return;
    
    int16_t active_ticks = (int16_t)(ring->config.progress * ring->config.tick_count);
    
    for (int i = 0; i < ring->config.tick_count; i++) {
        if (i < active_ticks) {
            // Active tick (scanned/complete)
            lv_obj_set_style_line_color(ring->tick_lines[i], ring->config.active_color, 0);
            lv_obj_set_style_line_opa(ring->tick_lines[i], ring->config.active_opa, 0);
        } else {
            // Inactive tick (pending)
            lv_obj_set_style_line_color(ring->tick_lines[i], ring->config.inactive_color, 0);
            lv_obj_set_style_line_opa(ring->tick_lines[i], ring->config.inactive_opa, 0);
        }
    }
}

/**
 * Create circular ring component
 * @param parent Parent screen/object
 * @param config Ring configuration
 * @return Circular ring object
 */
static inline circular_ring_t* circular_ring_create(lv_obj_t* parent, circular_ring_config_t* config) {
    // Allocate ring structure using malloc (LVGL 9 doesn't expose lv_mem_alloc)
    circular_ring_t* ring = (circular_ring_t*)malloc(sizeof(circular_ring_t));
    if (!ring) return NULL;
    
    // Copy configuration
    ring->config = *config;
    ring->target_progress = config->progress;
    ring->anim_timer = NULL;
    ring->tick_count = config->tick_count;
    
    // Create container with explicit size to contain all lines
    ring->container = lv_obj_create(parent);
    lv_obj_clear_flag(ring->container, LV_OBJ_FLAG_SCROLLABLE);
    
    // Set explicit size: diameter = 2 * (radius + tick_length) to contain all ticks
    int16_t container_size = 2 * (config->radius + config->tick_length) + 10; // +10 for safety margin
    lv_obj_set_size(ring->container, container_size, container_size);
    
    // Transparent container
    lv_obj_set_style_bg_opa(ring->container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(ring->container, 0, 0);
    lv_obj_set_style_pad_all(ring->container, 0, 0);
    
    // Allocate tick line array
    ring->tick_lines = (lv_obj_t**)malloc(sizeof(lv_obj_t*) * config->tick_count);
    if (!ring->tick_lines) {
        free(ring);
        return NULL;
    }
    
    // Allocate tick points array - CRITICAL: Points must persist for LVGL!
#if LV_VERSION_CHECK(9, 0, 0)
    ring->tick_points = (lv_point_precise_t**)malloc(sizeof(lv_point_precise_t*) * config->tick_count);
#else
    ring->tick_points = (lv_point_t**)malloc(sizeof(lv_point_t*) * config->tick_count);
#endif
    if (!ring->tick_points) {
        free(ring->tick_lines);
        free(ring);
        return NULL;
    }
    
    // Create tick marks around the circle
    float angle_step = 360.0f / config->tick_count;
    
    // Calculate center offset for lines within container
    int16_t container_center = (config->radius + config->tick_length) + 5; // Center of container
    
    for (int i = 0; i < config->tick_count; i++) {
        // Calculate angle for this tick
        float angle_deg = config->start_angle + (config->clockwise ? i : -i) * angle_step;
        float angle_rad = angle_deg * M_PI / 180.0f;
        
        // Calculate tick start and end points (relative to container center)
        int16_t outer_x = container_center + (int16_t)(config->radius * cos(angle_rad));
        int16_t outer_y = container_center + (int16_t)(config->radius * sin(angle_rad));
        int16_t inner_x = container_center + (int16_t)((config->radius - config->tick_length) * cos(angle_rad));
        int16_t inner_y = container_center + (int16_t)((config->radius - config->tick_length) * sin(angle_rad));
        
        // Allocate persistent points for this line - MUST persist for LVGL!
#if LV_VERSION_CHECK(9, 0, 0)
        ring->tick_points[i] = (lv_point_precise_t*)malloc(sizeof(lv_point_precise_t) * 2);
#else
        ring->tick_points[i] = (lv_point_t*)malloc(sizeof(lv_point_t) * 2);
#endif
        if (!ring->tick_points[i]) {
            // Cleanup on failure
            for (int j = 0; j < i; j++) {
                free(ring->tick_points[j]);
            }
            free(ring->tick_points);
            free(ring->tick_lines);
            free(ring);
            return NULL;
        }
        
        ring->tick_points[i][0].x = inner_x;
        ring->tick_points[i][0].y = inner_y;
        ring->tick_points[i][1].x = outer_x;
        ring->tick_points[i][1].y = outer_y;
        
        lv_obj_t* line = lv_line_create(ring->container);
        lv_line_set_points(line, ring->tick_points[i], 2);  // Points will persist!
        
        // Style the tick
        lv_obj_set_style_line_width(line, config->tick_width, 0);
        lv_obj_set_style_line_color(line, config->inactive_color, 0);
        lv_obj_set_style_line_opa(line, config->inactive_opa, 0);
        
        if (config->rounded_caps) {
            lv_obj_set_style_line_rounded(line, true, 0);
        }
        
        ring->tick_lines[i] = line;
    }
    
    // Update tick colors based on initial progress
    circular_ring_update_ticks(ring);
    
    // Position container so its center aligns with config center point
    // Container top-left = center - (container_size / 2)
    lv_obj_set_pos(ring->container, 
                   config->center_x - (container_size / 2), 
                   config->center_y - (container_size / 2));
    
    return ring;
}

/**
 * Set progress value (0.0 - 1.0)
 * @param ring Circular ring object
 * @param progress Progress value (0.0 = 0%, 1.0 = 100%)
 */
static inline void circular_ring_set_progress(circular_ring_t* ring, float progress) {
    if (!ring) return;
    
    // Clamp progress to valid range
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;
    
    ring->config.progress = progress;
    circular_ring_update_ticks(ring);
}

/**
 * Set progress by percentage (0-100)
 * @param ring Circular ring object
 * @param percentage Progress percentage (0-100)
 */
static inline void circular_ring_set_percentage(circular_ring_t* ring, int16_t percentage) {
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;
    circular_ring_set_progress(ring, percentage / 100.0f);
}

/**
 * Change active/inactive colors
 * @param ring Circular ring object
 * @param active_color Color for scanned/active ticks
 * @param inactive_color Color for pending/inactive ticks
 */
static inline void circular_ring_set_colors(circular_ring_t* ring, 
                                             lv_color_t active_color, 
                                             lv_color_t inactive_color) {
    if (!ring) return;
    
    ring->config.active_color = active_color;
    ring->config.inactive_color = inactive_color;
    circular_ring_update_ticks(ring);
}

// ============================================================================
// PLACEHOLDER ANIMATION (10-SECOND DEMO)
// ============================================================================

/**
 * Animation timer callback (for 10-second placeholder demo)
 */
static void circular_ring_anim_timer_cb(lv_timer_t* timer) {
#if LV_VERSION_CHECK(9, 0, 0)
    circular_ring_t* ring = (circular_ring_t*)lv_timer_get_user_data(timer);
#else
    circular_ring_t* ring = (circular_ring_t*)timer->user_data;
#endif
    if (!ring) return;
    
    // Increment progress (10 seconds = 10000ms, update every 100ms = 100 steps)
    ring->config.progress += 0.01f;  // 1% per step
    
    if (ring->config.progress >= 1.0f) {
        ring->config.progress = 1.0f;
        lv_timer_del(timer);  // Stop timer when complete
        ring->anim_timer = NULL;
    }
    
    circular_ring_update_ticks(ring);
}

/**
 * Start 10-second placeholder animation
 * @param ring Circular ring object
 */
static inline void circular_ring_start_placeholder_anim(circular_ring_t* ring) {
    if (!ring) return;
    
    // Stop existing animation if any
    if (ring->anim_timer) {
        lv_timer_del(ring->anim_timer);
    }
    
    // Reset progress
    ring->config.progress = 0.0f;
    circular_ring_update_ticks(ring);
    
    // Create timer: 10 seconds / 100 steps = 100ms per step
    ring->anim_timer = lv_timer_create(circular_ring_anim_timer_cb, 100, ring);
}

/**
 * Stop animation
 * @param ring Circular ring object
 */
static inline void circular_ring_stop_anim(circular_ring_t* ring) {
    if (!ring || !ring->anim_timer) return;
    lv_timer_del(ring->anim_timer);
    ring->anim_timer = NULL;
}

/**
 * Reset ring to 0% progress
 * @param ring Circular ring object
 */
static inline void circular_ring_reset(circular_ring_t* ring) {
    if (!ring) return;
    circular_ring_stop_anim(ring);
    circular_ring_set_progress(ring, 0.0f);
}

/**
 * Delete circular ring and free memory
 * @param ring Circular ring object
 */
static inline void circular_ring_delete(circular_ring_t* ring) {
    if (!ring) return;
    
    circular_ring_stop_anim(ring);
    
    // Free point arrays
    if (ring->tick_points) {
        for (int i = 0; i < ring->tick_count; i++) {
            if (ring->tick_points[i]) {
                free(ring->tick_points[i]);
            }
        }
        free(ring->tick_points);
    }
    
    if (ring->tick_lines) {
        free(ring->tick_lines);
    }
    
    if (ring->container) {
        lv_obj_del(ring->container);
    }
    
    free(ring);
}

#endif // CIRCULAR_RING_H

