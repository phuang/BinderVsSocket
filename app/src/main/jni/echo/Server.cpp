#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>

#include "IEcho.h"

using namespace android;

class Echo : public BnEcho {
public:
  Echo() {}
  ~Echo() {}
  int echoInt(int i) {
    return i;
  }

  void run() {
    do {
      sleep(1);
    } while (true);
  }
};

int binder_server_main() {

  // ProcessState::self()->setThreadPoolMaxThreadCount(4);

  // start the thread pool
  sp<ProcessState> ps(ProcessState::self());

  // instantiate surfaceflinger
  sp<Echo> echo = new Echo();
  
  // publish surface flinger
  sp<IServiceManager> sm(defaultServiceManager());
  sm->addService(String16("Echo"), echo, false);
  return 0;
}
