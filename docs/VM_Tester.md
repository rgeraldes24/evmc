# ZVMC VM Tester {#vmtester}

The ZVMC project contains a ZVMC-compatibility testing tool for VM implementations.

The tool is called `zvmc-vmtester` and to include it in the ZVMC build
add `-DZVMC_TESTING=ON` CMake option to the project configuration step.

Usage is simple as

```sh
zvmc-vmtester [vm]
```

where `[vm]` is a path to a shared library with VM implementation.

For more information check `zvmc-vmtester --help`.
