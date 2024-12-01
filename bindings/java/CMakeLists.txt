# EVMC: Ethereum Client-VM Connector API.
# Copyright 2020 The EVMC Authors.
# Licensed under the Apache License, Version 2.0.

include(UseJava)

find_package(Java REQUIRED COMPONENTS Development)
find_package(JNI REQUIRED)

add_jar(zvmc-jar
    OUTPUT_NAME zvmc-java
    VERSION ${PROJECT_VERSION}
    SOURCES
    java/src/main/java/org/theqrl/zvmc/ZvmcLoaderException.java
    java/src/main/java/org/theqrl/zvmc/ZvmcVm.java
    java/src/main/java/org/theqrl/zvmc/Host.java
    java/src/main/java/org/theqrl/zvmc/HostContext.java
    GENERATE_NATIVE_HEADERS zvmc-java-native-headers)

add_library(zvmc-java SHARED)
target_sources(zvmc-java PRIVATE
    c/zvmc-vm.c
    c/host.c
    c/host.h)
target_include_directories(zvmc-java PRIVATE ${JNI_INCLUDE_DIRS})
target_link_libraries(zvmc-java PRIVATE zvmc-java-native-headers zvmc::zvmc zvmc::loader ${JNI_LIBRARIES})

if(ZVMC_INSTALL)
    install(TARGETS zvmc-java EXPORT zvmcTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    install_jar(zvmc-jar DESTINATION ${CMAKE_INSTALL_DATADIR}/java)
endif()