# EVMC: Ethereum Client-VM Connector API.
# Copyright 2019 The EVMC Authors.
# Licensed under the Apache License, Version 2.0.


# Adds a CMake test to check the given ZVMC VM implementation with the zvmc-vmtester tool.
#
# zvmc_add_vm_test(NAME <test_name> TARGET <vm>)
# - NAME argument specifies the name of the added test,
# - TARGET argument specifies the CMake target being a shared library with ZVMC VM implementation.
function(zvmc_add_vm_test)
    if(NOT TARGET zvmc::zvmc-vmtester)
        message(FATAL_ERROR "The zvmc-vmtester has not been installed with this ZVMC package")
    endif()

    cmake_parse_arguments("" "" NAME;TARGET "" ${ARGN})
    add_test(NAME ${_NAME} COMMAND zvmc::zvmc-vmtester $<TARGET_FILE:${_TARGET}>)
endfunction()
