/*
 * Smart Screen Transition - Black overlay fade for OLED/AMOLED displays
 * Perfect for hiding color rendering issues with smooth black wipe
 */

#ifndef SMART_TRANSITION_H
#define SMART_TRANSITION_H

#include <lvgl.h>

// Overlay for transition
static lv_obj_t* black_overlay = NULL;
static lv_obj_t* target_screen = NULL;
static bool transition_in_progress = false;

/**
 * Callback when fade-out completes (overlay becomes transparent, revealing new screen)
 */
static void black_fade_out_complete(lv_anim_t* a) {
    if (black_overlay) {
        lv_obj_del(black_overlay);
        black_overlay = NULL;
    }
    transition_in_progress = false;
    target_screen = NULL;
}

/**
 * Callback when fade-in completes (screen fully black)
 * Now switch to new screen and fade out overlay
 */
static void black_fade_in_complete(lv_anim_t* a) {
    if (!target_screen) {
        transition_in_progress = false;
        return;
    }
    
    // CRITICAL: Wait a bit to ensure black overlay is fully rendered on display
    // This prevents the "white drop" artifact from display buffer refresh
    lv_timer_t* switch_timer = lv_timer_create([](lv_timer_t* timer) {
        lv_obj_t* new_screen = (lv_obj_t*)timer->user_data;
        
        // Switch to new screen while overlay is fully black (invisible switch!)
        #if LV_VERSION_CHECK(9, 0, 0)
        lv_screen_load(new_screen);
        #else
        lv_scr_load(new_screen);
        #endif
        
        // Move overlay to new screen
        lv_obj_set_parent(black_overlay, new_screen);
        lv_obj_move_foreground(black_overlay);
        
        // Fade OUT overlay to reveal new screen
        lv_anim_t anim_out;
        lv_anim_init(&anim_out);
        lv_anim_set_var(&anim_out, black_overlay);
        lv_anim_set_values(&anim_out, LV_OPA_COVER, LV_OPA_TRANSP);
        lv_anim_set_time(&anim_out, 200);  // Fast reveal
        lv_anim_set_exec_cb(&anim_out, (lv_anim_exec_xcb_t)lv_obj_set_style_bg_opa);
        lv_anim_set_path_cb(&anim_out, lv_anim_path_linear);  // Linear for solid fade
        #if LV_VERSION_CHECK(9, 0, 0)
        lv_anim_set_completed_cb(&anim_out, black_fade_out_complete);
        #else
        lv_anim_set_ready_cb(&anim_out, black_fade_out_complete);
        #endif
        lv_anim_start(&anim_out);
        
        // Delete the timer
        lv_timer_del(timer);
    }, 50, target_screen);  // 50ms delay to let display finish rendering black
    
    #if LV_VERSION_CHECK(9, 0, 0)
    lv_timer_set_repeat_count(switch_timer, 1);
    #else
    lv_timer_set_repeat_count(switch_timer, 1);
    #endif
}

/**
 * Black overlay fade transition - PERFECT for OLED displays!
 * 
 * How it works:
 * 1. Black overlay fades IN over current screen (200ms)
 * 2. Screen switches while fully black (invisible!)
 * 3. Black overlay fades OUT revealing new screen (200ms)
 * 
 * Total: 400ms smooth, elegant transition with NO color rendering issues!
 */
static inline void black_fade_transition(lv_obj_t* new_screen, uint32_t fade_in_ms, uint32_t fade_out_ms) {
    if (!new_screen || transition_in_progress) {
        return;
    }
    
    if (lv_scr_act() == new_screen) {
        return;
    }
    
    transition_in_progress = true;
    target_screen = new_screen;
    
    // Create full-screen black overlay on current screen (FULL SIZE!)
    black_overlay = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(black_overlay);
    
    // Set to FULL display size (466x466)
    lv_obj_set_size(black_overlay, 466, 466);
    lv_obj_set_pos(black_overlay, 0, 0);
    
    // Style as solid black
    lv_obj_set_style_bg_color(black_overlay, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(black_overlay, LV_OPA_TRANSP, 0);  // Start transparent
    lv_obj_set_style_border_width(black_overlay, 0, 0);
    lv_obj_set_style_pad_all(black_overlay, 0, 0);
    lv_obj_set_style_radius(black_overlay, 0, 0);  // No rounded corners
    
    // Disable all interactions
    lv_obj_clear_flag(black_overlay, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(black_overlay, LV_OBJ_FLAG_CLICKABLE);
    
    // Move to absolute foreground
    lv_obj_move_foreground(black_overlay);
    
    // Fade IN black overlay (cover current screen COMPLETELY)
    lv_anim_t anim_in;
    lv_anim_init(&anim_in);
    lv_anim_set_var(&anim_in, black_overlay);
    lv_anim_set_values(&anim_in, LV_OPA_TRANSP, LV_OPA_COVER);
    lv_anim_set_time(&anim_in, fade_in_ms);
    lv_anim_set_exec_cb(&anim_in, (lv_anim_exec_xcb_t)lv_obj_set_style_bg_opa);
    lv_anim_set_path_cb(&anim_in, lv_anim_path_linear);  // Linear for solid fade
    #if LV_VERSION_CHECK(9, 0, 0)
    lv_anim_set_completed_cb(&anim_in, black_fade_in_complete);
    #else
    lv_anim_set_ready_cb(&anim_in, black_fade_in_complete);
    #endif
    lv_anim_start(&anim_in);
}

/**
 * Fast black fade (150ms in, 150ms out = 300ms total)
 */
static inline void fast_black_fade_transition(lv_obj_t* new_screen) {
    black_fade_transition(new_screen, 150, 150);
}

/**
 * Smooth black fade (200ms in, 200ms out = 400ms total) - recommended!
 */
static inline void smooth_black_fade_transition(lv_obj_t* new_screen) {
    black_fade_transition(new_screen, 200, 200);
}

#endif // SMART_TRANSITION_H

