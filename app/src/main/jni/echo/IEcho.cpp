#include <binder/Parcel.h>

#include "IEcho.h"

namespace android {

class BpEcho : public BpInterface<IEcho> {
public:
  BpEcho(const sp<IBinder>& impl)
    : BpInterface<IEcho>(impl) {}
  virtual ~BpEcho() {}

  int echoInt(int i) {
    Parcel data, reply;
    data.writeInterfaceToken(IEcho::getInterfaceDescriptor());
    data.writeInt32(i);
    remote()->transact(ECHO_INT, data, &reply);
    return reply.readInt32();
  }

  String16 echoStr(const String16& str) {
    Parcel data, reply;
    data.writeInterfaceToken(IEcho::getInterfaceDescriptor());
    data.writeString16(str);
    remote()->transact(ECHO_STR, data, &reply);
    return reply.readString16();
  }
};

IMPLEMENT_META_INTERFACE(Echo, "Echo");

status_t BnEcho::onTransact(uint32_t code, const Parcel& data,
                            Parcel* reply, uint32_t flags) {
  switch(code) {
    case ECHO_INT: 
      {
        CHECK_INTERFACE(IEcho, data, reply);
        int r = echoInt(data.readInt32());
        reply->writeInt32(r);
        return NO_ERROR;
      }
    case ECHO_STR:
      {
        CHECK_INTERFACE(IEcho, data, reply);
        reply->writeString16(data.readString16());
        return NO_ERROR;
      }
    default:
      return BBinder::onTransact(code, data, reply, flags);
  }
}

};
