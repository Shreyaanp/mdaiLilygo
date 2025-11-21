/*
 * COMPLETE FONT REFERENCE TABLE
 * Stack Sans Text - All Available Fonts
 * 
 * Total: 72 fonts (6 weights × 12 sizes)
 * Character range: 0x20-0x7F, 0xA0-0xFF (English + Latin-1)
 * Quality: 4 BPP (good balance of size/quality)
 */

#ifndef FONT_REFERENCE_H
#define FONT_REFERENCE_H

/*
 * ╔══════════════════════════════════════════════════════════════════════════╗
 * ║                           FONT NAMING PATTERN                            ║
 * ╠══════════════════════════════════════════════════════════════════════════╣
 * ║  Format: stack_sans_<WEIGHT>_<SIZE>                                      ║
 * ║                                                                           ║
 * ║  Example: stack_sans_bold_32                                             ║
 * ║           └──────┬─────┘ └─┬─┘ └┬┘                                       ║
 * ║              font family  │   size (pixels)                              ║
 * ║                         weight                                           ║
 * ╚══════════════════════════════════════════════════════════════════════════╝
 */

/*
 * ┌──────────────────────────────────────────────────────────────────────────┐
 * │  WEIGHT GUIDE                                                            │
 * ├──────────────────────────────────────────────────────────────────────────┤
 * │  extralight  - Ultra thin, barely visible (display/decorative)           │
 * │  light       - Thin, elegant (captions, secondary text)                  │
 * │  regular     - Normal weight (body text, standard content)               │
 * │  medium      - Slightly emphasized (UI labels, important body)           │
 * │  semibold    - Strong emphasis (subheadings, highlights)                 │
 * │  bold        - Maximum weight (titles, headers, hero text)               │
 * └──────────────────────────────────────────────────────────────────────────┘
 */

/*
 * ┌──────────────────────────────────────────────────────────────────────────┐
 * │  SIZE GUIDE                                                              │
 * ├──────────────────────────────────────────────────────────────────────────┤
 * │  12, 14  - Fine print, very small labels                                 │
 * │  16, 18  - Small text, captions, secondary info                          │
 * │  20, 24  - Body text, standard UI elements                               │
 * │  28, 32  - Subheadings, emphasized text                                  │
 * │  40, 48  - Large titles, page headers                                    │
 * │  64, 67  - Hero text, large numbers (time, temperature)                  │
 * └──────────────────────────────────────────────────────────────────────────┘
 */

/*
 * ╔══════════════════════════════════════════════════════════════════════════╗
 * ║                        COMPLETE FONT TABLE                               ║
 * ╠════════╦═════════════════════════════════════════════════════════════════╣
 * ║ SIZE   ║ WEIGHTS (extralight, light, regular, medium, semibold, bold)   ║
 * ╠════════╬═════════════════════════════════════════════════════════════════╣
 * ║ 12     ║ _extralight_12, _light_12, _regular_12, _medium_12,            ║
 * ║        ║ _semibold_12, _bold_12                                          ║
 * ╠════════╬═════════════════════════════════════════════════════════════════╣
 * ║ 14     ║ _extralight_14, _light_14, _regular_14, _medium_14,            ║
 * ║        ║ _semibold_14, _bold_14                                          ║
 * ╠════════╬═════════════════════════════════════════════════════════════════╣
 * ║ 16     ║ _extralight_16, _light_16, _regular_16, _medium_16,            ║
 * ║        ║ _semibold_16, _bold_16                                          ║
 * ╠════════╬═════════════════════════════════════════════════════════════════╣
 * ║ 18     ║ _extralight_18, _light_18, _regular_18, _medium_18,            ║
 * ║        ║ _semibold_18, _bold_18                                          ║
 * ╠════════╬═════════════════════════════════════════════════════════════════╣
 * ║ 20     ║ _extralight_20, _light_20, _regular_20, _medium_20,            ║
 * ║        ║ _semibold_20, _bold_20                                          ║
 * ╠════════╬═════════════════════════════════════════════════════════════════╣
 * ║ 24     ║ _extralight_24, _light_24, _regular_24, _medium_24,            ║
 * ║        ║ _semibold_24, _bold_24                                          ║
 * ╠════════╬═════════════════════════════════════════════════════════════════╣
 * ║ 28     ║ _extralight_28, _light_28, _regular_28, _medium_28,            ║
 * ║        ║ _semibold_28, _bold_28                                          ║
 * ╠════════╬═════════════════════════════════════════════════════════════════╣
 * ║ 32     ║ _extralight_32, _light_32, _regular_32, _medium_32,            ║
 * ║        ║ _semibold_32, _bold_32                                          ║
 * ╠════════╬═════════════════════════════════════════════════════════════════╣
 * ║ 40     ║ _extralight_40, _light_40, _regular_40, _medium_40,            ║
 * ║        ║ _semibold_40, _bold_40                                          ║
 * ╠════════╬═════════════════════════════════════════════════════════════════╣
 * ║ 48     ║ _extralight_48, _light_48, _regular_48, _medium_48,            ║
 * ║        ║ _semibold_48, _bold_48                                          ║
 * ╠════════╬═════════════════════════════════════════════════════════════════╣
 * ║ 64     ║ _extralight_64, _light_64, _regular_64, _medium_64,            ║
 * ║        ║ _semibold_64, _bold_64                                          ║
 * ╠════════╬═════════════════════════════════════════════════════════════════╣
 * ║ 67     ║ _extralight_67, _light_67, _regular_67, _medium_67,            ║
 * ║        ║ _semibold_67, _bold_67                                          ║
 * ╚════════╩═════════════════════════════════════════════════════════════════╝
 */

/*
 * ╔══════════════════════════════════════════════════════════════════════════╗
 * ║                           USAGE EXAMPLES                                 ║
 * ╚══════════════════════════════════════════════════════════════════════════╝
 */

// Example 1: Large time display
// #include "../fonts/stack_sans_bold_67.c"
// LV_FONT_DECLARE(stack_sans_bold_67);
// lv_obj_set_style_text_font(time_label, &stack_sans_bold_67, 0);

// Example 2: Page title
// #include "../fonts/stack_sans_semibold_40.c"
// LV_FONT_DECLARE(stack_sans_semibold_40);
// lv_obj_set_style_text_font(title_label, &stack_sans_semibold_40, 0);

// Example 3: Body text
// #include "../fonts/stack_sans_regular_20.c"
// LV_FONT_DECLARE(stack_sans_regular_20);
// lv_obj_set_style_text_font(body_label, &stack_sans_regular_20, 0);

// Example 4: Small caption
// #include "../fonts/stack_sans_light_14.c"
// LV_FONT_DECLARE(stack_sans_light_14);
// lv_obj_set_style_text_font(caption_label, &stack_sans_light_14, 0);

/*
 * ╔══════════════════════════════════════════════════════════════════════════╗
 * ║                        MEMORY USAGE REFERENCE                            ║
 * ╠══════════════════════════════════════════════════════════════════════════╣
 * ║  Approximate file sizes (BPP=4, full Latin range):                      ║
 * ║                                                                           ║
 * ║  Size 12-14:  ~40-60 KB   per font                                       ║
 * ║  Size 16-18:  ~70-100 KB  per font                                       ║
 * ║  Size 20-24:  ~100-150 KB per font                                       ║
 * ║  Size 28-32:  ~200-260 KB per font                                       ║
 * ║  Size 40-48:  ~300-500 KB per font                                       ║
 * ║  Size 64-67:  ~700-900 KB per font                                       ║
 * ║                                                                           ║
 * ║  ⚠️  IMPORTANT: Only include fonts you actually use!                     ║
 * ║      Each included font increases your binary size.                     ║
 * ║                                                                           ║
 * ║  ✅  Good: Include 3-5 fonts for your UI                                 ║
 * ║  ❌  Bad:  Include all 72 fonts (unnecessary ~10+ MB)                    ║
 * ╚══════════════════════════════════════════════════════════════════════════╝
 */

/*
 * ╔══════════════════════════════════════════════════════════════════════════╗
 * ║                        TYPICAL UI FONT SET                               ║
 * ╠══════════════════════════════════════════════════════════════════════════╣
 * ║  For most UIs, you only need 3-5 fonts. Recommended starter set:        ║
 * ║                                                                           ║
 * ║  1. stack_sans_bold_67        - Hero numbers (time, temp)               ║
 * ║  2. stack_sans_semibold_32    - Page titles                             ║
 * ║  3. stack_sans_regular_20     - Body text                               ║
 * ║  4. stack_sans_medium_24      - Emphasized text                         ║
 * ║  5. stack_sans_light_14       - Captions/fine print                     ║
 * ║                                                                           ║
 * ║  Total: ~1.3 MB (reasonable for ESP32-S3)                               ║
 * ╚══════════════════════════════════════════════════════════════════════════╝
 */

/*
 * ╔══════════════════════════════════════════════════════════════════════════╗
 * ║                     REGENERATING/CUSTOMIZING FONTS                       ║
 * ╠══════════════════════════════════════════════════════════════════════════╣
 * ║  To regenerate with different settings:                                 ║
 * ║                                                                           ║
 * ║  1. Edit: /home/ichiro/lilygo1.43/generate_fonts.py                     ║
 * ║  2. Modify:                                                              ║
 * ║     - FONT_SIZES    (add/remove sizes)                                   ║
 * ║     - CHAR_RANGE    (change character support)                           ║
 * ║     - BPP           (1-8, quality vs size)                               ║
 * ║  3. Run: python3 generate_fonts.py                                       ║
 * ║                                                                           ║
 * ║  For single font conversion:                                             ║
 * ║  lv_font_conv --no-compress --bpp 4 --size 24 \                          ║
 * ║    --font "path/to/font.ttf" \                                           ║
 * ║    --range 0x20-0x7F,0xA0-0xFF \                                         ║
 * ║    --format lvgl \                                                       ║
 * ║    --output output_name.c \                                              ║
 * ║    --lv-font-name font_variable_name                                     ║
 * ╚══════════════════════════════════════════════════════════════════════════╝
 */

#endif // FONT_REFERENCE_H

