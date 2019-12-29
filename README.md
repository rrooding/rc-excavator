RC Excavator
====================

## Prerequisites
Make sure you have installed all of the following prerequisites on your development machine:

* ESP-IDF 4.1-dev
* ESP32-PS3 component in the ESP-IDF components folder ([rrooding/esp32-ps3](https://github.com/rrooding/esp32-ps3/tree/feature/esp-idf-4-support) for now).
* libusb-compat 0.1.5 (brew install libusb-compat)

## Configuration

Configure the app using `make menuconfig`

### Bluetooth

Make sure you have configured your project correctly in order to use the PS3 controller.

- Navigate to `Component config  --->` and press <kbd>Enter</kbd> to open the component config menu.
- Navigate to `Bluetooth  --->` and press <kbd>Enter</kbd> to open the Bluetooth config menu.
- Select `[] Bluetooth` and press <kbd>Y</kbd> to enable Bluetooth
- Navigate to `Bluetooth controller  --->` and press <kbd>Enter</kbd> to open its menu.
- Navigate to `Bluetooth controller mode` and press <kbd>Enter</kbd> to show the controller mode options.
- Select `BR/EDR Only` and press <kbd>Enter</kbd> to confirm the choice.
- Press <kbd>E</kbd> to go back to the Bluetooth menu.
- If it exists, navigate to `Bluetooth Host` and press <kbd>Enter</kbd> to show the host options.
- Select `Bluedroid - Dual-mode` and press <kbd>Enter</kbd> to confirm the choice.
- Navigate to `[] Bluedroid Enable  --->` and press <kbd>Y</kbd> to enable Bluedroid if it isn't already, then press <kbd>Enter</kbd> to open its menu.
- Navigate to `Classic Bluetooth` and press <kbd>Y</kbd> to enable it
- Navigate to `SPP` and press <kbd>Y</kbd> to enable it
- Navigate to `Secure Simple Pairing` and press <kbd>Y</kbd> to enable it if it isn't already
- Press <kbd>S</kbd> to save the configuration.


## Build

Build using `idf.py build`.

Flash using `idf.py -p /dev/tty.SLAB_USBtoUART flash`


## Utils

### [SixaxisPairTool](https://dancingpixelstudios.com/sixaxis-controller/sixaxispairtool/)

Configure the PS3 controller to connect to the ESP32. Connect the PS3 controller to your Mac and run the following with the MAC address of the ESP32:

`$ ./sixpair 11:22:33:44:55:66`
