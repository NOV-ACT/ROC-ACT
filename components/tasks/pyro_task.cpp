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

    // Subscribe to pyro_command topic
    std::optional<size_t> pyroCommandSubToken = messagingClient.subscribePyroCommand();
    if (!pyroCommandSubToken) {
        printf("Pyro Task: Failed to subscribe to pyro_command topic!\n");
        vTaskDelete(NULL);
    }

    for (;;) {
        PyroCommand pyroCommand;
        if (messagingClient.checkPyroCommand(pyroCommandSubToken.value())) {
            if (messagingClient.readPyroCommand(pyroCommandSubToken.value(), pyroCommand)) {
                if (pyroCommand.activate) {
                    pyroDriver.activateChannel(pyroCommand.channel_id);
                } else {
                    pyroDriver.deactivateChannel(pyroCommand.channel_id);
                }
                printf("Pyro Task: Processed pyro command for channel %d, activate: %d.\n",
                       static_cast<int>(pyroCommand.channel_id), pyroCommand.activate);
            }
        } else {
            printf("Pyro Task: No new pyro commands.\n");
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Run every 10ms (high priority)
    }
}