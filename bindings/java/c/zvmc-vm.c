// EVMC: Ethereum Client-VM Connector API.
// Copyright 2019 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

#include "host.h"
#include "org_theqrl_zvmc_ZvmcVm.h"
#include <zvmc/helpers.h>
#include <zvmc/loader.h>

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

JNIEXPORT jobject JNICALL Java_org_theqrl_zvmc_ZvmcVm_load_1and_1create(JNIEnv* jenv,
                                                                        jclass jcls,
                                                                        jstring jfilename)
{
    (void)jcls;
    struct zvmc_vm* zvm = NULL;
    jint rs = zvmc_java_set_jvm(jenv);
    (void)rs;
    assert(rs == JNI_OK);
    // load the ZVM
    const char* filename = (*jenv)->GetStringUTFChars(jenv, jfilename, NULL);
    assert(filename != NULL);
    enum zvmc_loader_error_code loader_error = ZVMC_LOADER_UNSPECIFIED_ERROR;
    zvm = zvmc_load_and_create(filename, &loader_error);
    (*jenv)->ReleaseStringUTFChars(jenv, jfilename, filename);
    if (loader_error != ZVMC_LOADER_SUCCESS)
    {
        const char* error_msg = zvmc_last_error_msg();
        jclass exception_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/ZvmcLoaderException");
        assert(exception_class != NULL);
        (*jenv)->ThrowNew(jenv, exception_class, error_msg ? error_msg : "Loading ZVMC VM failed");
    }
    jobject jresult = (*jenv)->NewDirectByteBuffer(jenv, (void*)zvm, sizeof(struct zvmc_vm));
    assert(jresult != NULL);
    return jresult;
}

JNIEXPORT jint JNICALL Java_org_theqrl_zvmc_ZvmcVm_abi_1version(JNIEnv* jenv, jclass jcls)
{
    (void)jenv;
    (void)jcls;
    return ZVMC_ABI_VERSION;
}

JNIEXPORT jstring JNICALL Java_org_theqrl_zvmc_ZvmcVm_name(JNIEnv* jenv, jclass jcls, jobject jzvm)
{
    (void)jcls;
    struct zvmc_vm* zvm = (struct zvmc_vm*)(*jenv)->GetDirectBufferAddress(jenv, jzvm);
    assert(zvm != NULL);
    const char* zvm_name = zvmc_vm_name(zvm);
    return (*jenv)->NewStringUTF(jenv, zvm_name);
}

JNIEXPORT jstring JNICALL Java_org_theqrl_zvmc_ZvmcVm_version(JNIEnv* jenv,
                                                              jclass jcls,
                                                              jobject jzvm)
{
    (void)jcls;
    struct zvmc_vm* zvm = (struct zvmc_vm*)(*jenv)->GetDirectBufferAddress(jenv, jzvm);
    assert(zvm != NULL);
    const char* zvm_version = zvmc_vm_version(zvm);
    return (*jenv)->NewStringUTF(jenv, zvm_version);
}

JNIEXPORT void JNICALL Java_org_theqrl_zvmc_ZvmcVm_destroy(JNIEnv* jenv, jclass jcls, jobject jzvm)
{
    (void)jcls;
    struct zvmc_vm* zvm = (struct zvmc_vm*)(*jenv)->GetDirectBufferAddress(jenv, jzvm);
    assert(zvm != NULL);
    zvmc_destroy(zvm);
}

static jobject AllocateDirect(JNIEnv* jenv, size_t capacity)
{
    const char java_class_name[] = "java/nio/ByteBuffer";
    const char java_method_name[] = "allocateDirect";
    const char java_method_signature[] = "(I)Ljava/nio/ByteBuffer;";

    jclass jcls = (*jenv)->FindClass(jenv, java_class_name);
    assert(jcls != NULL);
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, jcls, java_method_name, java_method_signature);
    assert(method != NULL);
    return (*jenv)->CallStaticObjectMethod(jenv, jcls, method, capacity);
}

JNIEXPORT jobject JNICALL Java_org_theqrl_zvmc_ZvmcVm_execute(JNIEnv* jenv,
                                                              jclass jcls,
                                                              jobject jzvm,
                                                              jobject jcontext,
                                                              jint jrev,
                                                              jobject jmsg,
                                                              jobject jcode)
{
    (void)jcls;
    struct zvmc_message* msg = (struct zvmc_message*)(*jenv)->GetDirectBufferAddress(jenv, jmsg);
    assert(msg != NULL);
    size_t code_size;
    const uint8_t* code = GetDirectBuffer(jenv, jcode, &code_size);
    struct zvmc_vm* zvm = (struct zvmc_vm*)(*jenv)->GetDirectBufferAddress(jenv, jzvm);
    assert(zvm != NULL);
    const struct zvmc_host_interface* host = zvmc_java_get_host_interface();
    jobject jresult = AllocateDirect(jenv, sizeof(struct zvmc_result));
    assert(jresult != NULL);
    struct zvmc_result* result =
        (struct zvmc_result*)(*jenv)->GetDirectBufferAddress(jenv, jresult);
    assert(result != NULL);
    *result = zvmc_execute(zvm, host, (struct zvmc_host_context*)jcontext, (enum zvmc_revision)jrev,
                           msg, code, code_size);
    return jresult;
}

JNIEXPORT jint JNICALL Java_org_theqrl_zvmc_ZvmcVm_get_1capabilities(JNIEnv* jenv,
                                                                     jclass jcls,
                                                                     jobject jzvm)
{
    (void)jcls;
    struct zvmc_vm* zvm = (struct zvmc_vm*)(*jenv)->GetDirectBufferAddress(jenv, jzvm);
    assert(zvm != NULL);
    return (jint)zvm->get_capabilities(zvm);
}

JNIEXPORT jint JNICALL Java_org_theqrl_zvmc_ZvmcVm_set_1option(JNIEnv* jenv,
                                                               jclass jcls,
                                                               jobject jzvm,
                                                               jstring jname,
                                                               jstring jval)
{
    (void)jcls;
    struct zvmc_vm* zvm = (struct zvmc_vm*)(*jenv)->GetDirectBufferAddress(jenv, jzvm);
    assert(zvm != NULL);
    const char* name = (*jenv)->GetStringUTFChars(jenv, jname, 0);
    const char* value = (*jenv)->GetStringUTFChars(jenv, jval, 0);
    assert(name != NULL);
    assert(value != NULL);
    enum zvmc_set_option_result option_result = zvmc_set_option(zvm, name, value);
    (*jenv)->ReleaseStringUTFChars(jenv, jname, name);
    (*jenv)->ReleaseStringUTFChars(jenv, jval, value);
    return (jint)option_result;
}
