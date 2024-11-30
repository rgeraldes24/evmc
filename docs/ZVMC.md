# ZVMC – Zond Client-VM Connector API {#mainpage}

**ABI version 10**

The ZVMC is the low-level ABI between Zond Virtual Machines (ZVMs) and
Zond Clients. On the ZVM-side it supports classic ZVM1 and [ewasm].
On the Client-side it defines the interface for ZVM implementations
to access Zond environment and state.


# Guides {#guides}

- [Host Implementation Guide](@ref hostguide)
- [VM Implementation Guide](@ref vmguide)


# Versioning {#versioning}

The ZVMC project uses [Semantic Versioning](https://semver.org).
The version format is `MAJOR.MINOR.PATCH`.

The _MAJOR_ version number is also referenced as the **ZVMC ABI version**.
This ABI version is available to VM and Host implementations by ::ZVMC_ABI_VERSION.
For example ZVMC 3.2.1 would have ABI version 3 and therefore this project release
can be referenced as ZVMC ABIv3 or just ZVMC 3.
Every C ABI breaking change requires increasing the _MAJOR_ version number.

The releases with _MINOR_ version change allow adding new API features
and modifying the language bindings API.
Backward incompatible API changes are allowed but should be avoided if possible.

The releases with _PATCH_ should only include bug fixes. Exceptionally,
API changes are allowed when required to fix a broken feature.


# Modules {#modules}

- [ZVMC](@ref ZVMC)
   – the main component that defines API for VMs and Clients (Hosts).
- [ZVMC C++ API](@ref zvmc)
   – the wrappers and bindings for C++.
- [ZVMC Loader](@ref loader)
   – the library for loading VMs implemented as Dynamically Loaded Libraries (DLLs, shared objects).
- [ZVMC Helpers](@ref helpers)
   – a collection of utility functions for easier integration with ZVMC.
- [ZVM Instructions](@ref instructions)
   – the library with collection of metrics for ZVM1 instruction set.
- [ZVMC VM Tester](@ref vmtester)
   – the ZVMC-compatibility testing tool for VM implementations.


# Language bindings {#bindings}

## Go

```go
import "github.com/theQRL/zvmc/bindings/go/zvmc"
```


[ewasm]: https://github.com/ewasm/design


@addtogroup ZVMC

## Terms

1. **VM** – A Zond Virtual Machine instance/implementation.
2. **Host** – An entity controlling the VM.
   The Host requests code execution and responses to VM queries by callback
   functions. This usually represents a Zond Client.


## Responsibilities

### VM

- Executes the code (obviously).
- Calculates the running gas cost and manages the gas counter except the refund
  counter.
- Controls the call depth, including the exceptional termination of execution
  in case the maximum depth is reached.


### Host

- Provides access to State.
- Creates new accounts (with code being a result of VM execution).
- Handles refunds entirely.
- Manages the set of precompiled contracts and handles execution of messages
  coming to them.
