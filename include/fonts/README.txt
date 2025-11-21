# Custom Fonts Guide for UI Application

## ✅ FONTS ALREADY GENERATED!

**72 Stack Sans fonts** are ready to use in multiple sizes and weights!

### Quick Start - Using Existing Fonts

All fonts follow this naming pattern: `stack_sans_<weight>_<size>`

**Available Weights:** extralight, light, regular, medium, semibold, bold
**Available Sizes:** 12, 14, 16, 18, 20, 24, 28, 32, 40, 48, 64, 67

**Example Usage:**
```c
// Include the font
#include "../fonts/stack_sans_bold_32.c"

// Declare it
LV_FONT_DECLARE(stack_sans_bold_32);

// Use it
lv_obj_set_style_text_font(my_label, &stack_sans_bold_32, 0);
```

See `FONT_USAGE_EXAMPLE.h` for more examples!

---

## 🔄 Regenerating Fonts

If you need to regenerate fonts with different sizes or character ranges:

```bash
# From project root
python3 generate_fonts.py
```

Edit `generate_fonts.py` to customize:
- `FONT_SIZES` - Add/remove sizes
- `CHAR_RANGE` - Change character support
- `BPP` - Adjust quality (1-8)

---

## 🎨 Adding New Font Families

### Step 1: Prepare Your Font File
- Get a TTF (TrueType Font) file
- Place it in `Main fonts/static/` folder
- Example: `MyCustomFont.ttf`

### Step 2: Convert TTF to C Array

#### Option A: Using Command Line Tool (Fastest)
```bash
# Install tool (already installed)
npm install -g lv_font_conv

# Convert single font
lv_font_conv --no-compress \
  --bpp 4 \
  --size 24 \
  --font "Main fonts/static/MyCustomFont.ttf" \
  --range 0x20-0x7F,0xA0-0xFF \
  --format lvgl \
  --output examples/UI_Application/fonts/my_custom_font_24.c \
  --lv-font-name my_custom_font_24
```

#### Option B: Using LVGL Online Font Converter
1. Go to: https://lvgl.io/tools/fontconverter
2. Settings:
   - **Name**: `my_custom_font_24` (lowercase, no spaces)
   - **Size**: Your desired size (e.g., 24)
   - **Bpp**: `4 bit-per-pixel` (good quality, reasonable size)
   - **Range**: `0x20-0x7F,0xA0-0xFF` (English + accents)
   - **Font**: Upload your TTF file
3. Click "Convert"
4. Download the generated `.c` file
5. Save it as: `fonts/my_custom_font_24.c`

### Step 3: Add Font to Your Project

1. **Place the font file** in `examples/UI_Application/fonts/`

2. **Include in your screen/component:**
```c
#include "../fonts/my_custom_font_24.c"
```

3. **Use the font:**
```c
// Declare the font (from the .c file)
LV_FONT_DECLARE(my_custom_font_24);

// Use it on a label
lv_obj_t* label = lv_label_create(parent);
lv_label_set_text(label, "Hello Custom Font!");
lv_obj_set_style_text_font(label, &my_custom_font_24, 0);
```

### Step 4: Complete Example

**File: `fonts/roboto_bold_32.c`** (generated from converter)
```c
// This is auto-generated, don't edit manually
#include "lvgl.h"

const lv_font_t roboto_bold_32 = {
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,
    // ... more font data ...
};
```

**File: `screens/Screen1.h`** (using the font)
```c
#include "../fonts/roboto_bold_32.c"

LV_FONT_DECLARE(roboto_bold_32);

static inline lv_obj_t* screen1_create() {
    lv_obj_t* screen = lv_obj_create(NULL);
    
    lv_obj_t* title = lv_label_create(screen);
    lv_label_set_text(title, "My Custom Title");
    lv_obj_set_style_text_font(title, &roboto_bold_32, 0);
    
    return screen;
}
```

## Tips & Best Practices

### Font Size Recommendations
- **Titles**: 28-32px
- **Body text**: 14-18px
- **Small labels**: 12-14px
- **Large display**: 48-64px

### Memory Optimization
- ✅ Only include characters you need (use `--range`)
- ✅ Use smaller `bpp` for simple fonts (2 or 4)
- ✅ Share font sizes across screens when possible
- ❌ Don't include entire Unicode range if not needed

### Character Ranges
```
Basic Latin:        0x20-0x7F   (English, numbers, symbols)
Latin-1 Supplement: 0xA0-0xFF   (Accented characters)
Numbers only:       0x30-0x39   (0-9)
Uppercase only:     0x41-0x5A   (A-Z)
```

### Testing Your Font
1. Add font to `fonts/` folder
2. Include in a screen
3. Test in simulator: `./live_preview.py`
4. Verify text displays correctly
5. Flash to device

## Example: Adding Multiple Font Sizes

```c
// In fonts/ folder:
// - inter_regular_14.c
// - inter_regular_24.c
// - inter_bold_32.c

// In your screen:
#include "../fonts/inter_regular_14.c"
#include "../fonts/inter_regular_24.c"
#include "../fonts/inter_bold_32.c"

LV_FONT_DECLARE(inter_regular_14);
LV_FONT_DECLARE(inter_regular_24);
LV_FONT_DECLARE(inter_bold_32);

// Use them:
lv_obj_set_style_text_font(subtitle, &inter_regular_14, 0);
lv_obj_set_style_text_font(body, &inter_regular_24, 0);
lv_obj_set_style_text_font(title, &inter_bold_32, 0);
```

## Troubleshooting

**Problem: Font not displaying**
- Check if you used `LV_FONT_DECLARE()` before using the font
- Verify the font name matches the one in the `.c` file

**Problem: Missing characters**
- Expand the character range in the font converter
- Check if special characters are included

**Problem: Font too large (memory)**
- Reduce font size
- Lower bpp (use 2 or 1)
- Reduce character range

## Built-in LVGL Fonts (Always Available)

No conversion needed, use directly:
```c
&lv_font_montserrat_12
&lv_font_montserrat_14
&lv_font_montserrat_16
&lv_font_montserrat_18
&lv_font_montserrat_20
&lv_font_montserrat_24
&lv_font_montserrat_28
&lv_font_montserrat_32
&lv_font_montserrat_48
```

Place your custom fonts in this folder and follow the guide above!

