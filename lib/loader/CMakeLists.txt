# EVMC: Ethereum Client-VM Connector API.
# Copyright 2018 The EVMC Authors.
# Licensed under the Apache License, Version 2.0.

add_library(
    loader STATIC
    ${ZVMC_INCLUDE_DIR}/zvmc/loader.h
    loader.c
)

add_library(zvmc::loader ALIAS loader)
set_target_properties(loader PROPERTIES
    OUTPUT_NAME zvmc-loader
    POSITION_INDEPENDENT_CODE TRUE
)
target_link_libraries(loader INTERFACE ${CMAKE_DL_LIBS} PUBLIC zvmc::zvmc)

if(ZVMC_INSTALL)
    install(TARGETS loader EXPORT zvmcTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
