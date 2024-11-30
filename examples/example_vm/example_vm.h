// EVMC: Ethereum Client-VM Connector API.
// Copyright 2018 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

#pragma once

#include <zvmc/zvmc.h>
#include <zvmc/utils.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates ZVMC Example VM.
 */
ZVMC_EXPORT struct zvmc_vm* zvmc_create_example_vm(void);

#ifdef __cplusplus
}
#endif
