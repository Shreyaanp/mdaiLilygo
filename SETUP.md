# Quick Setup Guide

## ✅ What's Included

This folder is a **complete, standalone PlatformIO project**. Everything you need is here:

- ✅ Source code (`src/`, `include/`)
- ✅ All required libraries (`lib/`)
- ✅ Board definitions (`boards/`)
- ✅ Build configuration (`platformio.ini`)
- ✅ Partition table (`default_16MB.csv`)

## 🚀 Getting Started

### 1. Install PlatformIO

**Option A: VS Code Extension**
1. Install Visual Studio Code
2. Install "PlatformIO IDE" extension
3. Reload VS Code

**Option B: Command Line**
```bash
pip install platformio
```

### 2. Open Project

**VS Code:**
- File → Open Folder → Select `UI_Application`

**Command Line:**
```bash
cd UI_Application
```

### 3. Select Your Display

Edit `platformio.ini` line 6:
```ini
default_envs = lilygo_amoled_143  # For 1.43" display
# OR
default_envs = lilygo_amoled_175  # For 1.75" display
```

### 4. Build & Upload

**VS Code:**
- Click "Build" (✓) button in PlatformIO toolbar
- Click "Upload" (→) button to flash device

**Command Line:**
```bash
pio run              # Build only
pio run -t upload    # Build and upload
pio device monitor   # Serial monitor
```

## 📊 Build Status

**Tested:** ✅ Compiles successfully for LVGL 8 (device)
**Target:** ESP32-S3 with 16MB Flash
**Frameworks:** Arduino + LVGL 8.3.5

## 🔧 Troubleshooting

**"Environment not found" error:**
- Check `platformio.ini` environment name (no dots allowed: use `_` not `.` or `-`)

**Display shows nothing:**
- Verify `DO0143FAT01` or `DO0143FMST10` in `pin_config.h`
- Check display controller type

**Build takes too long:**
- First build compiles all libraries (~2-3 minutes)
- Subsequent builds are much faster (~10 seconds)

## 📁 Project Structure

```
UI_Application/
├── platformio.ini        ← Build config
├── src/main.cpp          ← Entry point
├── include/              ← Headers & screens
│   ├── main.h
│   ├── components/
│   ├── screens/
│   ├── state/
│   └── utils/
└── lib/                  ← All dependencies (self-contained!)
    ├── lvgl-8.3.5/
    ├── Arduino_GFX-1.3.7/
    ├── SensorLib-20240907/
    ├── Arduino_DriveBus/
    ├── Mylibrary/
    └── private_library/
```

## 🎯 Next Steps

1. **Test on simulator** (if you have LVGL 9 simulator setup)
2. **Upload to device** and verify all screens work
3. **Customize screens** in `include/screens/`
4. **Add new components** in `include/components/`

## 🆘 Need Help?

- Check `README.md` for full documentation
- Verify all files in `lib/` folder exist
- Ensure USB cable supports data (not charge-only)

---

**This project is ready to build and run!** 🎉


