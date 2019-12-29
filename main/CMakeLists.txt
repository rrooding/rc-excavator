# Edit following two lines to set component requirements (see docs)
set(COMPONENT_SRCS "main.c"
                   "mac_address.c")
set(COMPONENT_ADD_INCLUDEDIRS "")

set(COMPONENT_REQUIRES controller excavator)

register_component()
