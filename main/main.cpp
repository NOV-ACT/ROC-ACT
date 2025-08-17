#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "freertos/queue.h"
#include "include/led_status_codes.hpp"

// Forward declarations for tasks
extern "C" void imu_task(void* pvParameters);
extern "C" void baro_task(void* pvParameters);
extern "C" void state_task(void* pvParameters);
extern "C" void pyro_task(void* pvParameters);
extern "C" void logger_task(void* pvParameters);
extern "C" void telemetry_task(void* pvParameters);
extern "C" void start_led_status_task(); // LED görevini başlatan fonksiyon

// LED durum kuyruğuna dışarıdan erişim için
extern QueueHandle_t led_state_queue;

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // This function will be called if a task overflows its stack.
    // You can add custom error handling or logging here.
    printf("ERROR: Stack overflow in task: %s\n", pcTaskName);
    while(1); // Halt the system
}

extern "C" void app_main(void) {
    // LED durum görevini başlat
    start_led_status_task();

    // Diğer görevleri oluştur
    xTaskCreate(imu_task, "IMU_Task", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(baro_task, "BARO_Task", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(state_task, "STATE_Task", configMINIMAL_STACK_SIZE * 3, NULL, tskIDLE_PRIORITY + 3, NULL);
    xTaskCreate(pyro_task, "PYRO_Task", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(logger_task, "LOGGER_Task", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(telemetry_task, "TELEMETRY_Task", configMINIMAL_STACK_SIZE * 3, NULL, tskIDLE_PRIORITY + 1, NULL);

    // --- LED Görevi Test Kodu ---
    // Bu döngü, LED durum görevini test etmek için periyodik olarak kuyruğa farklı durumlar gönderir.
    LedState test_states[] = {
        LedState::BOOTING,
        LedState::WIFI_CONNECTING,
        LedState::WIFI_CONNECTED,
        LedState::OPERATION_IN_PROGRESS,
        LedState::IDLE,
        LedState::ERROR
    };
    int num_states = sizeof(test_states) / sizeof(LedState);
    int current_state_index = 0;

    while (1) {
        LedState state_to_send = test_states[current_state_index];
        printf("Sending state to LED task: %d\n", static_cast<int>(state_to_send));
        
        // Kuyruğa yeni durumu gönder
        if (led_state_queue != NULL) {
            xQueueSend(led_state_queue, &state_to_send, portMAX_DELAY);
        }

        // Bir sonraki duruma geç
        current_state_index = (current_state_index + 1) % num_states;

        // 5 saniye bekle
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}