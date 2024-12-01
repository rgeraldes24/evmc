# EVMC: Ethereum Client-VM Connector API.
# Copyright 2020 The EVMC Authors.
# Licensed under the Apache License, Version 2.0.

set(PREFIX ${PROJECT_NAME}/examples)

add_test(NAME ${PREFIX}/example-static COMMAND zvmc-example-static)
add_test(NAME ${PREFIX}/example-dynamic-load COMMAND zvmc-example $<TARGET_FILE:zvmc::example-vm>)
