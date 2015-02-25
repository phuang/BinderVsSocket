#include "com_penghuang_bindervssocket_MainActivity.h"

double socket_main();
double binder_main();


JNIEXPORT jdouble JNICALL
Java_com_penghuang_bindervssocket_MainActivity_socketTest(JNIEnv *env, jclass clazz) {
    // return socket_main();
    return 0.2;
}

JNIEXPORT jdouble JNICALL
Java_com_penghuang_bindervssocket_MainActivity_binderTest (JNIEnv *env, jclass clazz) {
    return binder_main();
}

JNIEXPORT jdouble JNICALL
Java_com_penghuang_bindervssocket_MainActivity_binderTestStartServer (JNIEnv *env, jclass clazz) {
    return 0.0;
}