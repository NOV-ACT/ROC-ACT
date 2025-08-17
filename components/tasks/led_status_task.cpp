#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "led_strip.h"
#include "esp_log.h"
#include "include/led_status_codes.hpp"

// --- Yapılandırılabilir Parametreler ---
#define NEOPIXEL_PIN      GPIO_NUM_48 // ESP32-S3 dahili RGB LED pini
#define NEOPIXEL_COUNT    1           // LED sayısı

// --- Görev ve Kuyruk Tanımlamaları ---
QueueHandle_t led_state_queue;
static led_strip_handle_t led_strip;
static const char *TAG = "LED_TASK";

// --- Yardımcı Fonksiyonlar ---
void set_pixel_color(uint8_t r, uint8_t g, uint8_t b) {
    // led_strip_set_pixel, RMT'yi kullanarak LED rengini ayarlar.
    // Bu işlem arka planda DMA ile yapıldığı için CPU'yu meşgul etmez.
    led_strip_set_pixel(led_strip, 0, r, g, b);
    // led_strip_refresh, ayarlanan rengin LED'e gönderilmesini tetikler.
    led_strip_refresh(led_strip);
}

// --- Animasyon Fonksiyonları ---

/**
 * @brief LED'i yavaşça yakıp söndürerek "nefes alma" efekti oluşturur.
 * @param r Kırmızı renk bileşeni (0-255)
 * @param g Yeşil renk bileşeni (0-255)
 * @param b Mavi renk bileşeni (0-255)
 * @param duration Efektin toplam süresi (milisaniye)
 */
void breathing_effect(uint8_t r, uint8_t g, uint8_t b, int duration) {
    for (int i = 0; i < 256; i++) {
        float brightness = (float)i / 255.0;
        set_pixel_color(r * brightness, g * brightness, b * brightness);
        vTaskDelay(pdMS_TO_TICKS(duration / 512));
    }
    for (int i = 255; i >= 0; i--) {
        float brightness = (float)i / 255.0;
        set_pixel_color(r * brightness, g * brightness, b * brightness);
        vTaskDelay(pdMS_TO_TICKS(duration / 512));
    }
}

/**
 * @brief LED'i belirli bir renkte ve hızda yakıp söndürür.
 * @param r Kırmızı renk bileşeni (0-255)
 * @param g Yeşil renk bileşeni (0-255)
 * @param b Mavi renk bileşeni (0-255)
 * @param interval Yanıp sönme aralığı (milisaniye)
 */
void blinking_effect(uint8_t r, uint8_t g, uint8_t b, int interval) {
    set_pixel_color(r, g, b);
    vTaskDelay(pdMS_TO_TICKS(interval / 2));
    set_pixel_color(0, 0, 0); // LED'i kapat
    vTaskDelay(pdMS_TO_TICKS(interval / 2));
}

/**
 * @brief Dönen bir ışık animasyonu (chasing/wipe) oluşturur.
 * @note Tek LED olduğu için bu efekt, kısa süreli yanıp sönme olarak basitleştirilmiştir.
 */
void chasing_effect(uint8_t r, uint8_t g, uint8_t b) {
    set_pixel_color(r, g, b);
    vTaskDelay(pdMS_TO_TICKS(100));
    set_pixel_color(0, 0, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
}

// --- Ana LED Durum Görevi ---

/**
 * @brief `led_state_queue`'dan gelen durumları dinler ve ilgili LED animasyonunu çalıştırır.
 * @param pvParameters Görev parametreleri (kullanılmıyor).
 */
void led_status_task(void *pvParameters) {
    LedState current_state = LedState::BOOTING;
    LedState new_state;

    while (1) {
        if (xQueueReceive(led_state_queue, &new_state, pdMS_TO_TICKS(10)) == pdPASS) {
            current_state = new_state;
        }

        switch (current_state) {
            case LedState::BOOTING:
                breathing_effect(0, 0, 255, 2000); // Mavi
                break;
            case LedState::WIFI_CONNECTING:
                blinking_effect(255, 255, 0, 500); // Sarı
                break;
            case LedState::WIFI_CONNECTED:
                set_pixel_color(0, 255, 0); // Yeşil
                vTaskDelay(pdMS_TO_TICKS(100));
                break;
            case LedState::OPERATION_IN_PROGRESS:
                chasing_effect(0, 255, 255); // Cyan
                break;
            case LedState::ERROR:
                blinking_effect(255, 0, 0, 200); // Kırmızı
                break;
            case LedState::IDLE:
                set_pixel_color(20, 0, 20); // Mor (düşük parlaklık)
                vTaskDelay(pdMS_TO_TICKS(100));
                break;
            default:
                set_pixel_color(0, 0, 0);
                vTaskDelay(pdMS_TO_TICKS(100));
                break;
        }
    }
}

/**
 * @brief LED şeridini (strip) ve RMT kanalını yapılandırır.
 */
void configure_led() {
    ESP_LOGI(TAG, "Initializing LED strip");
    led_strip_config_t strip_config = {
        .strip_gpio_num = NEOPIXEL_PIN,
        .max_leds = NEOPIXEL_COUNT,
        .led_model = LED_MODEL_WS2812,
        .flags = {
            .invert_out = false,
        },
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags = {
            .with_dma = false,
        },
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    led_strip_clear(led_strip); // Başlangıçta LED'i kapat
}

/**
 * @brief LED durum görevini ve kuyruğunu başlatır.
 */
extern "C" void start_led_status_task() {
    led_state_queue = xQueueCreate(10, sizeof(LedState));
    configure_led();
    xTaskCreate(led_status_task, "led_status_task", 2048, NULL, 5, NULL);
}