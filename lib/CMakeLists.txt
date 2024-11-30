# EVMC: Ethereum Client-VM Connector API.
# Copyright 2018 The EVMC Authors.
# Licensed under the Apache License, Version 2.0.

add_library(zvmc INTERFACE)
add_library(zvmc::zvmc ALIAS zvmc)
target_compile_features(zvmc INTERFACE c_std_99)
target_include_directories(zvmc INTERFACE $<BUILD_INTERFACE:${ZVMC_INCLUDE_DIR}>$<INSTALL_INTERFACE:include>)

add_library(zvmc_cpp INTERFACE)
add_library(zvmc::zvmc_cpp ALIAS zvmc_cpp)
target_compile_features(zvmc_cpp INTERFACE cxx_std_17)
target_include_directories(zvmc_cpp INTERFACE $<BUILD_INTERFACE:${ZVMC_INCLUDE_DIR}>$<INSTALL_INTERFACE:include>)
target_link_libraries(zvmc_cpp INTERFACE zvmc::zvmc)

add_subdirectory(instructions)
add_subdirectory(loader)
add_subdirectory(mocked_host)
add_subdirectory(tooling)

if(ZVMC_INSTALL)
    install(TARGETS zvmc zvmc_cpp EXPORT zvmcTargets)
endif()
