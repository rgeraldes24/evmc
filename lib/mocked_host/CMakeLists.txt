# EVMC: Ethereum Client-VM Connector API.
# Copyright 2019 The EVMC Authors.
# Licensed under the Apache License, Version 2.0.

add_library(mocked_host INTERFACE)
target_sources(mocked_host INTERFACE $<BUILD_INTERFACE:${ZVMC_INCLUDE_DIR}/zvmc/mocked_host.hpp>)

add_library(zvmc::mocked_host ALIAS mocked_host)
target_link_libraries(mocked_host INTERFACE zvmc::zvmc_cpp)

if(ZVMC_INSTALL)
    install(TARGETS mocked_host EXPORT zvmcTargets)
endif()
