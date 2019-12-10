#include <stdio.h>
#include "ps3.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BLINK_GPIO 2


void controller_event_cb( ps3_t ps3, ps3_event_t event )
{
    if ( event.button_down.cross ) {
      gpio_set_level(BLINK_GPIO, 1);
    }

    if ( event.button_up.cross ) {
      gpio_set_level(BLINK_GPIO, 0);
    }
}

void app_main(void)
{
  /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
     muxed to GPIO on reset already, but some default to other
     functions and need to be switched to GPIO. Consult the
     Technical Reference for a list of pads and their default
     functions.)
  */
  gpio_pad_select_gpio(BLINK_GPIO);
  /* Set the GPIO as a push/pull output */
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

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

  printf("Waiting for PS3 controller");
  while (!ps3IsConnected()){
      printf(".");
      // Prevent the Task Watchdog from triggering
      vTaskDelay(10 / portTICK_PERIOD_MS);
  }

  ps3Enable();
  printf("PS3 controller connected");
  ps3SetLed(1);

  while (1) {
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
