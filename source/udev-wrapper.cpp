#include "udev-wrapper.hpp"

namespace HW::uDev {
uDev::uDev() : _udev_{udev_new()} {
  // if (nullptr == _udev_)
  //   throw std::runtime_error("Не вдалося ініціалізувати udev");
}
uDev::~uDev() { udev_unref(_udev_); }

uDev& uDev::Instance() {
  static uDev theInstance{};
  return theInstance;
}

/*******/

uDevMonitor::uDevMonitor(uDev& uDev, const char* name)
    : mon{udev_monitor_new_from_netlink(uDev.get(), name)} {}

uDevMonitor::~uDevMonitor() { udev_monitor_unref(mon); }

int uDevMonitor::get_fd() { return udev_monitor_get_fd(mon); }
int uDevMonitor::enable_receiving() {
  return udev_monitor_enable_receiving(mon);
}
int uDevMonitor::filter_add_match_subsystem_devtype(const char* subsystem) {
  return udev_monitor_filter_add_match_subsystem_devtype(mon, subsystem,
                                                         nullptr);
}
udev_device* uDevMonitor::receive_device() {
  return udev_monitor_receive_device(mon);
}

uDevice::uDevice(udev_device* device) : device{device} {}
uDevice::uDevice(udev* udev, const char* path)
    : device{udev_device_new_from_syspath(udev, path)} {}
uDevice::~uDevice() { udev_device_unref(device); }

const char* uDevice::get_devnode() const {
  return udev_device_get_devnode(device);
}
const char* uDevice::get_action() const {
  return udev_device_get_action(device);
}

const char* uDevice::get_property() {
  return udev_device_get_property_value(device, "NAME");
}

udev_device* uDevice::get_parrent() const {
  return udev_device_get_parent_with_subsystem_devtype(device, "usb",
                                                       "usb_device");
}

const char* uDevice::get_get_sysattr(const char* attr) {
  return udev_device_get_sysattr_value(get_parrent(), attr);
}

/*
class uDevMonitor {
 private:
  struct udev_monitor* mon;

 public:
  uDevMonitor(uDev& uDev, const char* name)
      : mon{udev_monitor_new_from_netlink(uDev.get(), name)} {
    // BOOST_LOG_TRIVIAL(debug) << __PRETTY_FUNCTION__;
  }

  int filter_add_match_subsystem_devtype(const char* subsystem) {
    return udev_monitor_filter_add_match_subsystem_devtype(mon, subsystem,
                                                           nullptr);
  }

  int enable_receiving() { return udev_monitor_enable_receiving(mon); }
  udev_device* receive_device() { return udev_monitor_receive_device(mon); }
  int get_fd() { return udev_monitor_get_fd(mon); }
  ~uDevMonitor() {
    // BOOST_LOG_TRIVIAL(debug) << __PRETTY_FUNCTION__;
    udev_monitor_unref(mon);
  }
};
 *
 */

}  // namespace HW::uDev
