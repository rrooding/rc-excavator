#include <stdio.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"
#include "ps3.h"

#define TAG "EX_CONTROLLER"

bool controller_is_connected() { return ps3IsConnected(); }

void ps3_event_cb(ps3_t ps3, ps3_event_t event) {
  if (ps3.analog.stick.lx) {
    ESP_LOGI(TAG, "Event detected %i", ps3.analog.stick.lx);
  }
}

void controller_initialize_connection(gpio_num_t gpio_num) {
  // 250ms delay after connecting is necessary to allow the channel to clear
  // before pushing the LED configuration.
  vTaskDelay(250 / portTICK_PERIOD_MS);
  ps3SetLed(1);

  // Enable sending data from the controller
  vTaskDelay(250 / portTICK_PERIOD_MS);
  ps3Enable();

  // Turn on the connection status LED
  controller_set_gpio(gpio_num, true);
}

void controller_handle_connection(void *pvParameters) {
  gpio_num_t gpio_num = (gpio_num_t)pvParameters;
  bool blink_state = false;
  bool connection_initialized = false;

  ESP_LOGI(TAG, "Waiting for controller connection.");

  while (true) {
    if (!controller_is_connected()) { // Wait for the controller to connect
      controller_set_gpio(gpio_num, blink_state);
      blink_state = !blink_state;
      connection_initialized = false;
      vTaskDelay(250 / portTICK_PERIOD_MS);
    } else if (!connection_initialized) { // Just connected
      ESP_LOGI(TAG, "Controller connected.");

      controller_initialize_connection(gpio_num);
      connection_initialized = true;
    } else { // Still connected
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }

  // Delete the task
  vTaskDelete(NULL);
}

void controller_init(gpio_num_t gpio_num) {
  controller_init_gpio(gpio_num);

  ps3SetEventCallback(ps3_event_cb);
  ps3Init();

  xTaskCreate(&controller_handle_connection, "controller_handle_connection",
              2048, (void *)gpio_num, 5, NULL);
}
