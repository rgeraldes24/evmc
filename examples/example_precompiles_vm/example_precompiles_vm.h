// EVMC: Ethereum Client-VM Connector API.
// Copyright 2019 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.
#pragma once

#include <zvmc/utils.h>
#include <zvmc/zvmc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates ZVMC Example Precompiles VM.
 */
ZVMC_EXPORT struct zvmc_vm* zvmc_create_example_precompiles_vm(void);

#ifdef __cplusplus
}
#endif
