// EVMC: Ethereum Client-VM Connector API.
// Copyright 2019 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "host.h"

static JavaVM* jvm;

int zvmc_java_set_jvm(JNIEnv* jenv)
{
    return (*jenv)->GetJavaVM(jenv, &jvm);
}

static JNIEnv* attach()
{
    JNIEnv* jenv;
    jint rs = (*jvm)->AttachCurrentThread(jvm, (void**)&jenv, NULL);
    (void)rs;
    assert(rs == JNI_OK);
    assert(jenv != NULL);
    return jenv;
}

// Why isn't this helper part of JNI?
static jbyteArray CopyDataToJava(JNIEnv* jenv, const void* ptr, size_t size)
{
    jbyteArray ret = (*jenv)->NewByteArray(jenv, (jsize)size);
    assert(ret != NULL);
    (*jenv)->SetByteArrayRegion(jenv, ret, 0, (jsize)size, (jbyte*)ptr);
    return ret;
}

static void CopyFromByteBuffer(JNIEnv* jenv, jobject src, void* dst, size_t size)
{
    size_t src_size;
    const void* ptr = GetDirectBuffer(jenv, src, &src_size);
    if (src_size != size)
    {
        jclass exception_class = (*jenv)->FindClass(jenv, "java/lang/IllegalArgumentException");
        assert(exception_class != NULL);
        (*jenv)->ThrowNew(jenv, exception_class, "Unexpected ByteBuffer length.");
    }
    memcpy(dst, ptr, size);
}

static bool account_exists_fn(struct zvmc_host_context* context, const zvmc_address* address)
{
    const char java_method_name[] = "account_exists";
    const char java_method_signature[] = "(Lorg/theqrl/zvmc/HostContext;[B)Z";

    assert(context != NULL);
    JNIEnv* jenv = attach();

    // get java class
    jclass host_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/Host");
    assert(host_class != NULL);

    // get java method
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, host_class, java_method_name, java_method_signature);
    assert(method != NULL);

    // set java method params
    jbyteArray jaddress = CopyDataToJava(jenv, address, sizeof(struct zvmc_address));

    // call java method
    jboolean jresult =
        (*jenv)->CallStaticBooleanMethod(jenv, host_class, method, (jobject)context, jaddress);
    return jresult != JNI_FALSE;
}

static zvmc_bytes32 get_storage_fn(struct zvmc_host_context* context,
                                   const zvmc_address* address,
                                   const zvmc_bytes32* key)
{
    const char java_method_name[] = "get_storage";
    const char java_method_signature[] = "(Lorg/theqrl/zvmc/HostContext;[B[B)Ljava/nio/ByteBuffer;";

    assert(context != NULL);
    JNIEnv* jenv = attach();

    // get java class
    jclass host_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/Host");
    assert(host_class != NULL);

    // get java method
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, host_class, java_method_name, java_method_signature);
    assert(method != NULL);

    // set java method params
    jbyteArray jaddress = CopyDataToJava(jenv, address, sizeof(struct zvmc_address));
    jbyteArray jkey = CopyDataToJava(jenv, key, sizeof(struct zvmc_bytes32));

    // call java method
    jobject jresult =
        (*jenv)->CallStaticObjectMethod(jenv, host_class, method, (jobject)context, jaddress, jkey);
    assert(jresult != NULL);

    zvmc_bytes32 result;
    CopyFromByteBuffer(jenv, jresult, &result, sizeof(zvmc_bytes32));
    return result;
}

static enum zvmc_storage_status set_storage_fn(struct zvmc_host_context* context,
                                               const zvmc_address* address,
                                               const zvmc_bytes32* key,
                                               const zvmc_bytes32* value)
{
    const char java_method_name[] = "set_storage";
    const char java_method_signature[] = "(Lorg/theqrl/zvmc/HostContext;[B[B[B)I";

    assert(context != NULL);
    JNIEnv* jenv = attach();

    // get java class
    jclass host_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/Host");
    assert(host_class != NULL);

    // get java method
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, host_class, java_method_name, java_method_signature);
    assert(method != NULL);

    // set java method params
    jbyteArray jaddress = CopyDataToJava(jenv, address, sizeof(struct zvmc_address));
    jbyteArray jkey = CopyDataToJava(jenv, key, sizeof(struct zvmc_bytes32));
    jbyteArray jval = CopyDataToJava(jenv, value, sizeof(struct zvmc_bytes32));

    // call java method
    jint jresult = (*jenv)->CallStaticIntMethod(jenv, host_class, method, (jobject)context,
                                                jaddress, jkey, jval);
    return (enum zvmc_storage_status)jresult;
}

static zvmc_uint256be get_balance_fn(struct zvmc_host_context* context, const zvmc_address* address)
{
    const char java_method_name[] = "get_balance";
    const char java_method_signature[] = "(Lorg/theqrl/zvmc/HostContext;[B)Ljava/nio/ByteBuffer;";

    assert(context != NULL);
    JNIEnv* jenv = attach();

    // get java class
    jclass host_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/Host");
    assert(host_class != NULL);

    // get java method
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, host_class, java_method_name, java_method_signature);
    assert(method != NULL);

    // set java method params
    jbyteArray jaddress = CopyDataToJava(jenv, address, sizeof(struct zvmc_address));

    // call java method
    jobject jresult =
        (*jenv)->CallStaticObjectMethod(jenv, host_class, method, (jobject)context, jaddress);
    assert(jresult != NULL);

    zvmc_uint256be result;
    CopyFromByteBuffer(jenv, jresult, &result, sizeof(zvmc_uint256be));

    (*jenv)->ReleaseByteArrayElements(jenv, jaddress, (jbyte*)address, 0);

    return result;
}

static size_t get_code_size_fn(struct zvmc_host_context* context, const zvmc_address* address)
{
    const char java_method_name[] = "get_code_size";
    const char java_method_signature[] = "(Lorg/theqrl/zvmc/HostContext;[B)I";

    assert(context != NULL);
    JNIEnv* jenv = attach();

    // get java class
    jclass host_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/Host");
    assert(host_class != NULL);

    // get java method
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, host_class, java_method_name, java_method_signature);
    assert(method != NULL);

    // set java method params
    jbyteArray jaddress = CopyDataToJava(jenv, address, sizeof(struct zvmc_address));

    // call java method
    jint jresult =
        (*jenv)->CallStaticIntMethod(jenv, host_class, method, (jobject)context, jaddress);
    return (size_t)jresult;
}

static zvmc_bytes32 get_code_hash_fn(struct zvmc_host_context* context, const zvmc_address* address)
{
    const char java_method_name[] = "get_code_hash";
    const char java_method_signature[] = "(Lorg/theqrl/zvmc/HostContext;[B)Ljava/nio/ByteBuffer;";

    assert(context != NULL);
    JNIEnv* jenv = attach();

    // get java class
    jclass host_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/Host");
    assert(host_class != NULL);

    // get java method
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, host_class, java_method_name, java_method_signature);
    assert(method != NULL);

    // set java method params
    jbyteArray jaddress = CopyDataToJava(jenv, address, sizeof(struct zvmc_address));

    // call java method
    jobject jresult =
        (*jenv)->CallStaticObjectMethod(jenv, host_class, method, (jobject)context, jaddress);
    assert(jresult != NULL);

    zvmc_bytes32 result;
    CopyFromByteBuffer(jenv, jresult, &result, sizeof(zvmc_bytes32));

    (*jenv)->ReleaseByteArrayElements(jenv, jaddress, (jbyte*)address, 0);

    return result;
}

static inline size_t min(size_t a, size_t b)
{
    return (a > b) ? b : a;
}

static size_t copy_code_fn(struct zvmc_host_context* context,
                           const zvmc_address* address,
                           size_t code_offset,
                           uint8_t* buffer_data,
                           size_t buffer_size)
{
    const char java_method_name[] = "copy_code";
    const char java_method_signature[] = "(Lorg/theqrl/zvmc/HostContext;[B)Ljava/nio/ByteBuffer;";

    assert(context != NULL);
    JNIEnv* jenv = attach();

    // get java class
    jclass host_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/Host");
    assert(host_class != NULL);

    // get java method
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, host_class, java_method_name, java_method_signature);
    assert(method != NULL);

    // set java method params
    jbyteArray jaddress = CopyDataToJava(jenv, address, sizeof(struct zvmc_address));

    // call java method
    jobject jresult =
        (*jenv)->CallStaticObjectMethod(jenv, host_class, method, (jobject)context, jaddress);
    assert(jresult != NULL);

    // copy jresult back to buffer_data
    size_t code_size;
    uint8_t* code = GetDirectBuffer(jenv, jresult, &code_size);

    size_t length = 0;
    if (code_offset < code_size)
    {
        length = min(buffer_size, code_size - code_offset);
        if (length > 0)
            memcpy(buffer_data, code + code_offset, length);
    }

    (*jenv)->ReleaseByteArrayElements(jenv, jaddress, (jbyte*)address, 0);

    return length;
}

static struct zvmc_result call_fn(struct zvmc_host_context* context, const struct zvmc_message* msg)
{
    const char java_method_name[] = "call";
    const char java_method_signature[] =
        "(Lorg/theqrl/zvmc/HostContext;Ljava/nio/ByteBuffer;)Ljava/nio/ByteBuffer;";

    assert(context != NULL);
    JNIEnv* jenv = attach();

    // get java class
    jclass host_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/Host");
    assert(host_class != NULL);

    // get java method
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, host_class, java_method_name, java_method_signature);
    assert(method != NULL);

    // set java method params
    jobject jmsg = (*jenv)->NewDirectByteBuffer(jenv, (void*)msg, sizeof(struct zvmc_message));
    assert(jmsg != NULL);

    // call java method
    jobject jresult =
        (*jenv)->CallStaticObjectMethod(jenv, host_class, method, (jobject)context, jmsg);
    assert(jresult != NULL);

    struct zvmc_result result;
    CopyFromByteBuffer(jenv, jresult, &result, sizeof(struct zvmc_result));
    return result;
}

static struct zvmc_tx_context get_tx_context_fn(struct zvmc_host_context* context)
{
    const char java_method_name[] = "get_tx_context";
    const char java_method_signature[] = "(Lorg/theqrl/zvmc/HostContext;)Ljava/nio/ByteBuffer;";

    assert(context != NULL);
    JNIEnv* jenv = attach();

    // get java class
    jclass host_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/Host");
    assert(host_class != NULL);

    // get java method
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, host_class, java_method_name, java_method_signature);
    assert(method != NULL);

    // call java method
    jobject jresult = (*jenv)->CallStaticObjectMethod(jenv, host_class, method, (jobject)context);
    assert(jresult != NULL);

    struct zvmc_tx_context result;
    CopyFromByteBuffer(jenv, jresult, &result, sizeof(struct zvmc_tx_context));
    return result;
}

static zvmc_bytes32 get_block_hash_fn(struct zvmc_host_context* context, int64_t number)
{
    char java_method_name[] = "get_block_hash";
    char java_method_signature[] = "(Lorg/theqrl/zvmc/HostContext;J)Ljava/nio/ByteBuffer;";

    assert(context != NULL);
    JNIEnv* jenv = attach();

    // get java class
    jclass host_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/Host");
    assert(host_class != NULL);

    // get java method
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, host_class, java_method_name, java_method_signature);
    assert(method != NULL);

    // call java method
    jobject jresult =
        (*jenv)->CallStaticObjectMethod(jenv, host_class, method, (jobject)context, (jlong)number);
    assert(jresult != NULL);

    zvmc_bytes32 result;
    CopyFromByteBuffer(jenv, jresult, &result, sizeof(zvmc_bytes32));
    return result;
}

static void emit_log_fn(struct zvmc_host_context* context,
                        const zvmc_address* address,
                        const uint8_t* data,
                        size_t data_size,
                        const zvmc_bytes32 topics[],
                        size_t topics_count)
{
    const char java_method_name[] = "emit_log";
    const char java_method_signature[] = "(Lorg/theqrl/zvmc/HostContext;[B[BI[[BI)V";

    assert(context != NULL);
    JNIEnv* jenv = attach();

    // get java class
    jclass host_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/Host");
    assert(host_class != NULL);

    // get java method
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, host_class, java_method_name, java_method_signature);
    assert(method != NULL);

    // set java method params
    jbyteArray jaddress = CopyDataToJava(jenv, address, sizeof(struct zvmc_address));
    jbyteArray jdata = CopyDataToJava(jenv, data, data_size);

    jclass byte_type = (*jenv)->FindClass(jenv, "[B");
    jobjectArray jtopics = (*jenv)->NewObjectArray(jenv, (jsize)topics_count, byte_type, NULL);
    assert(jtopics != NULL);
    for (size_t i = 0; i < topics_count; i++)
    {
        jbyteArray jtopic = CopyDataToJava(jenv, topics[i].bytes, sizeof(struct zvmc_bytes32));
        (*jenv)->SetObjectArrayElement(jenv, jtopics, (jsize)i, jtopic);
        (*jenv)->DeleteLocalRef(jenv, jtopic);
    }

    // call java method
    (*jenv)->CallStaticIntMethod(jenv, host_class, method, (jobject)context, jaddress, jdata,
                                 data_size, jtopics, topics_count);
}

static enum zvmc_access_status access_account_fn(struct zvmc_host_context* context,
                                                 const zvmc_address* address)
{
    const char java_method_name[] = "access_account";
    const char java_method_signature[] = "(Lorg/theqrl/zvmc/HostContext;[B)I";

    assert(context != NULL);
    JNIEnv* jenv = attach();

    // get java class
    jclass host_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/Host");
    assert(host_class != NULL);

    // get java method
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, host_class, java_method_name, java_method_signature);
    assert(method != NULL);

    // set java method params
    jbyteArray jaddress = CopyDataToJava(jenv, address, sizeof(struct zvmc_address));

    // call java method
    jint jresult =
        (*jenv)->CallStaticIntMethod(jenv, host_class, method, (jobject)context, jaddress);
    assert(jresult == ZVMC_ACCESS_COLD || jresult == ZVMC_ACCESS_WARM);
    return (enum zvmc_access_status)jresult;
}

static enum zvmc_access_status access_storage_fn(struct zvmc_host_context* context,
                                                 const zvmc_address* address,
                                                 const zvmc_bytes32* key)
{
    const char java_method_name[] = "access_storage";
    const char java_method_signature[] = "(Lorg/theqrl/zvmc/HostContext;[B[B)I";

    assert(context != NULL);
    JNIEnv* jenv = attach();

    // get java class
    jclass host_class = (*jenv)->FindClass(jenv, "org/theqrl/zvmc/Host");
    assert(host_class != NULL);

    // get java method
    jmethodID method =
        (*jenv)->GetStaticMethodID(jenv, host_class, java_method_name, java_method_signature);
    assert(method != NULL);

    // set java method params
    jbyteArray jaddress = CopyDataToJava(jenv, address, sizeof(struct zvmc_address));
    jbyteArray jkey = CopyDataToJava(jenv, key, sizeof(struct zvmc_bytes32));

    // call java method
    jint jresult =
        (*jenv)->CallStaticIntMethod(jenv, host_class, method, (jobject)context, jaddress, jkey);
    assert(jresult == ZVMC_ACCESS_COLD || jresult == ZVMC_ACCESS_WARM);
    return (enum zvmc_access_status)jresult;
}

const struct zvmc_host_interface* zvmc_java_get_host_interface()
{
    static const struct zvmc_host_interface host = {
        account_exists_fn, get_storage_fn,    set_storage_fn, get_balance_fn,
        get_code_size_fn,  get_code_hash_fn,  copy_code_fn,   call_fn,
        get_tx_context_fn, get_block_hash_fn, emit_log_fn,    access_account_fn,
        access_storage_fn,
    };
    return &host;
}
