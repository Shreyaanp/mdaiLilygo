/*
 * Screen 0 - Circular Ring Demo (Face Scanning Style)
 * Now working with LVGL 9 persistent points!
 */

#ifndef SCREEN_0_H
#define SCREEN_0_H

#include "lvgl.h"
#include "../components/CircularRing.h"
#include "../components/TargetIcon.h"

// Screen 0 objects
static lv_obj_t* screen0 = NULL;
static lv_obj_t* screen0_target = NULL;
static lv_obj_t* screen0_label = NULL;
static circular_ring_t* screen0_ring = NULL;

lv_obj_t* screen0_create() {
    // Create screen
    screen0 = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen0, lv_color_hex(0x000000), 0);
    
    // Display target icon at center (30x30px circle with diagonal cross)
    screen0_target = target_icon_create(screen0, 233 - 15, 233 - 15);  // Center at (233, 233)
    
    // Add label
    screen0_label = lv_label_create(screen0);
    lv_label_set_text(screen0_label, "Scanning...");
    lv_obj_set_style_text_color(screen0_label, lv_color_hex(0x00FF88), 0);
    lv_obj_align(screen0_label, LV_ALIGN_CENTER, 0, 90);
    
    // Create circular ring with WORKING persistent points!
    circular_ring_config_t ring_config = circular_ring_config_default();
    ring_config.center_x = 233;
    ring_config.center_y = 233;
    ring_config.radius = 200;           // Outer edge at 200px from center (safe for 233px max)
    ring_config.tick_count = 60;
    ring_config.tick_length = 25;       // Ticks extend inward 25px
    ring_config.tick_width = 5;
    ring_config.active_color = lv_color_hex(0x00FF88);  // Cyan-green
    ring_config.inactive_color = lv_color_hex(0x444444); // Grey
    ring_config.inactive_opa = LV_OPA_80;
    ring_config.rounded_caps = true;
    
    screen0_ring = circular_ring_create(screen0, &ring_config);
    
    // Start 10-second animation
    if (screen0_ring) {
        circular_ring_start_placeholder_anim(screen0_ring);
    }
    
    return screen0;
}

#endif
