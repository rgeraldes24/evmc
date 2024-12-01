// EVMC: Ethereum Client-VM Connector API.
// Copyright 2019-2020 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.
package org.theqrl.zvmc;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

/**
 * The Java interface to the zvm instance.
 *
 * <p>Defines the Java methods capable of accessing the zvm implementation.
 */
public final class ZvmcVm implements AutoCloseable {
  private static final Throwable zvmcLoadingError;
  private ByteBuffer nativeVm;

  // Load the dynamic library containing the JNI bindings to ZVMC.
  static {
    Throwable error = null;

    // First try loading from global path.
    try {
      System.loadLibrary("libzvmc-java");
    } catch (UnsatisfiedLinkError globalLoadingError) {
      String extension = null;
      String os = System.getProperty("os.name").toLowerCase();
      if (os.contains("win")) {
        extension = "dll";
      } else if (os.contains("nix") || os.contains("nux") || os.contains("aix")) {
        extension = "so";
      } else if (os.contains("mac") || os.contains("darwin")) {
        extension = "dylib";
      } else {
        // Give up, because we are unsure what system we are running on.
        error = globalLoadingError;
      }

      // Try loading the binding from the package.
      if (extension != null) {
        try {
          Path zvmcLib = Files.createTempFile("libzvmc-java", extension);
          Files.copy(
              ZvmcVm.class.getResourceAsStream("/libzvmc-java." + extension),
              zvmcLib,
              StandardCopyOption.REPLACE_EXISTING);
          zvmcLib.toFile().deleteOnExit();
          // We are somewhat certain about the file, try loading it.
          try {
            System.load(zvmcLib.toAbsolutePath().toString());
          } catch (UnsatisfiedLinkError packageLoadingError) {
            error = packageLoadingError;
          }
        } catch (IOException packageCreationError) {
          error = packageCreationError;
        }
      }
    }
    zvmcLoadingError = error;
  }

  /**
   * Returns true if the native library was loaded successfully and ZVMC capabilities are available.
   *
   * @return true if the library is available
   */
  public static boolean isAvailable() {
    return zvmcLoadingError == null;
  }

  /**
   * This method loads the specified zvm shared library and loads/initializes the jni bindings.
   *
   * @param filename /path/filename of the zvm shared object
   * @throws org.theqrl.zvmc.ZvmcLoaderException
   */
  public static ZvmcVm create(String filename) throws ZvmcLoaderException {
    if (!isAvailable()) {
      throw new ZvmcLoaderException("ZVMC JNI binding library failed to load", zvmcLoadingError);
    }
    return new ZvmcVm(filename);
  }

  private ZvmcVm(String filename) throws ZvmcLoaderException {
    nativeVm = load_and_create(filename);
  }

  /**
   * This method loads the specified ZVM implementation and returns its pointer.
   *
   * @param filename Path to the dynamic object representing the ZVM implementation
   * @return Internal object pointer.
   * @throws org.theqrl.zvmc.ZvmcLoaderException
   */
  private static native ByteBuffer load_and_create(String filename) throws ZvmcLoaderException;

  /**
   * ZVMC ABI version implemented by the VM instance.
   *
   * <p>Can be used to detect ABI incompatibilities. The ZVMC ABI version represented by this file
   * is in ::ZVMC_ABI_VERSION.
   */
  public static native int abi_version();

  /**
   * The name of the ZVMC VM implementation.
   *
   * <p>It MUST be a NULL-terminated not empty string. The content MUST be UTF-8 encoded (this
   * implies ASCII encoding is also allowed).
   */
  private static native String name(ByteBuffer nativeVm);

  /** Function is a wrapper around native name(). */
  public String name() {
    return name(nativeVm);
  }

  /**
   * The version of the ZVMC VM implementation, e.g. "1.2.3b4".
   *
   * <p>It MUST be a NULL-terminated not empty string. The content MUST be UTF-8 encoded (this
   * implies ASCII encoding is also allowed).
   */
  private static native String version(ByteBuffer nativeVm);

  /** Function is a wrapper around native version(). */
  public String version() {
    return version(nativeVm);
  }

  /**
   * Function to destroy the VM instance.
   *
   * <p>This is a mandatory method and MUST NOT be set to NULL.
   */
  private static native void destroy(ByteBuffer nativeVm);

  /**
   * Function to execute a code by the VM instance.
   *
   * <p>This is a mandatory method and MUST NOT be set to NULL.
   */
  private static native ByteBuffer execute(
      ByteBuffer nativeVm, HostContext context, int rev, ByteBuffer msg, ByteBuffer code);

  /**
   * Function is a wrapper around native execute.
   *
   * <p>This allows the context to managed in one method
   */
  public synchronized ByteBuffer execute(
      HostContext context, int rev, ByteBuffer msg, ByteBuffer code) {
    return execute(nativeVm, context, rev, msg, code);
  }

  /**
   * A method returning capabilities supported by the VM instance.
   *
   * <p>The value returned MAY change when different options are set via the set_option() method.
   *
   * <p>A Client SHOULD only rely on the value returned if it has queried it after it has called the
   * set_option().
   *
   * <p>This is a mandatory method and MUST NOT be set to NULL.
   */
  private static native int get_capabilities(ByteBuffer nativeVm);

  /** Function is a wrapper around native get_capabilities(). */
  public int get_capabilities() {
    return get_capabilities(nativeVm);
  }

  /**
   * Function that modifies VM's options.
   *
   * <p>If the VM does not support this feature the pointer can be NULL.
   */
  private static native int set_option(ByteBuffer nativeVm, String name, String value);

  /** Function is a wrapper around native set_option(). */
  public int set_option(String name, String value) {
    return set_option(nativeVm, name, value);
  }

  /** This method cleans up resources. */
  @Override
  public void close() {
    destroy(nativeVm);
  }
}
