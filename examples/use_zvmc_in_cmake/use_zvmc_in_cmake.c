// EVMC: Ethereum Client-VM Connector API.
// Copyright 2018 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

/** This example shows how to use zvmc INTERFACE library from zvmc CMake package. */

#include <zvmc/zvmc.h>

int main()
{
    struct zvmc_vm vm = {.abi_version = ZVMC_ABI_VERSION};
    return vm.abi_version - ZVMC_ABI_VERSION;
}
