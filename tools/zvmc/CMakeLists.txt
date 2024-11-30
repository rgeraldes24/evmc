# EVMC: Ethereum Client-VM Connector API.
# Copyright 2019-2020 The EVMC Authors.
# Licensed under the Apache License, Version 2.0.

hunter_add_package(CLI11)
find_package(CLI11 REQUIRED)

add_executable(zvmc-tool main.cpp)
add_executable(zvmc::tool ALIAS zvmc-tool)
set_target_properties(zvmc-tool PROPERTIES OUTPUT_NAME zvmc)
set_source_files_properties(main.cpp PROPERTIES
    COMPILE_DEFINITIONS PROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(zvmc-tool PRIVATE zvmc::tooling zvmc::loader CLI11::CLI11)
