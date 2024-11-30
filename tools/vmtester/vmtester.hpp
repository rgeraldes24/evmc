// EVMC: Ethereum Client-VM Connector API
// Copyright 2018 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.
#pragma once

#include <zvmc/zvmc.hpp>
#include <gtest/gtest.h>

class zvmc_vm_test : public ::testing::Test
{
public:
    static void set_vm(zvmc::VM _owned_vm) noexcept { owned_vm = std::move(_owned_vm); }

protected:
    /// The raw pointer to the loaded VM instance.
    /// The C API is used to allow more sophisticated unit tests.
    zvmc_vm* vm = nullptr;

    /// The C++ RAII wrapper of the loaded VM instance.
    static zvmc::VM owned_vm;

    zvmc_vm_test() : vm{owned_vm.get_raw_pointer()} {}

    void SetUp() override { ASSERT_TRUE(vm != nullptr) << "VM instance not loaded"; }
};
