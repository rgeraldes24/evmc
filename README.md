# ZVMC

[![chat: on gitter][gitter badge]][Gitter]
[![readme style: standard][readme style standard badge]][standard readme]

> Zond Client-VM Connector API

The ZVMC is the low-level ABI between Zond Virtual Machines (ZVMs) and
Zond Clients. On the ZVM side it supports classic ZVM1 and [zwasm].
On the Client-side it defines the interface for ZVM implementations
to access Zond environment and state.


## Usage

### Documentation

Please visit the [documentation].

### Languages support

| Language                      | Supported Versions   | Supported Compilers          | Feature Support   |
|-------------------------------|----------------------|------------------------------|-------------------|
| **C**                         | C99, C11             | GCC 8+, clang 9+, MSVC 2017+ | Host- and VM-side |
| **C++**                       | C++17                | GCC 8+, clang 9+, MSVC 2017+ | Host- and VM-side |
| **Go** _(bindings)_           | 1.11+ (with modules) |                              | Host-side only    |
| **Rust** _(bindings)_[¹](#n1) | 2018 edition         | 1.47.0 and newer             | VM-side only      |
| **Java** _(bindings)_[²](#n2) | 11                   |                              | Host-side only    |

1. <sup id="n1">↑</sup> Rust support is limited and not complete yet, but it is mostly functional already. Breaking changes are possible at this stage.
2. <sup id="n2">↑</sup> Java support is in progress and the interface remains in flux. Breaking changes are possible at this stage.

### Testing tools

* **zvmc run** ([tools/zvmc]) — executes bytecode in any ZVMC-compatible VM implementation.
* **zvmc-vmtester** ([tools/vmtester]) — can test any ZVM implementation for compatibility with ZVMC.
* **zvm-test** ([zvmone → test/unittests]) — allows running the collection of [zvmone]'s unit tests on any ZVMC-compatible ZVM implementation.
* **zvmone-fuzzer** ([zvmone → test/fuzzer]) — differential fuzzer for ZVMC-compatible ZVM implementations. 

## License

[![license badge]][Apache License, Version 2.0]

Licensed under the [Apache License, Version 2.0].

## Internal

### Making new release

1. Update [CHANGELOG.md](CHANGELOG.md), put the release date, update release link.
2. `git add CHANGELOG.md`.
3. Tag new release: `bumpversion --allow-dirty prerel`.
4. Prepare CHANGELOG for next release: add unreleased section and link.
5. `git add CHANGELOG.md`.
6. Start new release series: `bumpversion --allow-dirty --no-tag minor`.