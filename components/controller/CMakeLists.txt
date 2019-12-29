idf_component_register(SRCS "controller.c"
                            "gpio.c"
                       INCLUDE_DIRS "include"
                       REQUIRES ps3 driver
                )
