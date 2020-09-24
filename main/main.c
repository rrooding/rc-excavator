#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include <stdio.h>

#include "controller.h"
#include "excavator.h"
#include "pin_bindings.h"
#include "mac_address.h"

#define TAG "EX_MAIN"

void app_main(void) {
  // Initialize NVS flash, it is used by the PS3 library
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  Excavator *ex = ExcavatorCreate();

  mac_address_print_bt_addr();

  controller_set_event_callback(ExcavatorControllerCallback, ex);
  controller_init(CONTROLLER_STATUS_LED);
}
