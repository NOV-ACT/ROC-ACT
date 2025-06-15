#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Here is a good place to include header files that are also required by
the FreeRTOS API functions. */
#include <stdint.h>

/* Software timer definitions. */
#define configUSE_TIMERS                       1
#define configTIMER_TASK_PRIORITY              ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH               10
#define configTIMER_TASK_STACK_DEPTH           ( configMINIMAL_STACK_SIZE * 2 )

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                  0
#define configMAX_CO_ROUTINE_PRIORITIES        ( 2 )

/* Set the following definitions to 1 to include the API function, or 0 to exclude
the API function. */
#define INCLUDE_vTaskPrioritySet               1
#define INCLUDE_uxTaskPriorityGet              1
#define INCLUDE_vTaskDelete                    1
#define INCLUDE_vTaskCleanUpResources          0
#define INCLUDE_vTaskSuspend                   1
#define INCLUDE_vTaskDelayUntil                1
#define INCLUDE_vTaskDelay                     1
#define INCLUDE_xTaskGetSchedulerState         1
#define INCLUDE_xTimerPendFunctionCall         1
#define INCLUDE_xQueueGetMutexHolder           1
#define INCLUDE_uxTaskGetStackHighWaterMark    1
#define INCLUDE_xTaskGetCurrentTaskHandle      1
#define INCLUDE_eTaskGetState                  1

/* This is the most basic FreeRTOS configuration.
 * You will need to adjust these values based on your specific microcontroller
 * and application requirements.
 */
#define configUSE_PREEMPTION                   1
#define configUSE_IDLE_HOOK                    0
#define configUSE_TICK_HOOK                    0
#define configCPU_CLOCK_HZ                     ( ( unsigned long ) 240000000 ) // Example for ESP32
#define configTICK_RATE_HZ                     ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES                   ( 5 )
#define configMINIMAL_STACK_SIZE               ( ( unsigned short ) 128 )
#define configTOTAL_HEAP_SIZE                  ( ( size_t ) ( 64 * 1024 ) )
#define configMAX_TASK_NAME_LEN                ( 16 )
#define configUSE_TRACE_FACILITY               0
#define configUSE_16_BIT_TICKS                 0
#define configIDLE_SHOULD_YIELD                1
#define configUSE_MUTEXES                      1
#define configQUEUE_REGISTRY_SIZE              8
#define configCHECK_FOR_STACK_OVERFLOW         2 // 1 for simple check, 2 for more robust
#define configUSE_RECURSIVE_MUTEXES            1
#define configUSE_MALLOC_FAILED_HOOK           0
#define configUSE_APPLICATION_TASK_TAG         0
#define configUSE_COUNTING_SEMAPHORES          1
#define configGENERATE_RUN_TIME_STATS          0

/* Define to trap errors during development. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/* Optional: Define the port-specific includes if necessary */
// #define portFORCE_INLINE __attribute__((always_inline))

#endif /* FREERTOS_CONFIG_H */