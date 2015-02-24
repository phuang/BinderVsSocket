#include "com_penghuang_bindervssocket_MainActivity.h"

double socket_main();

JNIEXPORT jdouble JNICALL
Java_com_penghuang_bindervssocket_MainActivity_socketTest(JNIEnv *env, jobject obj) {
    return socket_main();
}

JNIEXPORT jdouble JNICALL
Java_com_penghuang_bindervssocket_MainActivity_binderTest (JNIEnv *env, jobject obj) {
    return 0.888;
}