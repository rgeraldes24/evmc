// EVMC: Ethereum Client-VM Connector API.
// Copyright 2019 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

/**
 * @file
 * The loader OS mock for opening DLLs. To be inserted in loader.c for unit tests.
 */

static const int magic_handle = 0xE7AC;

const char* zvmc_test_library_path = NULL;
const char* zvmc_test_library_symbol = NULL;
zvmc_create_fn zvmc_test_create_fn = NULL;

static const char* zvmc_test_last_error_msg = NULL;

/* Limited variant of strcpy_s(). Exposed to unittests when building with ZVMC_LOADER_MOCK. */
int strcpy_sx(char* dest, size_t destsz, const char* src);

static int zvmc_test_load_library(const char* filename)
{
    zvmc_test_last_error_msg = NULL;
    if (filename && zvmc_test_library_path && strcmp(filename, zvmc_test_library_path) == 0)
        return magic_handle;
    zvmc_test_last_error_msg = "cannot load library";
    return 0;
}

static void zvmc_test_free_library(int handle)
{
    (void)handle;
}

static zvmc_create_fn zvmc_test_get_symbol_address(int handle, const char* symbol)
{
    if (handle != magic_handle)
        return NULL;

    if (zvmc_test_library_symbol && strcmp(symbol, zvmc_test_library_symbol) == 0)
        return zvmc_test_create_fn;
    return NULL;
}

static const char* zvmc_test_get_last_error_msg(void)
{
    // Return the last error message only once.
    const char* m = zvmc_test_last_error_msg;
    zvmc_test_last_error_msg = NULL;
    return m;
}

#define DLL_HANDLE int
#define DLL_OPEN(filename) zvmc_test_load_library(filename)
#define DLL_CLOSE(handle) zvmc_test_free_library(handle)
#define DLL_GET_CREATE_FN(handle, name) zvmc_test_get_symbol_address(handle, name)
#define DLL_GET_ERROR_MSG() zvmc_test_get_last_error_msg()
