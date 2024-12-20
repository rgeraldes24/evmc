// EVMC: Ethereum Client-VM Connector API.
// Copyright 2019-2020 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

#include <zvmc/hex.hpp>
#include <zvmc/mocked_host.hpp>
#include <zvmc/tooling.hpp>
#include <zvmc/zvmc.hpp>
#include <chrono>
#include <ostream>

namespace zvmc::tooling
{
namespace
{
/// The address where a new contract is created with --create option.
constexpr auto create_address = "Zc9ea7ed000000000000000000000000000000001"_address;

/// The gas limit for contract creation.
constexpr auto create_gas = 10'000'000;

auto bench(MockedHost& host,
           zvmc::VM& vm,
           zvmc_revision rev,
           const zvmc_message& msg,
           bytes_view code,
           const zvmc::Result& expected_result,
           std::ostream& out)
{
    {
        using clock = std::chrono::steady_clock;
        using unit = std::chrono::nanoseconds;
        constexpr auto unit_name = " ns";
        constexpr auto target_bench_time = std::chrono::seconds{1};
        constexpr auto warning =
            "WARNING! Inconsistent execution result likely due to the use of storage ";

        // Probe run: execute once again the already warm code to estimate a single run time.
        const auto probe_start = clock::now();
        const auto result = vm.execute(host, rev, msg, code.data(), code.size());
        const auto bench_start = clock::now();
        const auto probe_time = bench_start - probe_start;

        if (result.gas_left != expected_result.gas_left)
            out << warning << "(gas used: " << (msg.gas - result.gas_left) << ")\n";
        if (bytes_view{result.output_data, result.output_size} !=
            bytes_view{expected_result.output_data, expected_result.output_size})
            out << warning << "(output: " << hex({result.output_data, result.output_size}) << ")\n";

        // Benchmark loop.
        const auto num_iterations = std::max(static_cast<int>(target_bench_time / probe_time), 1);
        for (int i = 0; i < num_iterations; ++i)
            vm.execute(host, rev, msg, code.data(), code.size());
        const auto bench_time = (clock::now() - bench_start) / num_iterations;

        out << "Time:     " << std::chrono::duration_cast<unit>(bench_time).count() << unit_name
            << " (avg of " << num_iterations << " iterations)\n";
    }
}
}  // namespace

int run(VM& vm,
        zvmc_revision rev,
        int64_t gas,
        bytes_view code,
        bytes_view input,
        bool create,
        bool bench,
        std::ostream& out)
{
    out << (create ? "Creating and executing on " : "Executing on ") << rev << " with " << gas
        << " gas limit\n";

    MockedHost host;

    zvmc_message msg{};
    msg.gas = gas;
    msg.input_data = input.data();
    msg.input_size = input.size();

    bytes_view exec_code = code;
    if (create)
    {
        zvmc_message create_msg{};
        create_msg.kind = ZVMC_CREATE;
        create_msg.recipient = create_address;
        create_msg.gas = create_gas;

        const auto create_result = vm.execute(host, rev, create_msg, code.data(), code.size());
        if (create_result.status_code != ZVMC_SUCCESS)
        {
            out << "Contract creation failed: " << create_result.status_code << "\n";
            return create_result.status_code;
        }

        auto& created_account = host.accounts[create_address];
        created_account.code = bytes(create_result.output_data, create_result.output_size);

        msg.recipient = create_address;
        exec_code = created_account.code;
    }
    out << "\n";

    const auto result = vm.execute(host, rev, msg, exec_code.data(), exec_code.size());

    if (bench)
        tooling::bench(host, vm, rev, msg, exec_code, result, out);

    const auto gas_used = msg.gas - result.gas_left;
    out << "Result:   " << result.status_code << "\nGas used: " << gas_used << "\n";

    if (result.status_code == ZVMC_SUCCESS || result.status_code == ZVMC_REVERT)
        out << "Output:   " << hex({result.output_data, result.output_size}) << "\n";

    return 0;
}
}  // namespace zvmc::tooling
