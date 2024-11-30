// EVMC: Ethereum Client-VM Connector API.
// Copyright 2018 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

/** This example shows how to use zvmc::instructions library from zvmc CMake package. */

#include <zvmc/instructions.h>

int main()
{
    return zvmc_get_instruction_metrics_table(ZVMC_SHANGHAI)[OP_STOP].gas_cost;
}
