/*
 * Animation Helpers - CSS-like animation and transition utilities
 * Features: transitions, keyframes, easing functions, transform animations
 */

#ifndef ANIMATION_HELPERS_H
#define ANIMATION_HELPERS_H

#include <lvgl.h>

// ============================================================================
// CSS-LIKE EASING FUNCTIONS
// ============================================================================

/**
 * Easing function types (like CSS transition-timing-function)
 */
typedef enum {
    EASE_LINEAR,
    EASE_IN,           // Slow start
    EASE_OUT,          // Slow end
    EASE_IN_OUT,       // Slow start and end
    EASE_IN_BACK,      // Overshoot at start
    EASE_OUT_BACK,     // Overshoot at end
    EASE_IN_OUT_BACK,  // Overshoot both ends
    EASE_BOUNCE        // Bounce at end
} ease_type_t;

/**
 * Get LVGL path callback for easing type
 */
static inline lv_anim_path_cb_t anim_get_easing(ease_type_t type) {
    switch (type) {
        case EASE_LINEAR:
            return lv_anim_path_linear;
        case EASE_IN:
            return lv_anim_path_ease_in;
        case EASE_OUT:
            return lv_anim_path_ease_out;
        case EASE_IN_OUT:
            return lv_anim_path_ease_in_out;
        case EASE_IN_BACK:
            return lv_anim_path_overshoot;
        case EASE_OUT_BACK:
            return lv_anim_path_ease_out;
        case EASE_IN_OUT_BACK:
            return lv_anim_path_ease_in_out;
        case EASE_BOUNCE:
            return lv_anim_path_bounce;
        default:
            return lv_anim_path_linear;
    }
}

// ============================================================================
// TRANSITION HELPERS (like CSS transition property)
// ============================================================================

/**
 * Transition configuration (like CSS transition shorthand)
 */
typedef struct {
    uint32_t duration;        // Duration in ms
    uint32_t delay;           // Delay in ms
    ease_type_t easing;       // Easing function
    lv_anim_ready_cb_t ready_cb;  // Callback when done
    void* user_data;          // User data for callback
} transition_t;

/**
 * Create default transition (300ms, ease-in-out, no delay)
 */
static inline transition_t transition_default() {
    transition_t trans = {0};
    trans.duration = 300;
    trans.delay = 0;
    trans.easing = EASE_IN_OUT;
    trans.ready_cb = NULL;
    trans.user_data = NULL;
    return trans;
}

// ============================================================================
// ENHANCED ANIMATION FUNCTIONS
// ============================================================================

/**
 * Fade in animation
 * @param obj Object to animate
 * @param duration Duration in milliseconds
 * @param delay Delay before starting (ms)
 */
static inline void anim_fade_in(lv_obj_t* obj, uint32_t duration, uint32_t delay) {
    lv_obj_set_style_opa(obj, LV_OPA_TRANSP, 0);
    
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, LV_OPA_TRANSP, LV_OPA_COVER);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_delay(&anim, delay);
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
    lv_anim_start(&anim);
}

/**
 * Fade in with transition config
 */
static inline void anim_fade_in_trans(lv_obj_t* obj, transition_t* trans) {
    lv_obj_set_style_opa(obj, LV_OPA_TRANSP, 0);
    
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, LV_OPA_TRANSP, LV_OPA_COVER);
    lv_anim_set_time(&anim, trans->duration);
    lv_anim_set_delay(&anim, trans->delay);
    lv_anim_set_path_cb(&anim, anim_get_easing(trans->easing));
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
    if (trans->ready_cb) lv_anim_set_ready_cb(&anim, trans->ready_cb);
    if (trans->user_data) lv_anim_set_user_data(&anim, trans->user_data);
    lv_anim_start(&anim);
}

/**
 * Fade out animation
 * @param obj Object to animate
 * @param duration Duration in milliseconds
 * @param delay Delay before starting (ms)
 */
static inline void anim_fade_out(lv_obj_t* obj, uint32_t duration, uint32_t delay) {
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, LV_OPA_COVER, LV_OPA_TRANSP);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_delay(&anim, delay);
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
    lv_anim_start(&anim);
}

/**
 * Slide in from bottom animation
 * @param obj Object to animate
 * @param duration Duration in milliseconds
 * @param delay Delay before starting (ms)
 */
static inline void anim_slide_in_bottom(lv_obj_t* obj, uint32_t duration, uint32_t delay) {
    int16_t final_y = lv_obj_get_y(obj);
    lv_obj_set_y(obj, 466);  // Start off-screen
    
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, 466, final_y);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_delay(&anim, delay);
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);
    lv_anim_start(&anim);
}

/**
 * Rotate animation (continuous)
 * @param obj Object to animate
 * @param duration Duration for one full rotation (ms)
 * @param clockwise true for clockwise, false for counter-clockwise
 */
static inline void anim_rotate_continuous(lv_obj_t* obj, uint32_t duration, bool clockwise) {
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, 0, clockwise ? 3600 : -3600);  // 360.0 degrees (LVGL uses 0.1 degree units)
    lv_anim_set_time(&anim, duration);
    lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_transform_angle);
    lv_anim_start(&anim);
}

/**
 * Scale pulse animation (heartbeat effect)
 * @param obj Object to animate
 * @param duration Duration of one pulse (ms)
 * @param scale_percent Scale percentage (e.g., 110 for 110%)
 */
static inline void anim_pulse(lv_obj_t* obj, uint32_t duration, int16_t scale_percent) {
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, 256, (256 * scale_percent) / 100);  // LVGL uses 256 as 100%
    lv_anim_set_time(&anim, duration);
    lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_playback_time(&anim, duration);
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_transform_zoom);
    lv_anim_start(&anim);
}

/**
 * Blink animation (visibility toggle)
 * @param obj Object to animate
 * @param on_time Time visible (ms)
 * @param off_time Time hidden (ms)
 */
static inline void anim_blink(lv_obj_t* obj, uint32_t on_time, uint32_t off_time) {
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, LV_OPA_COVER, LV_OPA_TRANSP);
    lv_anim_set_time(&anim, on_time);
    lv_anim_set_playback_time(&anim, off_time);
    lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
    lv_anim_start(&anim);
}

// ============================================================================
// CSS TRANSFORM-LIKE ANIMATIONS
// ============================================================================

/**
 * Translate (move) animation with easing
 * @param obj Object to animate
 * @param from_x Starting X offset
 * @param to_x Ending X offset
 * @param from_y Starting Y offset
 * @param to_y Ending Y offset
 * @param trans Transition config
 */
static inline void anim_translate(lv_obj_t* obj, int16_t from_x, int16_t to_x,
                                   int16_t from_y, int16_t to_y, transition_t* trans) {
    // X animation
    if (from_x != to_x) {
        lv_obj_set_x(obj, from_x);
        lv_anim_t anim_x;
        lv_anim_init(&anim_x);
        lv_anim_set_var(&anim_x, obj);
        lv_anim_set_values(&anim_x, from_x, to_x);
        lv_anim_set_time(&anim_x, trans->duration);
        lv_anim_set_delay(&anim_x, trans->delay);
        lv_anim_set_path_cb(&anim_x, anim_get_easing(trans->easing));
        lv_anim_set_exec_cb(&anim_x, (lv_anim_exec_xcb_t)lv_obj_set_x);
        lv_anim_start(&anim_x);
    }
    
    // Y animation
    if (from_y != to_y) {
        lv_obj_set_y(obj, from_y);
        lv_anim_t anim_y;
        lv_anim_init(&anim_y);
        lv_anim_set_var(&anim_y, obj);
        lv_anim_set_values(&anim_y, from_y, to_y);
        lv_anim_set_time(&anim_y, trans->duration);
        lv_anim_set_delay(&anim_y, trans->delay);
        lv_anim_set_path_cb(&anim_y, anim_get_easing(trans->easing));
        lv_anim_set_exec_cb(&anim_y, (lv_anim_exec_xcb_t)lv_obj_set_y);
        if (trans->ready_cb) lv_anim_set_ready_cb(&anim_y, trans->ready_cb);
        lv_anim_start(&anim_y);
    }
}

/**
 * Scale animation (like CSS transform: scale())
 * @param obj Object to animate
 * @param from_scale Starting scale (256 = 100%)
 * @param to_scale Ending scale (256 = 100%)
 * @param trans Transition config
 */
static inline void anim_scale(lv_obj_t* obj, int16_t from_scale, int16_t to_scale, 
                               transition_t* trans) {
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, from_scale, to_scale);
    lv_anim_set_time(&anim, trans->duration);
    lv_anim_set_delay(&anim, trans->delay);
    lv_anim_set_path_cb(&anim, anim_get_easing(trans->easing));
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_transform_zoom);
    if (trans->ready_cb) lv_anim_set_ready_cb(&anim, trans->ready_cb);
    lv_anim_start(&anim);
}

/**
 * Rotate animation (like CSS transform: rotate())
 * @param obj Object to animate
 * @param from_angle Starting angle in 0.1 degrees (0-3600)
 * @param to_angle Ending angle in 0.1 degrees
 * @param trans Transition config
 */
static inline void anim_rotate(lv_obj_t* obj, int16_t from_angle, int16_t to_angle,
                                transition_t* trans) {
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, from_angle, to_angle);
    lv_anim_set_time(&anim, trans->duration);
    lv_anim_set_delay(&anim, trans->delay);
    lv_anim_set_path_cb(&anim, anim_get_easing(trans->easing));
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_transform_angle);
    if (trans->ready_cb) lv_anim_set_ready_cb(&anim, trans->ready_cb);
    lv_anim_start(&anim);
}

/**
 * Width animation (responsive resize)
 * @param obj Object to animate
 * @param from_width Starting width
 * @param to_width Ending width
 * @param trans Transition config
 */
static inline void anim_width(lv_obj_t* obj, int16_t from_width, int16_t to_width,
                               transition_t* trans) {
    lv_obj_set_width(obj, from_width);
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, from_width, to_width);
    lv_anim_set_time(&anim, trans->duration);
    lv_anim_set_delay(&anim, trans->delay);
    lv_anim_set_path_cb(&anim, anim_get_easing(trans->easing));
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_width);
    if (trans->ready_cb) lv_anim_set_ready_cb(&anim, trans->ready_cb);
    lv_anim_start(&anim);
}

/**
 * Height animation (responsive resize)
 * @param obj Object to animate
 * @param from_height Starting height
 * @param to_height Ending height
 * @param trans Transition config
 */
static inline void anim_height(lv_obj_t* obj, int16_t from_height, int16_t to_height,
                                transition_t* trans) {
    lv_obj_set_height(obj, from_height);
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, from_height, to_height);
    lv_anim_set_time(&anim, trans->duration);
    lv_anim_set_delay(&anim, trans->delay);
    lv_anim_set_path_cb(&anim, anim_get_easing(trans->easing));
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_height);
    if (trans->ready_cb) lv_anim_set_ready_cb(&anim, trans->ready_cb);
    lv_anim_start(&anim);
}

// ============================================================================
// ANIMATION SEQUENCES & COMBOS (like CSS @keyframes)
// ============================================================================

/**
 * Slide and fade in combo (common pattern)
 * @param obj Object to animate
 * @param from_y Starting Y offset
 * @param to_y Ending Y position
 * @param trans Transition config
 */
static inline void anim_slide_fade_in(lv_obj_t* obj, int16_t from_y, int16_t to_y,
                                       transition_t* trans) {
    lv_obj_set_y(obj, from_y);
    lv_obj_set_style_opa(obj, LV_OPA_TRANSP, 0);
    
    // Slide animation
    lv_anim_t anim_y;
    lv_anim_init(&anim_y);
    lv_anim_set_var(&anim_y, obj);
    lv_anim_set_values(&anim_y, from_y, to_y);
    lv_anim_set_time(&anim_y, trans->duration);
    lv_anim_set_delay(&anim_y, trans->delay);
    lv_anim_set_path_cb(&anim_y, anim_get_easing(trans->easing));
    lv_anim_set_exec_cb(&anim_y, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_start(&anim_y);
    
    // Fade animation
    lv_anim_t anim_opa;
    lv_anim_init(&anim_opa);
    lv_anim_set_var(&anim_opa, obj);
    lv_anim_set_values(&anim_opa, LV_OPA_TRANSP, LV_OPA_COVER);
    lv_anim_set_time(&anim_opa, trans->duration);
    lv_anim_set_delay(&anim_opa, trans->delay);
    lv_anim_set_path_cb(&anim_opa, anim_get_easing(trans->easing));
    lv_anim_set_exec_cb(&anim_opa, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
    if (trans->ready_cb) lv_anim_set_ready_cb(&anim_opa, trans->ready_cb);
    lv_anim_start(&anim_opa);
}

/**
 * Scale and fade in combo (zoom effect)
 * @param obj Object to animate
 * @param from_scale Starting scale (256 = 100%)
 * @param trans Transition config
 */
static inline void anim_zoom_in(lv_obj_t* obj, int16_t from_scale, transition_t* trans) {
    lv_obj_set_style_opa(obj, LV_OPA_TRANSP, 0);
    
    // Scale animation
    lv_anim_t anim_scale;
    lv_anim_init(&anim_scale);
    lv_anim_set_var(&anim_scale, obj);
    lv_anim_set_values(&anim_scale, from_scale, 256);
    lv_anim_set_time(&anim_scale, trans->duration);
    lv_anim_set_delay(&anim_scale, trans->delay);
    lv_anim_set_path_cb(&anim_scale, anim_get_easing(trans->easing));
    lv_anim_set_exec_cb(&anim_scale, (lv_anim_exec_xcb_t)lv_obj_set_style_transform_zoom);
    lv_anim_start(&anim_scale);
    
    // Fade animation
    lv_anim_t anim_opa;
    lv_anim_init(&anim_opa);
    lv_anim_set_var(&anim_opa, obj);
    lv_anim_set_values(&anim_opa, LV_OPA_TRANSP, LV_OPA_COVER);
    lv_anim_set_time(&anim_opa, trans->duration);
    lv_anim_set_delay(&anim_opa, trans->delay);
    lv_anim_set_path_cb(&anim_opa, anim_get_easing(trans->easing));
    lv_anim_set_exec_cb(&anim_opa, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
    if (trans->ready_cb) lv_anim_set_ready_cb(&anim_opa, trans->ready_cb);
    lv_anim_start(&anim_opa);
}

/**
 * Stop all animations on an object
 * @param obj Object to stop animations on
 */
static inline void anim_stop_all(lv_obj_t* obj) {
    lv_anim_del(obj, NULL);
}

#endif // ANIMATION_HELPERS_H

