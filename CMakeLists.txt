# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# Somehow this is needed for now (can not be enabled through menuconfig)
add_compile_options(-DVND_BT_JV_BTA_L2CAP=TRUE)

set(SUPPORTED_TARGETS esp32)
set(COMPONENTS app_trace bootloader esptool_py main)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(rc-excavator)
