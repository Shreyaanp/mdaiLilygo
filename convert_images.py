#!/usr/bin/env python3
"""
Convert PNG images to LVGL C arrays in ARGB8888 format for simulator compatibility.
"""

from PIL import Image
import os
import sys

def convert_png_to_argb8888_c(png_path, output_path, var_name):
    """Convert PNG to ARGB8888 format C array."""
    
    # Load image
    img = Image.open(png_path)
    
    # Convert to RGBA if not already
    if img.mode != 'RGBA':
        img = img.convert('RGBA')
    
    width, height = img.size
    pixels = img.load()
    
    # Generate C file
    with open(output_path, 'w') as f:
        # Write header
        f.write(f"// Generated from {os.path.basename(png_path)}\n")
        f.write(f"// ARGB8888 format for LVGL 9 simulator\n\n")
        f.write(f"#include \"lvgl.h\"\n\n")
        
        # Write pixel data array
        f.write(f"#ifndef LV_ATTRIBUTE_MEM_ALIGN\n")
        f.write(f"#define LV_ATTRIBUTE_MEM_ALIGN\n")
        f.write(f"#endif\n\n")
        
        f.write(f"#ifndef LV_ATTRIBUTE_IMAGE_{var_name.upper()}\n")
        f.write(f"#define LV_ATTRIBUTE_IMAGE_{var_name.upper()}\n")
        f.write(f"#endif\n\n")
        
        f.write(f"const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST ")
        f.write(f"LV_ATTRIBUTE_IMAGE_{var_name.upper()} uint8_t {var_name}_map[] = {{\n  ")
        
        # Write ARGB8888 data as 32-bit words (little-endian: BGRA byte order in memory)
        # LVGL expects native 32-bit format: 0xAARRGGBB as uint32_t
        # In little-endian memory: BB GG RR AA
        byte_count = 0
        for y in range(height):
            for x in range(width):
                r, g, b, a = pixels[x, y]
                
                # Write as BGRA in little-endian byte order (B, G, R, A)
                # This represents 0xAARRGGBB in a 32-bit word
                f.write(f"0x{b:02x}, 0x{g:02x}, 0x{r:02x}, 0x{a:02x}, ")
                byte_count += 4
                
                # Line break every 16 bytes (4 pixels)
                if byte_count % 16 == 0:
                    f.write("\n  ")
        
        f.write("\n};\n\n")
        
        # Write image descriptor
        total_pixels = width * height
        data_size = total_pixels * 4  # 4 bytes per pixel
        
        f.write(f"const lv_img_dsc_t {var_name} = {{\n")
        f.write(f"  .header = {{\n")
        f.write(f"    .magic = LV_IMAGE_HEADER_MAGIC,\n")
        f.write(f"    .cf = LV_COLOR_FORMAT_ARGB8888,\n")
        f.write(f"    .flags = 0,\n")
        f.write(f"    .w = {width},\n")
        f.write(f"    .h = {height},\n")
        f.write(f"    .stride = {width * 4},\n")
        f.write(f"    .reserved_2 = 0\n")
        f.write(f"  }},\n")
        f.write(f"  .data_size = {data_size},\n")
        f.write(f"  .data = {var_name}_map,\n")
        f.write(f"}};\n")
    
    print(f"✓ Converted {png_path} -> {output_path}")
    print(f"  Size: {width}x{height}, {data_size} bytes ({data_size/1024:.1f} KB)")

def main():
    # Define conversions
    conversions = [
        ("assets/logo.png", "components/logo.c", "logo"),
        ("assets/Frame 3965 (2).png", "components/frame_3965.c", "frame_3965"),
        ("assets/target.png", "components/target.c", "target"),
    ]
    
    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    print("Converting PNG images to ARGB8888 C arrays...")
    print("=" * 60)
    
    for png_rel, c_rel, var_name in conversions:
        png_path = os.path.join(script_dir, png_rel)
        c_path = os.path.join(script_dir, c_rel)
        
        if not os.path.exists(png_path):
            print(f"⚠ Warning: {png_path} not found, skipping...")
            continue
        
        convert_png_to_argb8888_c(png_path, c_path, var_name)
    
    print("=" * 60)
    print("✓ Image conversion complete!")
    print("\nNow rebuild the simulator:")
    print("  cd simulator/build && make")

if __name__ == "__main__":
    main()

