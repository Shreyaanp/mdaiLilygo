/*
 * Screen Transition Manager - Smooth animations for screen changes
 */

#ifndef SCREEN_TRANSITION_H
#define SCREEN_TRANSITION_H

#include <lvgl.h>

/**
 * Screen transition types
 */
typedef enum {
    TRANSITION_FADE = 0,        // Smooth fade (best for circular displays)
    TRANSITION_SLIDE_LEFT,      // Slide left
    TRANSITION_SLIDE_RIGHT,     // Slide right
    TRANSITION_NONE             // Instant (no animation)
} screen_transition_t;

/**
 * Transition configuration
 */
typedef struct {
    screen_transition_t type;
    uint32_t duration_ms;       // Animation duration (default: 500ms)
    uint32_t delay_ms;          // Delay before animation (default: 0ms)
} transition_config_t;

/**
 * Default transition configuration (optimized for smooth performance)
 */
static inline transition_config_t transition_config_default() {
    transition_config_t config = {
        .type = TRANSITION_FADE,
        .duration_ms = 350,      // 350ms - balanced speed and smoothness
        .delay_ms = 0
    };
    return config;
}

/**
 * Fast transition configuration (for quick navigation)
 */
static inline transition_config_t transition_config_fast() {
    transition_config_t config = {
        .type = TRANSITION_FADE,
        .duration_ms = 300,      // 300ms for quick transition
        .delay_ms = 0
    };
    return config;
}

/**
 * Slow transition configuration (for emphasis)
 */
static inline transition_config_t transition_config_slow() {
    transition_config_t config = {
        .type = TRANSITION_FADE,
        .duration_ms = 800,      // 800ms for dramatic effect
        .delay_ms = 0
    };
    return config;
}

/**
 * Load screen with smooth transition
 * @param screen LVGL screen object to load
 * @param config Transition configuration
 */
static inline void load_screen_with_transition(lv_obj_t* screen, transition_config_t config) {
    if (!screen) {
        return;
    }
    
    // Map transition type to LVGL animation
    lv_scr_load_anim_t lvgl_anim;
    
    switch (config.type) {
        case TRANSITION_SLIDE_LEFT:
            lvgl_anim = LV_SCR_LOAD_ANIM_MOVE_LEFT;
            break;
        case TRANSITION_SLIDE_RIGHT:
            lvgl_anim = LV_SCR_LOAD_ANIM_MOVE_RIGHT;
            break;
        case TRANSITION_NONE:
            lvgl_anim = LV_SCR_LOAD_ANIM_NONE;
            config.duration_ms = 0;
            break;
        case TRANSITION_FADE:
        default:
            #if LV_VERSION_CHECK(9, 0, 0)
            lvgl_anim = LV_SCR_LOAD_ANIM_FADE_IN;
            #else
            lvgl_anim = LV_SCR_LOAD_ANIM_FADE_ON;
            #endif
            break;
    }
    
    // Load with animation
    if (config.duration_ms == 0 && config.delay_ms == 0) {
        // Instant load (no animation)
        #if LV_VERSION_CHECK(9, 0, 0)
        lv_screen_load(screen);
        #else
        lv_scr_load(screen);
        #endif
    } else {
        // Animated load
        #if LV_VERSION_CHECK(9, 0, 0)
        lv_screen_load_anim(screen, lvgl_anim, config.duration_ms, config.delay_ms, false);
        #else
        lv_scr_load_anim(screen, lvgl_anim, config.duration_ms, config.delay_ms, false);
        #endif
    }
}

/**
 * Quick helper: Load screen with default smooth fade
 */
static inline void load_screen_smooth(lv_obj_t* screen) {
    load_screen_with_transition(screen, transition_config_default());
}

/**
 * Quick helper: Load screen instantly (no animation)
 */
static inline void load_screen_instant(lv_obj_t* screen) {
    transition_config_t config = {
        .type = TRANSITION_NONE,
        .duration_ms = 0,
        .delay_ms = 0
    };
    load_screen_with_transition(screen, config);
}

#endif // SCREEN_TRANSITION_H

