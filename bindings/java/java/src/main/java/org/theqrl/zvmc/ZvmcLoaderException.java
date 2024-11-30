// EVMC: Ethereum Client-VM Connector API.
// Copyright 2019-2020 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.
package org.theqrl.zvmc;

/** Exception thrown when the ZVMC binding or VM fails to load. */
public class ZvmcLoaderException extends Exception {
  public ZvmcLoaderException(String message) {
    super(message);
  }

  public ZvmcLoaderException(String message, Throwable cause) {
    super(message, cause);
  }
}
