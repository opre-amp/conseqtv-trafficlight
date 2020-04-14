#include <string.h>
#include <mailbox.h>

#include "hu_conseqtv_Mailbox.h"

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_init_1mailbox
  (JNIEnv * env, jobject obj) {
      return init_mailbox();
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_uninit_1mailbox
  (JNIEnv * env, jobject obj) {
      return uninit_mailbox();
  }

static JavaVM* vm = NULL;

static JNIEnv* heartbeat_env;
static jobject heartbeat_obj;
static jmethodID heartbeat_mid;
static int heartbeat_initialized = 0;

static void handle_heartbeat() 
{
    if(!heartbeat_initialized) {
        (*vm)->AttachCurrentThread(vm, (void**)&heartbeat_env, NULL);
        heartbeat_initialized = 1;
    }
    (*heartbeat_env)->CallVoidMethod(heartbeat_env, heartbeat_obj, heartbeat_mid);
}

JNIEXPORT void JNICALL Java_hu_conseqtv_Mailbox_register_1heartbeat_1handler
  (JNIEnv * env, jobject _obj, jobject obj) {
      heartbeat_env = env;
      heartbeat_obj = obj;
      jclass cls = (*env)->GetObjectClass(env, obj);
      heartbeat_mid = (*env)->GetMethodID(env, cls, "handle", "()V");
      if(!vm)(*env)->GetJavaVM(env, &vm);
      heartbeat_initialized = 0;
      register_heartbeat_handler(handle_heartbeat);
  }

static JNIEnv* err_env;
static jobject err_obj;
static jmethodID err_mid;
static int err_initialized = 0;

static void handle_err(char* buf) 
{
    if(!err_initialized) {
        (*vm)->AttachCurrentThread(vm, (void**)&err_env, NULL);
        err_initialized = 1;
    }
    jstring str = (*err_env)->NewStringUTF(err_env, buf);
    (*err_env)->CallVoidMethod(err_env, err_obj, err_mid, str);
}

JNIEXPORT void JNICALL Java_hu_conseqtv_Mailbox_register_1error_1handler
  (JNIEnv * env, jobject _obj, jobject obj) {
      err_env = env;
      err_obj = obj;
      jclass cls = (*env)->GetObjectClass(env, obj);
      err_mid = (*env)->GetMethodID(env, cls, "handle", "(Ljava/lang/String;)V");
      if(!vm)(*env)->GetJavaVM(env, &vm);
      err_initialized = 0;
      register_error_handler(handle_err);
  }

JNIEXPORT jstring JNICALL Java_hu_conseqtv_Mailbox_get_1state
  (JNIEnv * env, jobject obj) {
      char buf[50];
      return get_state(buf, 50) ? (*env)->NewStringUTF(env, buf) : (*env)->NewStringUTF(env, "ERR");
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_set_1state
  (JNIEnv * env, jobject _obj, jstring str) {
      char buf[50];
      const char *ptr;
      strcpy(buf, ptr = (*env)->GetStringUTFChars(env, str, 0));
      (*env)->ReleaseStringUTFChars(env, str, ptr);
      return set_state(buf);
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_send_1signal
  (JNIEnv * env, jobject _obj, jstring str) {
      char buf[50];   
      const char *ptr;
      strcpy(buf, ptr = (*env)->GetStringUTFChars(env, str, 0));
      (*env)->ReleaseStringUTFChars(env, str, ptr);
      return send_signal(buf);
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_set_1time_1A
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_A(time);
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_set_1time_1B
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_B(time);
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_set_1time_1C
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_C(time);
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_set_1time_1C_1
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_C_(time);
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_set_1time_1D
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_D(time);
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_set_1time_1E
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_E(time);
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_set_1time_1F
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_F(time);
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_set_1time_1G
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_G(time);
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_set_1time_1H
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_H(time);
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_test_1red
  (JNIEnv * env, jobject obj) {
      return test_red();
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_test_1ylw
  (JNIEnv * env, jobject obj) {
      return test_ylw();
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_test_1grn
  (JNIEnv * env, jobject obj) {
      return test_grn();
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_test_1pred
  (JNIEnv * env, jobject obj) {
      return test_pred();
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_test_1pgrn
  (JNIEnv * env, jobject obj) {
      return test_pgrn();
  }