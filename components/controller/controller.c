#include <stdio.h>

#include "controller.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"
#include "ps3.h"

#define TAG "EX_CONTROLLER"

static controller_event_callback_t controller_event_cb;
static void *controller_event_cb_context;

void ps3_event_cb(ps3_t ps3, ps3_event_t ps3_event) {
  if (controller_event_cb) {
    controller_event_t event;

    event.left_joystick.x = ps3.analog.stick.lx;
    event.left_joystick.y = ps3.analog.stick.ly;
    event.right_joystick.x = ps3.analog.stick.rx;
    event.right_joystick.y = ps3.analog.stick.ry;

    event.up.button_up = ps3_event.button_up.up;
    event.up.button_down = ps3_event.button_down.up;
    event.down.button_up = ps3_event.button_up.down;
    event.down.button_down = ps3_event.button_down.down;
    event.left.button_up = ps3_event.button_up.left;
    event.left.button_down = ps3_event.button_down.left;
    event.right.button_up = ps3_event.button_up.right;
    event.right.button_down = ps3_event.button_down.right;

    event.l1.button_up = ps3_event.button_up.l1;
    event.l1.button_down = ps3_event.button_down.l1;
    event.l1.analog = ps3.analog.button.l1;
    event.l2.button_up = ps3_event.button_up.l2;
    event.l2.button_down = ps3_event.button_down.l2;
    event.l2.analog = ps3.analog.button.l2;
    event.r1.button_up = ps3_event.button_up.r1;
    event.r1.button_down = ps3_event.button_down.r1;
    event.r1.analog = ps3.analog.button.r1;
    event.r2.button_up = ps3_event.button_up.r2;
    event.r2.button_down = ps3_event.button_down.r2;
    event.r2.analog = ps3.analog.button.r2;

    event.start.button_up = ps3_event.button_up.start;
    event.start.button_down = ps3_event.button_down.start;
    event.select.button_up = ps3_event.button_up.select;
    event.select.button_down = ps3_event.button_down.select;

    event.a.button_up = ps3_event.button_up.cross;
    event.a.button_down = ps3_event.button_down.cross;
    event.b.button_up = ps3_event.button_up.circle;
    event.b.button_down = ps3_event.button_down.circle;
    event.x.button_up = ps3_event.button_up.square;
    event.x.button_down = ps3_event.button_down.square;
    event.y.button_up = ps3_event.button_up.triangle;
    event.y.button_down = ps3_event.button_down.triangle;

    controller_event_cb(controller_event_cb_context, event);
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

void controller_set_event_callback(controller_event_callback_t cb,
                                   void *context) {
  controller_event_cb = cb;
  controller_event_cb_context = context;
}

bool controller_is_connected() { return ps3IsConnected(); }
