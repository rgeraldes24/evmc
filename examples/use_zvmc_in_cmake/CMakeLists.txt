# EVMC: Ethereum Client-VM Connector API.
# Copyright 2018 The EVMC Authors.
# Licensed under the Apache License, Version 2.0.

# This example shows how to use zvmc INTERFACE library from zvmc CMake package.

cmake_minimum_required(VERSION 3.5)

project(use_zvmc_in_cmake)

find_package(zvmc CONFIG REQUIRED)

add_executable(use_zvmc_in_cmake use_zvmc_in_cmake.c)
target_link_libraries(use_zvmc_in_cmake PRIVATE zvmc::zvmc)



# Only for integration tests.
if(NOT COMMAND zvmc_add_vm_test)
    message(FATAL_ERROR "Function zvmc_add_vm_test() not in ZVMC.cmake module")
endif()
