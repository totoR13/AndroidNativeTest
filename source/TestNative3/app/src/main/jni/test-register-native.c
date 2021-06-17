#define LOG_TAG "simplejni native.cpp"
#include <android/log.h>
#include <stdio.h>
#include "jni.h"

#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static jint
add(JNIEnv* env, jobject thiz, jint a, jint b) {
int result = a + b;
    ALOGI("%d + %d = %d", a, b, result);
    return result;
}
static const char *classPathName = "com/experiments/testnative3/Native";
static JNINativeMethod methods[] = {
  {"add", "(II)I", (void*)add },
};
/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    env->FindClass(env, className);
    clazz = (*env)->FindClass(env, className);
    if (clazz == NULL) {
        ALOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
        ALOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}
/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
  if (!registerNativeMethods(env, classPathName,
                 methods, sizeof(methods) / sizeof(methods[0]))) {
    return JNI_FALSE;
  }
  return JNI_TRUE;
}
// ----------------------------------------------------------------------------
/*
 * This is called by the VM when the shared library is first loaded.
 */

/*typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;*/

jint JNI_OnLoad(JavaVM* vm, void* reserved /*reserved*/)
{
    //UnionJNIEnvToVoid uenv;
    //uenv.venv = NULL;
    jint result = -1;
    void* tmp;
    JNIEnv* env = NULL;

    ALOGI("JNI_OnLoad");
    if ((*vm)->GetEnv(vm, &tmp, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed");
        goto bail;
    }
    env = (JNIEnv*) tmp;
    if (registerNatives(env) != JNI_TRUE) {
        ALOGE("ERROR: registerNatives failed");
        goto bail;
    }

    result = JNI_VERSION_1_4;

bail:
    return result;
}