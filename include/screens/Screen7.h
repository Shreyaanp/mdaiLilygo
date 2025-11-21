/*
 * Screen 7 - Nose Tracking with target icon and circular ring
 */

#ifndef SCREEN_7_H
#define SCREEN_7_H

#include <lvgl.h>
#include "../components/TargetIcon.h"
#include "../components/CircularRing.h"
#include "../state/AppState.h"
#include "../utils/LayerManager.h"

// Screen 7 objects
static lv_obj_t* screen7 = NULL;
static lv_obj_t* screen7_target = NULL;
static circular_ring_t* screen7_ring = NULL;
static AppState* screen7_appState = NULL;

/**
 * Create Screen 7 - Target icon with circular ring and nose tracking
 */
static inline lv_obj_t* screen7_create() {
    // Create screen
    screen7 = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen7, lv_color_hex(0x000000), 0);  // Black background

    // Clear any default padding/border
    lv_obj_set_style_pad_all(screen7, 0, 0);
    lv_obj_set_style_border_width(screen7, 0, 0);

    // Get AppState instance
    screen7_appState = AppState::getInstance();

    // Create circular ring around the target (same as Screen 0)
    circular_ring_config_t ring_config = circular_ring_config_default();
    ring_config.center_x = 233;
    ring_config.center_y = 233;
    ring_config.radius = 200;           // Outer edge at 200px from center
    ring_config.tick_count = 60;
    ring_config.tick_length = 25;       // Ticks extend inward 25px
    ring_config.tick_width = 5;
    ring_config.active_color = lv_color_hex(0x00FF88);  // Light green to match target
    ring_config.inactive_color = lv_color_hex(0x444444); // Grey
    ring_config.inactive_opa = LV_OPA_80;
    ring_config.rounded_caps = true;
    
    screen7_ring = circular_ring_create(screen7, &ring_config);
    
    // Start 10-second animation
    if (screen7_ring) {
        circular_ring_start_placeholder_anim(screen7_ring);
    }

    // Display target icon at center (initial position) - 30x30px
    screen7_target = target_icon_create(screen7, 233 - 15, 233 - 15);  // Center at (233, 233)
    
    // Apply explicit layer order (bottom to top)
    ui_component_t screen7_layers[] = {
        UI_COMPONENT("circular_ring", screen7_ring->container, LAYER_RING),
        UI_COMPONENT("target_icon", screen7_target, LAYER_TARGET),
    };
    apply_layer_order(screen7_layers, 2);

    return screen7;
}

/**
 * Update Screen 7 - Move target to tracked position
 * Call this from your main loop when on Screen 7
 */
static inline void screen7_update_position() {
    if (screen7_target && screen7_appState) {
        // Only update position if tracking is active
        if (screen7_appState->isTrackingActive()) {
            int16_t targetX = screen7_appState->getTargetX();
            int16_t targetY = screen7_appState->getTargetY();
            
            // Set position (center the 30x30px icon on the target coordinates)
            lv_obj_set_pos(screen7_target, targetX - 15, targetY - 15);
        } else {
            // No tracking data - keep centered
            lv_obj_set_pos(screen7_target, 233 - 15, 233 - 15);
        }
    }
}

/**
 * Update Screen 7 (if needed for other updates)
 */
static inline void screen7_update() {
    // Called when state changes
    screen7_update_position();
}

#endif // SCREEN_7_H