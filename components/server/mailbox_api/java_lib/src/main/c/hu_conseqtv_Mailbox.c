#include <string.h>
#include <mailbox.h>

#include "hu_conseqtv_Mailbox.h"

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_initMailbox
  (JNIEnv * env, jobject obj) {
      return init_mailbox();
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_uninitMailbox
  (JNIEnv * env, jobject obj) {
      return uninit_mailbox();
  }

static JavaVM* vm = NULL;

static JNIEnv* heartbeat_env;
static jobject heartbeat_obj;
static jmethodID heartbeat_mid;
static int heartbeat_initialized = 0;

static void handleHeartbeat() 
{
    if(!heartbeat_initialized) {
        (*vm)->AttachCurrentThread(vm, (void**)&heartbeat_env, NULL);
        heartbeat_initialized = 1;
    }
    (*heartbeat_env)->CallVoidMethod(heartbeat_env, heartbeat_obj, heartbeat_mid);
}

JNIEXPORT void JNICALL Java_hu_conseqtv_Mailbox_registerHeartbeatHhandler
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

static void handleErr(char* buf) 
{
    if(!err_initialized) {
        (*vm)->AttachCurrentThread(vm, (void**)&err_env, NULL);
        err_initialized = 1;
    }
    jstring str = (*err_env)->NewStringUTF(err_env, buf);
    (*err_env)->CallVoidMethod(err_env, err_obj, err_mid, str);
}

JNIEXPORT void JNICALL Java_hu_conseqtv_Mailbox_registerErrorHandler
  (JNIEnv * env, jobject _obj, jobject obj) {
      err_env = env;
      err_obj = obj;
      jclass cls = (*env)->GetObjectClass(env, obj);
      err_mid = (*env)->GetMethodID(env, cls, "handle", "(Ljava/lang/String;)V");
      if(!vm)(*env)->GetJavaVM(env, &vm);
      err_initialized = 0;
      register_error_handler(handle_err);
  }

JNIEXPORT jstring JNICALL Java_hu_conseqtv_Mailbox_getState
  (JNIEnv * env, jobject obj) {
      char buf[50];
      return get_state(buf, 50) ? (*env)->NewStringUTF(env, buf) : (*env)->NewStringUTF(env, "ERR");
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_setState
  (JNIEnv * env, jobject _obj, jstring str) {
      char buf[50];
      const char *ptr;
      strcpy(buf, ptr = (*env)->GetStringUTFChars(env, str, 0));
      (*env)->ReleaseStringUTFChars(env, str, ptr);
      return set_state(buf);
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_sendSignal
  (JNIEnv * env, jobject _obj, jstring str) {
      char buf[50];   
      const char *ptr;
      strcpy(buf, ptr = (*env)->GetStringUTFChars(env, str, 0));
      (*env)->ReleaseStringUTFChars(env, str, ptr);
      return send_signal(buf);
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_setTimeA
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_A(time);
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_setTimeB
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_B(time);
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_setTimeC
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_C(time);
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_setTimeC_1
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_C_(time);
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_setTimeD
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_D(time);
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_setTimeE
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_E(time);
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_setTimeF
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_F(time);
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_setTimeG
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_G(time);
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_setTimeH
  (JNIEnv * env, jobject _obj, jint time) {
      return set_time_H(time);
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_testRed
  (JNIEnv * env, jobject obj) {
      return test_red();
  }

JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_testYlw
  (JNIEnv * env, jobject obj) {
      return test_ylw();
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_testGrn
  (JNIEnv * env, jobject obj) {
      return test_grn();
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_testPred
  (JNIEnv * env, jobject obj) {
      return test_pred();
  }
JNIEXPORT jint JNICALL Java_hu_conseqtv_Mailbox_testPgrn
  (JNIEnv * env, jobject obj) {
      return test_pgrn();
  }