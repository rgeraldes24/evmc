// EVMC: Ethereum Client-VM Connector API.
// Copyright 2016 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

/// @file
/// Example implementation of an ZVMC Host.

#include "example_host.h"

#include <zvmc/zvmc.hpp>

#include <algorithm>
#include <map>
#include <vector>

using namespace zvmc::literals;

namespace zvmc
{
struct account
{
    virtual ~account() = default;

    zvmc::uint256be balance = {};
    std::vector<uint8_t> code;
    std::map<zvmc::bytes32, zvmc::bytes32> storage;

    virtual zvmc::bytes32 code_hash() const
    {
        // Extremely dumb "hash" function.
        zvmc::bytes32 ret{};
        for (const auto v : code)
            ret.bytes[v % sizeof(ret.bytes)] ^= v;
        return ret;
    }
};

using accounts = std::map<zvmc::address, account>;

}  // namespace zvmc

class ExampleHost : public zvmc::Host
{
    zvmc::accounts accounts;
    zvmc_tx_context tx_context{};

public:
    ExampleHost() = default;
    explicit ExampleHost(zvmc_tx_context& _tx_context) noexcept : tx_context{_tx_context} {}
    ExampleHost(zvmc_tx_context& _tx_context, zvmc::accounts& _accounts) noexcept
      : accounts{_accounts}, tx_context{_tx_context}
    {}

    bool account_exists(const zvmc::address& addr) const noexcept final
    {
        return accounts.find(addr) != accounts.end();
    }

    zvmc::bytes32 get_storage(const zvmc::address& addr,
                              const zvmc::bytes32& key) const noexcept final
    {
        const auto account_iter = accounts.find(addr);
        if (account_iter == accounts.end())
            return {};

        const auto storage_iter = account_iter->second.storage.find(key);
        if (storage_iter != account_iter->second.storage.end())
            return storage_iter->second;
        return {};
    }

    zvmc_storage_status set_storage(const zvmc::address& addr,
                                    const zvmc::bytes32& key,
                                    const zvmc::bytes32& value) noexcept final
    {
        auto& account = accounts[addr];
        auto prev_value = account.storage[key];
        account.storage[key] = value;

        return (prev_value == value) ? ZVMC_STORAGE_ASSIGNED : ZVMC_STORAGE_MODIFIED;
    }

    zvmc::uint256be get_balance(const zvmc::address& addr) const noexcept final
    {
        auto it = accounts.find(addr);
        if (it != accounts.end())
            return it->second.balance;
        return {};
    }

    size_t get_code_size(const zvmc::address& addr) const noexcept final
    {
        auto it = accounts.find(addr);
        if (it != accounts.end())
            return it->second.code.size();
        return 0;
    }

    zvmc::bytes32 get_code_hash(const zvmc::address& addr) const noexcept final
    {
        auto it = accounts.find(addr);
        if (it != accounts.end())
            return it->second.code_hash();
        return {};
    }

    size_t copy_code(const zvmc::address& addr,
                     size_t code_offset,
                     uint8_t* buffer_data,
                     size_t buffer_size) const noexcept final
    {
        const auto it = accounts.find(addr);
        if (it == accounts.end())
            return 0;

        const auto& code = it->second.code;

        if (code_offset >= code.size())
            return 0;

        const auto n = std::min(buffer_size, code.size() - code_offset);

        if (n > 0)
            std::copy_n(&code[code_offset], n, buffer_data);
        return n;
    }

    zvmc::Result call(const zvmc_message& msg) noexcept final
    {
        return zvmc::Result{ZVMC_REVERT, msg.gas, 0, msg.input_data, msg.input_size};
    }

    zvmc_tx_context get_tx_context() const noexcept final { return tx_context; }

    // NOLINTNEXTLINE(bugprone-exception-escape)
    zvmc::bytes32 get_block_hash(int64_t number) const noexcept final
    {
        const int64_t current_block_number = get_tx_context().block_number;

        return (number < current_block_number && number >= current_block_number - 256) ?
                   0xb10c8a5fb10c8a5fb10c8a5fb10c8a5fb10c8a5fb10c8a5fb10c8a5fb10c8a5f_bytes32 :
                   0x0000000000000000000000000000000000000000000000000000000000000000_bytes32;
    }

    void emit_log(const zvmc::address& addr,
                  const uint8_t* data,
                  size_t data_size,
                  const zvmc::bytes32 topics[],
                  size_t topics_count) noexcept final
    {
        (void)addr;
        (void)data;
        (void)data_size;
        (void)topics;
        (void)topics_count;
    }

    zvmc_access_status access_account(const zvmc::address& addr) noexcept final
    {
        (void)addr;
        return ZVMC_ACCESS_COLD;
    }

    zvmc_access_status access_storage(const zvmc::address& addr,
                                      const zvmc::bytes32& key) noexcept final
    {
        (void)addr;
        (void)key;
        return ZVMC_ACCESS_COLD;
    }
};


extern "C" {

const zvmc_host_interface* example_host_get_interface()
{
    return &zvmc::Host::get_interface();
}

zvmc_host_context* example_host_create_context(zvmc_tx_context tx_context)
{
    auto host = new ExampleHost{tx_context};
    return host->to_context();
}

void example_host_destroy_context(zvmc_host_context* context)
{
    delete zvmc::Host::from_context<ExampleHost>(context);
}
}
