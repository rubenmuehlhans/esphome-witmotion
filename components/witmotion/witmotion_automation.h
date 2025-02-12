#pragma once
#ifdef USE_ESP32

#include "esphome/core/automation.h"

namespace esphome {
namespace witmotion {

class UpdateTrigger : public Trigger<> {
 public:
  void process() { this->trigger(); }
};

}  // namespace witmotion
}  // namespace esphome

#endif // USE_ESP32
