idf_component_register(SRCS "excavator.c"
                            "servo.c"
                            "audio.c"
                       INCLUDE_DIRS "include"
                       REQUIRES controller driver
                )
