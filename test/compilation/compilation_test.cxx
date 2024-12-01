// EVMC: Ethereum Client-VM Connector API.
// Copyright 2018 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

// Test compilation of C and C++ public headers.

#include <zvmc/zvmc.h>
#include <zvmc/zvmc.hpp>
#include <zvmc/filter_iterator.hpp>
#include <zvmc/helpers.h>
#include <zvmc/hex.hpp>
#include <zvmc/instructions.h>
#include <zvmc/loader.h>
#include <zvmc/mocked_host.hpp>
#include <zvmc/utils.h>

// Include again to check if headers have proper include guards.
#include <zvmc/zvmc.h>               //NOLINT(readability-duplicate-include)
#include <zvmc/zvmc.hpp>             //NOLINT(readability-duplicate-include)
#include <zvmc/filter_iterator.hpp>  //NOLINT(readability-duplicate-include)
#include <zvmc/helpers.h>            //NOLINT(readability-duplicate-include)
#include <zvmc/hex.hpp>              //NOLINT(readability-duplicate-include)
#include <zvmc/instructions.h>       //NOLINT(readability-duplicate-include)
#include <zvmc/loader.h>             //NOLINT(readability-duplicate-include)
#include <zvmc/mocked_host.hpp>      //NOLINT(readability-duplicate-include)
#include <zvmc/utils.h>              //NOLINT(readability-duplicate-include)
