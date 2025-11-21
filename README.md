# UI_Application - Multi-Device Standalone Project

Complete, self-contained UI application for **multiple LilyGo AMOLED displays**.

## 🎯 Supported Devices

| Device | Resolution | Shape | Status |
|--------|-----------|-------|--------|
| **T-Display S3 AMOLED 1.43"** | 466×466 | ● Circular | ✅ Tested |
| **T-Display S3 AMOLED 1.75"** | 466×466 | ● Circular | ✅ Compatible |
| **T-Display AMOLED 2.41"** | 450×600 | ▭ Rectangular | ✅ Ready |

## 🚀 Quick Start

### 1. Select Your Device

Edit `platformio.ini` line 13:

```ini
[platformio]
default_envs = lilygo_amoled_143  # For 1.43" circular
# default_envs = lilygo_amoled_175  # For 1.75" circular
# default_envs = lilygo_amoled_241  # For 2.41" rectangular
```

### 2. Build & Upload

```bash
pio run              # Build
pio run -t upload    # Upload to device
pio device monitor   # Serial monitor
```

## 📦 Project Structure

```
UI_Application/
├── platformio.ini        # Multi-device configuration
├── boards/               # Board definitions
│   ├── esp32s3_flash_*.json
│   └── T-Display-AMOLED.json
├── lib/                  # All required libraries (self-contained!)
│   ├── lvgl-8.3.5/
│   ├── Arduino_GFX-1.3.7/    # For 1.43" & 1.75"
│   ├── LilyGo_AMOLED/        # For 2.41"
│   ├── XPowersLib/
│   ├── TFT_eSPI/
│   └── ... (all dependencies)
├── src/
│   ├── main.cpp
│   └── HardwareAbstraction.cpp
├── include/
│   ├── HardwareAbstraction.h  # Hardware abstraction layer
│   ├── main.h
│   ├── components/
│   ├── screens/
│   ├── state/
│   └── utils/
└── simulator/            # LVGL simulator
    └── bin/main          # Run ./simulator/bin/main
```

## 🎨 Features

### CircularRing Component
Face-scanning style progress ring with:
- Customizable tick count, thickness, colors
- Percentage-based control (0-100%)
- LVGL 8 & 9 compatible
- Works on both circular and rectangular displays

### HTML5/CSS-Like UI Framework
- **Box Model**: padding, margin, border, background, border-radius
- **Flexbox**: `flex_direction`, `justify-content`, `align-items`
- **CSS Grid**: `grid-template-columns`, `grid-gap`
- **CSS Animations**: easing functions, transitions, transforms
- **Responsive**: Viewport zones, percentage-based sizing

### State Management
- Screen navigation
- Serial communication for remote control
- Button handling (hardware + touch)

### 11 Demo Screens
- Screen 0: CircularRing scanning animation
- Screen 1: Logo display
- Screen 2-3: Loaders & animations  
- Screen 4: Info cards
- Screen 5: Face frame display
- Screen 6: Colored buttons
- Screen 7: Crosshair tracker
- Screen 8: Grid layout
- Screen 9: Text & animations
- Screen 10: Final demo

## 🔧 Hardware Abstraction

The project uses a **hardware abstraction layer** (`HardwareAbstraction.h`) to support multiple devices:

```cpp
// Device-agnostic functions
bool hardware_init();
void display_set_brightness(uint8_t brightness);
const char* get_device_name();
bool is_circular_display();
```

### Device-Specific Drivers

| Device | Display Driver | Touch Driver | Power Management |
|--------|---------------|--------------|------------------|
| 1.43" / 1.75" | Arduino_GFX (SH8601/CO5300) | CSTXXX | N/A |
| 2.41" | LilyGo_AMOLED | Built-in | XPowersLib (BQ25896) |

## 🧪 Testing

### Physical Device
1. Connect device via USB
2. Check connection: `pio device list`
3. Upload: `pio run -t upload`
4. Navigate screens using hardware buttons

### Simulator (LVGL 9)
```bash
cd simulator/build
cmake .. && make
../bin/main
```

The simulator shows **exactly** what's rendered on the device, including:
- All 11 screens
- CircularRing animations
- Touch interactions (click with mouse)
- Navigation buttons

## 💾 Memory Usage

| Device | RAM | Flash |
|--------|-----|-------|
| 1.43" | 8.4% (27KB) | 27.4% (1.8MB) |
| 2.41" | ~10% (estimated) | ~30% (estimated) |

## 🔄 Version Compatibility

- **LVGL 8.3.5-8.4.0**: Device firmware
- **LVGL 9.x**: Simulator
- **ESP32 Arduino Core**: 2.0.16
- **PlatformIO**: 6.7.0+

All components use conditional compilation for dual LVGL 8/9 compatibility:

```cpp
#if LV_VERSION_CHECK(9, 0, 0)
    // LVGL 9 code
#else
    // LVGL 8 code
#endif
```

## 📝 Adding New Devices

1. Add board definition to `boards/`
2. Copy required libraries to `lib/`
3. Add environment to `platformio.ini`
4. Update `HardwareAbstraction.cpp` with device-specific code
5. Test build: `pio run -e your_new_device`

## 🐛 Troubleshooting

**Build fails:**
- Check all libraries are in `lib/` folder
- Verify `boards_dir = boards` in platformio.ini

**Upload fails:**
- Check USB connection: `ls /dev/ttyACM*` or `ls /dev/ttyUSB*`
- Try lowering upload speed: `upload_speed = 460800`

**Display shows nothing:**
- Verify correct device environment selected
- Check `#define` in pin_config.h matches your hardware

**Simulator won't build:**
- Install SDL2: `sudo apt install libsdl2-dev`
- Clean build: `rm -rf simulator/build && mkdir simulator/build`

## 📚 Documentation

- `SETUP.md`: Quick setup guide
- `DISPLAY_METHODS_GUIDE.md`: Display function reference
- `GIF_SELECTION_GUIDE.txt`: Image format guide

## 🆘 Support

For device-specific issues:
- **1.43" / 1.75"**: Check Arduino_GFX and pin_config.h
- **2.41"**: Check LilyGo_AMOLED library and board initialization

---

**Built with ❤️ for LilyGo AMOLED Displays**

**Status**: ✅ Production-ready for all supported devices!
