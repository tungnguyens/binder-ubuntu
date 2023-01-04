#include <ICalculation/BnCalculation.h>
#include <binder/Binder.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <binder/ProcessState.h>
#include <csignal>
#include <iostream>
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

class MyServiceClient : public IBinder::DeathRecipient {
public:
  MyServiceClient() {}

  // DeathRecipient
  virtual void binderDied(const android::wp<IBinder> &who);
};

void MyServiceClient::binderDied(const wp<IBinder> &who) {
  INFO("server died!\n");
}

sp<MyServiceClient> sServiceClient;

// Helper function to get a hold of the "Demo" service.
sp<ICalculation::ICalculation> getService() {
  sp<IServiceManager> sm = defaultServiceManager();
  if (sm == 0) {
    INFO("Error ! Cannot get Service Manager");
    exit(-1);
  }
  sp<IBinder> binder = sm->getService(String16(serviceName));

  if (binder == 0) {
    INFO("Error ! Not found any Service %s", serviceName);
    exit(-1);
  }

  sp<ICalculation::ICalculation> ret =
      interface_cast<ICalculation::ICalculation>(binder);

  return ret;
}

// Helper function to get a hold of the "Demo" service.
sp<IBinder> getBinder() {
  sp<IServiceManager> sm = defaultServiceManager();
  if (sm == 0) {
    INFO("Error ! Cannot get Service Manager");
    exit(-1);
  }
  sp<IBinder> binder = sm->getService(String16(serviceName));

  if (binder == 0) {
    INFO("Error ! Not found any Service %s", serviceName);
    exit(-1);
  }

  return binder;
}

void signal_handler(int signal_num) {
  cout << "The interrupt signal is (" << signal_num << "). \n";

  // It terminates the  program
  exit(signal_num);
}

int main() {
  // Signal
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  int rv;

  sp<ICalculation::ICalculation> binder_service = getService();

  if (binder_service != nullptr) {
    int32_t value = 0;
    binder_service->Get(&value);
    INFO("Get: value = %d", value);
    // binder_service->Set(10, &value);
    // INFO("Set: value = %d", value);
    binder_service->Add(10, &value);
    INFO("Add: value = %d", value);
    binder_service->Multi(10, &value);
    INFO("Multi: value = %d", value);
  } else {
    return 1;
  }

  // we need a thread pool to receive binder callbacks (e.g., for
  // DeathRecipient)
  sp<MyServiceClient> sServiceClient;

  if (sServiceClient == NULL)
    sServiceClient = new MyServiceClient();

  sp<IBinder> binder = getBinder();
  binder->linkToDeath(sServiceClient); // returns 0

  // ping
  Timer t;
  t.setInterval(
      [&]() {
        auto ret = binder->pingBinder();
        cout << "ping = " << ret << endl;
      },
      1000);

  sp<ProcessState> proc(ProcessState::self());
  ProcessState::self()->startThreadPool();
  IPCThreadState::self()->joinThreadPool();

  return 0;
}
