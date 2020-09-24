#ifndef CONTROLLER_MAC_ADDRESS
#define CONTROLLER_MAC_ADDRESS

#include "esp_err.h"

void mac_address_print_bt_addr();
esp_err_t mac_address_set(uint8_t *mac_address);

#endif
