/*
 * Layer Manager - Declarative z-index control for LVGL components
 * Provides web-like layering system for UI components
 */

#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H

#include <lvgl.h>

/**
 * Layer indices (higher = on top, like CSS z-index)
 */
typedef enum {
    LAYER_BACKGROUND = 0,
    LAYER_RING = 10,
    LAYER_LOADER = 20,
    LAYER_IMAGE = 30,
    LAYER_TARGET = 40,
    LAYER_TEXT = 50,
    LAYER_OVERLAY = 100
} layer_index_t;

/**
 * Component definition with layer
 */
typedef struct {
    const char* name;           // Component name (for debugging)
    lv_obj_t* obj;             // LVGL object pointer
    layer_index_t layer;        // Desired layer index
} ui_component_t;

/**
 * Macro to define a UI component with layer
 */
#define UI_COMPONENT(component_name, component_obj, component_layer) \
    { .name = component_name, .obj = component_obj, .layer = component_layer }

/**
 * Apply layer order to components
 * Components are ordered by their position in the array (manual control)
 * Lower array index = background, higher array index = foreground
 * 
 * @param components Array of UI components
 * @param count Number of components
 */
static inline void apply_layer_order(ui_component_t* components, int count) {
    if (!components || count <= 0) {
        return;
    }
    
    // Apply layers in order: iterate from background to foreground
    for (int i = 0; i < count; i++) {
        if (components[i].obj != NULL) {
            // Move each component to its explicit index position
            // Index 0 = background, higher index = foreground
            lv_obj_move_to_index(components[i].obj, i);
            
            #ifdef LAYER_DEBUG
            LV_LOG_USER("Layer: %s at index %d (layer value: %d)", 
                        components[i].name, i, components[i].layer);
            #endif
        }
    }
}

/**
 * Debug helper: print current layer order
 */
static inline void debug_layer_order(ui_component_t* components, int count) {
    LV_LOG_USER("=== Layer Order (bottom to top) ===");
    for (int i = 0; i < count; i++) {
        if (components[i].obj != NULL) {
            int32_t actual_index = lv_obj_get_index(components[i].obj);
            LV_LOG_USER("  [%d] %s (layer: %d, actual_index: %d)", 
                        i, components[i].name, components[i].layer, (int)actual_index);
        }
    }
}

#endif // LAYER_MANAGER_H

