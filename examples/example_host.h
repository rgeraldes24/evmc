// EVMC: Ethereum Client-VM Connector API.
// Copyright 2016 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

#include <zvmc/zvmc.h>

#if __cplusplus
extern "C" {
#endif

const struct zvmc_host_interface* example_host_get_interface(void);

struct zvmc_host_context* example_host_create_context(struct zvmc_tx_context tx_context);

void example_host_destroy_context(struct zvmc_host_context* context);

#if __cplusplus
}
#endif
