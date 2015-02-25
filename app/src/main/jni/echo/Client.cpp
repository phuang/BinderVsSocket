#include <time.h>

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>

#include "IEcho.h"

using namespace android;

#define COUNT (100000)

double clock_now() {
  struct timespec ts;
  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
    return 0;
  }
  return ts.tv_sec + (((double)ts.tv_nsec) / 1000000000);
}

int binder_client_main(int fd) {
   double result = -3;
  // ProcessState::self()->setThreadPoolMaxThreadCount(4);

  // start the thread pool
  sp<ProcessState> ps(ProcessState::self());
  // ps->startThreadPool();
  
  // publish surface flinger
  sp<IServiceManager> sm(defaultServiceManager());

  sp<IEcho> echo = interface_cast<IEcho>(sm->getService(String16("Echo")));

  double begin = clock_now();
  int r = 0;
  for (int i = 0; i < COUNT; i++) {
    if (echo != NULL)
        r = echo->echoInt(i);
  }

  double dur =  clock_now() - begin;
  write(fd, &dur, sizeof(dur));
  return 0;
}
