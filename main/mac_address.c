#include "esp_log.h"
#include "esp_system.h"

#define TAG "EX_MAC"

void mac_address_print_bt_addr() {
  uint8_t mac_addr[8] = {0};
  esp_err_t ret = ESP_OK;
  ret = esp_read_mac(mac_addr, ESP_MAC_BT);

  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to get BT MAC address. (%s)", esp_err_to_name(ret));
    ESP_LOGE(TAG, "Aborting");
    abort();
  }

  char mac_str[18];
  snprintf(mac_str, sizeof(mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
           mac_addr[5]);

  ESP_LOGI(TAG, "Bluetooth MAC address: %s", mac_str);
}
