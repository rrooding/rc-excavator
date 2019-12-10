#include <stdio.h>
#include "ps3.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define STATUS_LED_GPIO 2
#define START_LED_GPIO 5

bool isStarted = false;

void toggleStart(void) {
  isStarted = !isStarted;
  gpio_set_level(START_LED_GPIO, isStarted ? 1 : 0);
}

void controller_event_cb(ps3_t ps3, ps3_event_t event) {
  if(event.button_up.start) {
    toggleStart();
  }

  // Don't process the rest of the events when we haven't started yet.
  if(!isStarted) {
    return;
  }

  if(event.analog_changed.stick.rx) {
    printf("leftx %i", event.analog_changed.stick.lx);
  }

  /*
    if ( event.button_down.cross ) {
      gpio_set_level(START_LED_GPIO, 1);
    }

    if ( event.button_up.cross ) {
      gpio_set_level(START_LED_GPIO, 0);
    }
    */
}

void init_gpio(void)
{
  gpio_pad_select_gpio(STATUS_LED_GPIO);
  gpio_set_direction(STATUS_LED_GPIO, GPIO_MODE_OUTPUT);

  gpio_pad_select_gpio(START_LED_GPIO);
  gpio_set_direction(START_LED_GPIO, GPIO_MODE_OUTPUT);
}

void app_main(void)
{
  init_gpio();

  nvs_flash_init();

  uint8_t mac_addr[8] = {0};
  esp_err_t ret = ESP_OK;

  ret = esp_efuse_mac_get_default(mac_addr);
  if (ret != ESP_OK) {
    printf("ERROR: Failed to get base MAC address from BLK0\n");
    abort();
  }
  else {
    char macStr[19];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
         mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5] + 2);
    printf("Connect PS3 controller to master %s", macStr);
  }

  ps3SetEventCallback(controller_event_cb);

  ps3Init();

  printf("Waiting for PS3 controller\n");
  bool blink = false;
  while (!ps3IsConnected()){
      gpio_set_level(STATUS_LED_GPIO, blink ? 1 : 0);
      blink = !blink;
      // Prevent the Task Watchdog from triggering
      vTaskDelay(100 / portTICK_PERIOD_MS);
  }

  // 250ms delay after connecting is necessary to allow the channel to clear
  // before pushing the LED configuration.
  vTaskDelay(250 / portTICK_PERIOD_MS);
  ps3SetLed(1);
  ps3Enable();
  gpio_set_level(STATUS_LED_GPIO, 1);
  printf("PS3 controller connected");

  while (1) {
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
