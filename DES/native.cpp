#include "des.h"
#include "jni.h"

#define DES_KEY "jvn4l8B2Uly81YdYUKFoLbLhYD0gF3pQGdItoyJDLc9kx95b"

//jstring to char*
char *jstringTostring(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    if (jstr == NULL) {
        return NULL;
    }
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

extern "C" JNIEXPORT jstring
JNICALL
Java_com_k3dx_game_zlib_NativeLib_desEncode(
        JNIEnv *env,
        jobject,jstring src) {
    // jstring 转 char*
    char* chardata = jstringTostring(env, src);
    // char* 转 string
    std::string origin = chardata;
    auto my3Des = new Des(DES_KEY);
    std::string result=my3Des->desEncrypt(origin);
    return env->NewStringUTF(result.c_str());
}

extern "C" JNIEXPORT jstring
JNICALL
Java_com_k3dx_game_zlib_NativeLib_desDecode(
        JNIEnv *env,
        jobject,jstring src) {
    // jstring 转 char*
    char* chardata = jstringTostring(env, src);
    // char* 转 string
    std::string origin = chardata;
    auto my3Des = new Des(DES_KEY);
    std::string result=my3Des->desDecrypt(origin);
    return env->NewStringUTF(result.c_str());
}