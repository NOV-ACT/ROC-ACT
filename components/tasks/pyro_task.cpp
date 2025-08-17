#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "drivers/PyroDriver.hpp"
#include "mreq/mreq.hpp"
#include "topic_registry_autogen.hpp"
#include "pyro_command.pb.h" // For PyroCommand message

using namespace mreq::autogen;

extern "C" void pyro_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    novact::drivers::PyroDriver pyroDriver;

    if (!pyroDriver.initialize()) {
        printf("Pyro Task: Failed to initialize Pyro Driver!\n");
        vTaskDelete(NULL); // Delete task if initialization fails
    }

    printf("Pyro Task: Initializing...\n");

    // Subscribe to pyro_command topic
    auto pyroCommandSubToken = MREQ_SUBSCRIBE(pyro_command);
    if (!pyroCommandSubToken) {
        printf("Pyro Task: Failed to subscribe to pyro_command topic!\n");
        vTaskDelete(NULL);
    }

    for (;;) {
        if (auto pyroCommand = MREQ_READ(pyro_command, *pyroCommandSubToken)) {
            if (pyroCommand->activate) {
                pyroDriver.activateChannel(pyroCommand->channel_id);
            } else {
                pyroDriver.deactivateChannel(pyroCommand->channel_id);
            }
            printf("Pyro Task: Processed pyro command for channel %d, activate: %d.\n",
                    static_cast<int>(pyroCommand->channel_id), pyroCommand->activate);
        } else {
            // No print statement here to avoid spamming the console
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Run every 10ms (high priority)
    }
}