#pragma once

#include <libudev.h>

namespace HW::uDev {
class uDev final {
 private:
  uDev();
  ~uDev();
  uDev(const uDev&) = delete;
  uDev(uDev&&) = delete;
  uDev& operator=(const uDev&) = delete;
  uDev& operator=(uDev&&) = delete;
  struct udev* _udev_;

 public:
  udev* get() { return _udev_; }
  static uDev& Instance();
};

class uDevMonitor {
 private:
  struct udev_monitor* mon;

 public:
  uDevMonitor(uDev& uDev, const char* name);

  int filter_add_match_subsystem_devtype(const char* subsystem);
  int enable_receiving();
  udev_device* receive_device();
  int get_fd();
  ~uDevMonitor();
};

class uDevice {
 public:
  udev_device* device;

  uDevice(udev_device* device);
  uDevice(udev* udev, const char* path);

  /* retrieve device properties */
  const char* get_devpath() const;
  const char* get_subsystem() const;
  const char* get_devtype() const;
  const char* get_syspath() const;
  const char* get_sysname() const;
  const char* get_sysnum() const;
  const char* get_devnode() const;
  int is_initialized();
  const char* get_action() const;
  udev_device* get_parrent() const;
  const char* get_property();
  const char* get_get_sysattr(const char* attr);
  ~uDevice();
};

class uDevEnumerate {
  udev_enumerate* enumerate;

 public:
  uDevEnumerate(uDev& uDev) : enumerate{udev_enumerate_new(uDev.get())} {}
  int add_match_subsystem(const char* subsystem) {
    return udev_enumerate_add_match_subsystem(enumerate, subsystem);
  }
  int scan_devices() { return udev_enumerate_scan_devices(enumerate); }

  udev_list_entry* get_list_entry() {
    return udev_enumerate_get_list_entry(enumerate);
  }
  ~uDevEnumerate() { udev_enumerate_unref(enumerate); }
};
}  // namespace HW::uDev