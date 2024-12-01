// EVMC: Ethereum Client-VM Connector API.
// Copyright 2018 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

#include "_cgo_export.h"

#include <stdlib.h>

/* Go does not support exporting functions with parameters with const modifiers,
 * so we have to cast function pointers to the function types defined in ZVMC.
 * This disables any type checking of exported Go functions. To mitigate this
 * problem the go_exported_functions_type_checks() function simulates usage
 * of Go exported functions with expected types to check them during compilation.
 */
const struct zvmc_host_interface zvmc_go_host = {
    (zvmc_account_exists_fn)accountExists,
    (zvmc_get_storage_fn)getStorage,
    (zvmc_set_storage_fn)setStorage,
    (zvmc_get_balance_fn)getBalance,
    (zvmc_get_code_size_fn)getCodeSize,
    (zvmc_get_code_hash_fn)getCodeHash,
    (zvmc_copy_code_fn)copyCode,
    (zvmc_call_fn)call,
    (zvmc_get_tx_context_fn)getTxContext,
    (zvmc_get_block_hash_fn)getBlockHash,
    (zvmc_emit_log_fn)emitLog,
    (zvmc_access_account_fn)accessAccount,
    (zvmc_access_storage_fn)accessStorage,
};


#pragma GCC diagnostic error "-Wconversion"
static inline void go_exported_functions_type_checks()
{
    struct zvmc_host_context* context = NULL;
    zvmc_address* address = NULL;
    zvmc_bytes32 bytes32;
    uint8_t* data = NULL;
    size_t size = 0;
    int64_t number = 0;
    struct zvmc_message* message = NULL;

    zvmc_uint256be uint256be;
    (void)uint256be;
    struct zvmc_tx_context tx_context;
    (void)tx_context;
    struct zvmc_result result;
    (void)result;
    enum zvmc_access_status access_status;
    (void)access_status;
    enum zvmc_storage_status storage_status;
    (void)storage_status;
    bool bool_flag;
    (void)bool_flag;

    zvmc_account_exists_fn account_exists_fn = NULL;
    bool_flag = account_exists_fn(context, address);
    bool_flag = accountExists(context, address);

    zvmc_get_storage_fn get_storage_fn = NULL;
    bytes32 = get_storage_fn(context, address, &bytes32);
    bytes32 = getStorage(context, address, &bytes32);

    zvmc_set_storage_fn set_storage_fn = NULL;
    storage_status = set_storage_fn(context, address, &bytes32, &bytes32);
    storage_status = setStorage(context, address, &bytes32, &bytes32);

    zvmc_get_balance_fn get_balance_fn = NULL;
    uint256be = get_balance_fn(context, address);
    uint256be = getBalance(context, address);

    zvmc_get_code_size_fn get_code_size_fn = NULL;
    size = get_code_size_fn(context, address);
    size = getCodeSize(context, address);

    zvmc_get_code_hash_fn get_code_hash_fn = NULL;
    bytes32 = get_code_hash_fn(context, address);
    bytes32 = getCodeHash(context, address);

    zvmc_copy_code_fn copy_code_fn = NULL;
    size = copy_code_fn(context, address, size, data, size);
    size = copyCode(context, address, size, data, size);

    zvmc_call_fn call_fn = NULL;
    result = call_fn(context, message);
    result = call(context, message);

    zvmc_get_tx_context_fn get_tx_context_fn = NULL;
    tx_context = get_tx_context_fn(context);
    tx_context = getTxContext(context);

    zvmc_get_block_hash_fn get_block_hash_fn = NULL;
    bytes32 = get_block_hash_fn(context, number);
    bytes32 = getBlockHash(context, number);

    zvmc_emit_log_fn emit_log_fn = NULL;
    emit_log_fn(context, address, data, size, &bytes32, size);
    emitLog(context, address, data, size, &bytes32, size);

    zvmc_access_account_fn access_account_fn = NULL;
    access_status = access_account_fn(context, address);
    access_status = accessAccount(context, address);

    zvmc_access_storage_fn access_storage_fn = NULL;
    access_status = access_storage_fn(context, address, &bytes32);
    access_status = accessStorage(context, address, &bytes32);
}
