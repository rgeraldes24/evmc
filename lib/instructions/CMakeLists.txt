# EVMC: Ethereum Client-VM Connector API.
# Copyright 2018 The EVMC Authors.
# Licensed under the Apache License, Version 2.0.

add_library(
    instructions STATIC
    ${ZVMC_INCLUDE_DIR}/zvmc/instructions.h
    instruction_metrics.c
    instruction_names.c
)

add_library(zvmc::instructions ALIAS instructions)
set_target_properties(instructions PROPERTIES
    OUTPUT_NAME zvmc-instructions
    POSITION_INDEPENDENT_CODE TRUE
)
target_include_directories(instructions PUBLIC
    $<BUILD_INTERFACE:${ZVMC_INCLUDE_DIR}>$<INSTALL_INTERFACE:include>
)

if(ZVMC_INSTALL)
    install(TARGETS instructions EXPORT zvmcTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
