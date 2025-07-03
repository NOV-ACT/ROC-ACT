#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "drivers/PyroDriver.hpp"
#include "core/MessagingClient.hpp"
#include "pyro_command.pb.h" // For PyroCommand message

extern "C" void pyro_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    novact::drivers::PyroDriver pyroDriver;
    novact::core::MessagingClient messagingClient;

    if (!pyroDriver.initialize()) {
        printf("Pyro Task: Failed to initialize Pyro Driver!\n");
        vTaskDelete(NULL); // Delete task if initialization fails
    }

    printf("Pyro Task: Initializing...\n");

    for (;;) {
        std::optional<PyroCommand> pyroCommand = messagingClient.readPyroCommand();
        if (pyroCommand) {
            if (pyroCommand.value().activate) {
                pyroDriver.activateChannel(pyroCommand.value().channel_id);
            } else {
                pyroDriver.deactivateChannel(pyroCommand.value().channel_id);
            }
            printf("Pyro Task: Processed pyro command for channel %d, activate: %d.\n",
                   static_cast<int>(pyroCommand.value().channel_id), pyroCommand.value().activate);
        } else {
            printf("Pyro Task: No new pyro commands.\n");
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Run every 10ms (high priority)
    }
}