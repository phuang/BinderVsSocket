#ifndef __IECHO_H_
#define __IECHO_H_

#include <binder/IInterface.h>
#include <utils/String16.h>

namespace android {

class IEcho : public IInterface {
public:
  DECLARE_META_INTERFACE(Echo);

  virtual int echoInt(int i) = 0;

protected:
  enum {
    ECHO_INT = IBinder::FIRST_CALL_TRANSACTION,
    ECHO_STR,
    ECHO_STR_CB,
  };
};

class BnEcho : public BnInterface<IEcho> {
public:
  virtual status_t onTransact(uint32_t code, const Parcel& data,
                              Parcel* reply, uint32_t flags = 0);
};

}; // namespace android

#endif  // __IECHO_H_
