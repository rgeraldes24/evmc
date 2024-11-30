/// EVMC: Ethereum Client-VM Connector API.
/// Copyright 2016 The EVMC Authors.
/// Licensed under the Apache License, Version 2.0.

#include "example_host.h"
#ifdef STATICALLY_LINKED_EXAMPLE
#include "example_vm/example_vm.h"
#endif

#include <zvmc/helpers.h>
#include <zvmc/loader.h>

#include <inttypes.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
#ifdef STATICALLY_LINKED_EXAMPLE
    (void)argc;
    (void)argv;
    struct zvmc_vm* vm = zvmc_create_example_vm();
    if (!vm)
        return ZVMC_LOADER_VM_CREATION_FAILURE;
    if (!zvmc_is_abi_compatible(vm))
        return ZVMC_LOADER_ABI_VERSION_MISMATCH;
#else
    const char* config_string = (argc > 1) ? argv[1] : "example-vm.so";
    enum zvmc_loader_error_code error_code = ZVMC_LOADER_UNSPECIFIED_ERROR;
    struct zvmc_vm* vm = zvmc_load_and_configure(config_string, &error_code);
    if (!vm)
    {
        printf("Loading error: %d\n", error_code);
        // NOTE: the values are small enough for casting
        return (int)error_code;
    }
#endif

    // ZVM bytecode goes here. This is one of the examples.
    const uint8_t code[] = "\x43\x60\x00\x55\x43\x60\x00\x52\x59\x60\x00\xf3";
    const size_t code_size = sizeof(code) - 1;
    const uint8_t input[] = "Hello World!";
    const zvmc_uint256be value = {{1, 0}};
    const zvmc_address addr = {{0, 1, 2}};
    const int64_t gas = 200000;
    struct zvmc_tx_context tx_context = {
        .block_number = 42,
        .block_timestamp = 66,
        .block_gas_limit = gas * 2,
    };
    const struct zvmc_host_interface* host = example_host_get_interface();
    struct zvmc_host_context* ctx = example_host_create_context(tx_context);
    struct zvmc_message msg = {
        .kind = ZVMC_CALL,
        .sender = addr,
        .recipient = addr,
        .value = value,
        .input_data = input,
        .input_size = sizeof(input),
        .gas = gas,
        .depth = 0,
    };
    struct zvmc_result result = zvmc_execute(vm, host, ctx, ZVMC_SHANGHAI, &msg, code, code_size);
    printf("Execution result:\n");
    int exit_code = 0;
    if (result.status_code != ZVMC_SUCCESS)
    {
        printf("  ZVM execution failure: %d\n", result.status_code);
        exit_code = result.status_code;
    }
    else
    {
        printf("  Gas used: %" PRId64 "\n", gas - result.gas_left);
        printf("  Gas left: %" PRId64 "\n", result.gas_left);
        printf("  Output size: %zd\n", result.output_size);
        printf("  Output: ");
        for (size_t i = 0; i < result.output_size; i++)
            printf("%02x", result.output_data[i]);
        printf("\n");
        const zvmc_bytes32 storage_key = {{0}};
        zvmc_bytes32 storage_value = host->get_storage(ctx, &msg.recipient, &storage_key);
        printf("  Storage at 0x00..00: ");
        for (size_t i = 0; i < sizeof(storage_value.bytes) / sizeof(storage_value.bytes[0]); i++)
            printf("%02x", storage_value.bytes[i]);
        printf("\n");
    }
    zvmc_release_result(&result);
    example_host_destroy_context(ctx);
    zvmc_destroy(vm);
    return exit_code;
}
