# EVMC: Ethereum Client-VM Connector API.
# Copyright 2021 The EVMC Authors.
# Licensed under the Apache License, Version 2.0.

add_library(tooling STATIC)
add_library(zvmc::tooling ALIAS tooling)
target_compile_features(tooling PUBLIC cxx_std_17)
target_link_libraries(tooling PUBLIC zvmc::zvmc_cpp zvmc::mocked_host)

target_sources(
    tooling PRIVATE
    ${ZVMC_INCLUDE_DIR}/zvmc/tooling.hpp
    run.cpp
)

if(ZVMC_INSTALL)
    install(TARGETS tooling EXPORT zvmcTargets ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
