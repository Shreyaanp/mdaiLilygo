/*
 * UI Application - Full Architecture
 * 10 screens with state management and serial communication
 */

#include <Arduino.h>
#include <lvgl.h>

// Arduino_GFX for 1.43" display
#include "Arduino_GFX_Library.h"
#include "pin_config.h"
#include "Arduino_DriveBus_Library.h"

// Application state
#include "state/AppState.h"
#include "state/SerialManager.h"

// Main UI controller (includes all screens)
#include "main.h"

// Display
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    LCD_CS, LCD_SCLK, LCD_SDIO0, LCD_SDIO1, LCD_SDIO2, LCD_SDIO3);

#if defined(DO0143FAT01)
Arduino_GFX *gfx = new Arduino_SH8601(bus, LCD_RST, 0, false, LCD_WIDTH, LCD_HEIGHT);
#elif defined(DO0143FMST10)
Arduino_GFX *gfx = new Arduino_CO5300(bus, LCD_RST, 0, false, LCD_WIDTH, LCD_HEIGHT, 6, 0, 0, 0);
#endif

// Touch
std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus = std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);
std::unique_ptr<Arduino_IIC> FT3168(new Arduino_CST816x(IIC_Bus, FT3168_DEVICE_ADDRESS, DRIVEBUS_DEFAULT_VALUE, TP_INT, nullptr));

// LVGL
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
bool IIC_Interrupt_Flag = false;

// Application managers
AppState* appState;
SerialManager* serialManager;

// LVGL callbacks
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
    lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    static bool was_pressed = false;
    
    if (IIC_Interrupt_Flag) {
        IIC_Interrupt_Flag = false;
        
        int32_t touch_x = FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
        int32_t touch_y = FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);
        uint8_t fingers = FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER);
        
        if (fingers > 0) {
            data->state = LV_INDEV_STATE_PR;
            data->point.x = touch_x;
            data->point.y = touch_y;
            was_pressed = true;
        } else {
            // Touch released - advance to next screen
            if (was_pressed) {
                ScreenID current = appState->getCurrentScreen();
                // Cycle through all screens 0-10
                if (current < SCREEN_10) {
                    appState->changeScreen((ScreenID)(current + 1));
                } else {
                    appState->changeScreen(SCREEN_0); // Loop back to start
                }
                was_pressed = false;
            }
            data->state = LV_INDEV_STATE_REL;
        }
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

void my_rounder_cb(lv_disp_drv_t *disp_drv, lv_area_t *area)
{
    uint16_t x1 = area->x1;
    uint16_t x2 = area->x2;
    area->x1 = (x1 >> 1) << 1;
    area->x2 = ((x2 >> 1) << 1) + 1;
    
    uint16_t h = area->y2 - area->y1 + 1;
    if (h % 2 != 0)
        area->y2 -= 1;
}

void Touch_Interrupt()
{
    IIC_Interrupt_Flag = true;
}

void lvgl_init()
{
    lv_init();
    
    // Use original safe buffer size (80 lines = 466*80 = 37,280 pixels per buffer)
    lv_color_t *buf_1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * LCD_WIDTH * 80, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    lv_color_t *buf_2 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * LCD_WIDTH * 80, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    
    while ((!buf_1) || (!buf_2)) {
        Serial.println("LVGL buffer allocation failed!");
        delay(1000);
    }
    
    lv_disp_draw_buf_init(&draw_buf, buf_1, buf_2, LCD_WIDTH * 80);
    
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.rounder_cb = my_rounder_cb;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 0;
    lv_disp_drv_register(&disp_drv);
    
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
    
    Serial.println("✅ LVGL initialized");
}

void setup()
{
    Serial.begin(115200);
    Serial.println("\n\n╔════════════════════════════════════════════╗");
    Serial.println("║   LilyGo UI Application Starting          ║");
    Serial.println("╚════════════════════════════════════════════╝\n");
    
    // Enable display power
    pinMode(LCD_EN, OUTPUT);
    digitalWrite(LCD_EN, HIGH);
    
    // Setup touch interrupt
    attachInterrupt(TP_INT, Touch_Interrupt, FALLING);
    
    // Initialize touch
    FT3168->begin();
    FT3168->IIC_Write_Device_State(FT3168->Arduino_IIC_Touch::Device::TOUCH_DEVICE_INTERRUPT_MODE,
                                    FT3168->Arduino_IIC_Touch::Device_Mode::TOUCH_DEVICE_INTERRUPT_PERIODIC);
    
    // Initialize display
    gfx->begin(120000000);
    gfx->fillScreen(BLACK);
    
    // Initialize LVGL
    lvgl_init();
    
    // Initialize application managers
    appState = AppState::getInstance();
    serialManager = new SerialManager();
    serialManager->begin(115200);
    
    // Initialize UI (creates all screens)
    init_ui();
    
    // Connect AppState screen change callback to use smooth transitions
    appState->setScreenChangeCallback([](ScreenID newScreen) {
        switch_to_screen((int)newScreen, true);  // Always use smooth animation
    });
    
    // Display initialization complete
    // Note: Arduino_GFX doesn't have brightness control for this display
    Serial.println("Display ready!");
    
    Serial.println("\n✅ Setup Complete!");
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.println("📱 10 Screens ready");
    Serial.println("💻 Send JSON: {\"screen\": 2} to switch screens");
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

void loop()
{
    // Update LVGL (high frequency for smooth animations)
    lv_timer_handler();
    
    // Update serial communication (handles {"screen": N} commands and nose tracking "X:###,Y:###")
    serialManager->update();
    
    // Update Screen 7 target position if on Screen 7
    if (appState->getCurrentScreen() == SCREEN_7) {
        screen7_update_position();  // Declared in Screen7.h
    }
    
    // Check if screen needs to change based on state
    static ScreenID lastScreen = SCREEN_1;
    ScreenID currentScreen = appState->getCurrentScreen();
    if (currentScreen != lastScreen) {
        switch_to_screen(currentScreen, true);  // true = with animation
        lastScreen = currentScreen;
    }
    
    // Update UI based on state changes
    update_ui();
    
    // Small delay to prevent task watchdog (1ms is negligible for 350ms animation)
    delay(1);
}
