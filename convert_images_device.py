#!/usr/bin/env python3
"""
Convert PNG images to LVGL C arrays for LVGL 8 device.
- RGB565 for opaque images (logo, face)
- RGB565A8 for transparent images (target icon)
For LilyGo 1.43" AMOLED display with LVGL 8.3.5
"""

from PIL import Image
import os
import sys

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565."""
    r5 = (r >> 3) & 0x1F  # 5 bits
    g6 = (g >> 2) & 0x3F  # 6 bits
    b5 = (b >> 3) & 0x1F  # 5 bits
    return (r5 << 11) | (g6 << 5) | b5

def has_transparency(img):
    """Check if image has meaningful transparency."""
    if img.mode != 'RGBA':
        return False
    alpha = img.split()[3]
    extrema = alpha.getextrema()
    return extrema[0] < 255  # Has some transparency

def convert_png_to_rgb565_c(png_path, output_path, var_name):
    """Convert PNG to RGB565 TRUE_COLOR format for LVGL 8."""
    
    # Load image
    img = Image.open(png_path)
    
    # Convert to RGB (composite on black background if has alpha)
    if img.mode != 'RGB':
        if img.mode == 'RGBA':
            background = Image.new('RGB', img.size, (0, 0, 0))  # Black background
            background.paste(img, mask=img.split()[3])
            img = background
        else:
            img = img.convert('RGB')
    
    width, height = img.size
    pixels = img.load()
    
    # Generate C file
    with open(output_path, 'w') as f:
        # Write header
        f.write(f"// Generated from {os.path.basename(png_path)}\n")
        f.write(f"// RGB565 TRUE_COLOR format for LVGL 8 device\n\n")
        f.write(f"#include <lvgl.h>\n\n")
        
        # Write pixel data array
        f.write(f"#ifndef LV_ATTRIBUTE_MEM_ALIGN\n")
        f.write(f"#define LV_ATTRIBUTE_MEM_ALIGN\n")
        f.write(f"#endif\n\n")
        
        f.write(f"#ifndef LV_ATTRIBUTE_IMG_{var_name.upper()}\n")
        f.write(f"#define LV_ATTRIBUTE_IMG_{var_name.upper()}\n")
        f.write(f"#endif\n\n")
        
        f.write(f"const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST ")
        f.write(f"LV_ATTRIBUTE_IMG_{var_name.upper()} uint8_t {var_name}_map[] = {{\n  ")
        
        byte_count = 0
        # RGB565: 2 bytes per pixel
        for y in range(height):
            for x in range(width):
                r, g, b = pixels[x, y]
                rgb565 = rgb888_to_rgb565(r, g, b)
                
                byte_low = rgb565 & 0xFF
                byte_high = (rgb565 >> 8) & 0xFF
                f.write(f"0x{byte_low:02x}, 0x{byte_high:02x}, ")
                byte_count += 2
                
                if byte_count % 16 == 0:
                    f.write("\n  ")
        
        f.write("\n};\n\n")
        
        # Write image descriptor
        total_pixels = width * height
        data_size = total_pixels * 2
        color_format = "LV_IMG_CF_TRUE_COLOR"
        
        f.write(f"const lv_img_dsc_t {var_name} = {{\n")
        f.write(f"  .header = {{\n")
        f.write(f"    .cf = {color_format},\n")
        f.write(f"    .always_zero = 0,\n")
        f.write(f"    .reserved = 0,\n")
        f.write(f"    .w = {width},\n")
        f.write(f"    .h = {height},\n")
        f.write(f"  }},\n")
        f.write(f"  .data_size = {data_size},\n")
        f.write(f"  .data = {var_name}_map,\n")
        f.write(f"}};\n")
    
    print(f"✓ Converted {png_path} -> {output_path}")
    print(f"  Size: {width}x{height}")
    print(f"  Format: RGB565 TRUE_COLOR")
    print(f"  Data size: {data_size} bytes ({data_size/1024:.1f} KB)")

def main():
    # Define conversions (no alpha, all opaque RGB565)
    conversions = [
        # (png_path, output_path, var_name)
        ("assets/logo.png", "include/components/logo.c", "logo"),
        ("assets/Frame 3965 (2).png", "include/components/frame_3965.c", "frame_3965"),
    ]
    
    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    print("Converting PNG images to LVGL 8 C arrays...")
    print("For LilyGo 1.43\" AMOLED with LVGL 8.3.5")
    print("=" * 60)
    
    for png_rel, c_rel, var_name in conversions:
        png_path = os.path.join(script_dir, png_rel)
        c_path = os.path.join(script_dir, c_rel)
        
        if not os.path.exists(png_path):
            print(f"⚠ Warning: {png_path} not found, skipping...")
            continue
        
        convert_png_to_rgb565_c(png_path, c_path, var_name)
    
    print("=" * 60)
    print("✓ Image conversion complete!")
    print("\nRebuild and flash: pio run -e lilygo_amoled_143 -t upload")

if __name__ == "__main__":
    main()
