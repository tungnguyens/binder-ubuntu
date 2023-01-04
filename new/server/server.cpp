#include <ICalculation/BnCalculation.h>
#include <atomic>
#include <binder/Binder.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <binder/ProcessState.h>
#include <csignal>
#include <timercpp.h>

using namespace android;
using namespace std;
using namespace ICalculation;

String16 serviceName("binder.Calculation");

#define INFO(...)                                                              \
  do {                                                                         \
    printf(__VA_ARGS__);                                                       \
    printf("\n");                                                              \
    ALOGD(__VA_ARGS__);                                                        \
  } while (0)

class Calculation : public BnCalculation {
private:
  int32_t _value;

public:
  int32_t get_value() { return _value; }
  virtual android::status_t Get(int32_t *_aidl_return) {
    INFO("Recived GET Request");
    *_aidl_return = _value;
    return android::OK;
  }
  virtual android::status_t Set(int32_t value, int32_t *_aidl_return) {
    INFO("Recived Set Request %d", value);
    _value = value;
    *_aidl_return = _value;
    return android::OK;
  }
  virtual android::status_t Add(int32_t value, int32_t *_aidl_return) {
    INFO("Recived Add Request %d", value);
    _value += value;
    *_aidl_return = _value;
    return android::OK;
  }
  virtual android::status_t Multi(int32_t value, int32_t *_aidl_return) {
    INFO("Recived Multi Request %d", value);
    _value *= value;
    *_aidl_return = _value;
    return android::OK;
  }
};

void signal_handler(int signal_num) {
  cout << "The interrupt signal is (" << signal_num << "). \n";

  // It terminates the  program
  exit(signal_num);
}

Calculation *pCalc = nullptr;

int main() {
  // Signal
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  pCalc = new Calculation();

  Timer t;
  t.setInterval([&]() { cout << "value = " << pCalc->get_value() << endl; },
                1000);

  int rv;

  // Add the service
  sp<ProcessState> proc(ProcessState::self());
  sp<IServiceManager> sm = defaultServiceManager();
  if ((rv = sm->addService(serviceName, pCalc)) != 0) {
    cerr << "addService " << serviceName << " failed, rv: " << rv
         << " errno: " << errno << endl;
  }

  INFO("Server start");
  // Start threads to handle server work
  proc->startThreadPool();

  IPCThreadState::self()->joinThreadPool();

  return 0;
}
