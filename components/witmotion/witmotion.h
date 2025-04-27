#pragma once
#ifdef USE_ESP32

#include <vector>

#include "witmotion_automation.h"
#include "witmotion_struct.h"

#include "esphome/core/component.h"
#include "esphome/core/time.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/sensor/sensor.h"
#ifdef USE_TIME
#include "esphome/components/time/real_time_clock.h"
#endif

namespace esphome {
namespace witmotion {

static const char *const TAG = "witmotion";

class WitMotion : public Component, public esphome::ble_client::BLEClientNode {

 public:
  /* Component overrides */

  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::BLUETOOTH; }
  void loop() override;
  void setup() override;

  /* BLEClient overrides */

  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;

  /* Our interface */

  void set_update_rate(RateArg rate);

#ifdef USE_TIME
  void set_time_id(time::RealTimeClock *time_id) { this->time_id_ = time_id; }
#endif

  void register_onupdate_trigger(UpdateTrigger *trig) { triggers_onupdate_.push_back(trig); }
  void set_trigger_every(int every) { trigger_every = every; }

  void set_acceleration(sensor::Sensor *acceleration) { acceleration_ = acceleration; }
  void set_acceleration_x(sensor::Sensor *acceleration_x) { acceleration_x_ = acceleration_x; }
  void set_acceleration_y(sensor::Sensor *acceleration_y) { acceleration_y_ = acceleration_y; }
  void set_acceleration_z(sensor::Sensor *acceleration_z) { acceleration_z_ = acceleration_z; }
  void set_angular_velocity(sensor::Sensor *angular_velocity) { angular_velocity_ = angular_velocity; }
  void set_angular_velocity_x(sensor::Sensor *angular_velocity_x) { angular_velocity_x_ = angular_velocity_x; }
  void set_angular_velocity_y(sensor::Sensor *angular_velocity_y) { angular_velocity_y_ = angular_velocity_y; }
  void set_angular_velocity_z(sensor::Sensor *angular_velocity_z) { angular_velocity_z_ = angular_velocity_z; }
  void set_roll_angle(sensor::Sensor *roll_angle) { roll_angle_ = roll_angle; }
  void set_pitch_angle(sensor::Sensor *pitch_angle) { pitch_angle_ = pitch_angle; }
  void set_yaw_angle(sensor::Sensor *yaw_angle) { yaw_angle_ = yaw_angle; }
  void set_battery_level(sensor::Sensor *battery_level);
  void set_battery_voltage(sensor::Sensor *battery_voltage);
  void set_magnetic_flux_density(sensor::Sensor *magnetic_flux_density);
  void set_magnetic_flux_density_x(sensor::Sensor *magnetic_flux_density_x);
  void set_magnetic_flux_density_y(sensor::Sensor *magnetic_flux_density_y);
  void set_magnetic_flux_density_z(sensor::Sensor *magnetic_flux_density_z);
  void set_quaternion_norm(sensor::Sensor *quaternion_norm);
  void set_quaternion_0(sensor::Sensor *quaternion_0);
  void set_quaternion_1(sensor::Sensor *quaternion_1);
  void set_quaternion_2(sensor::Sensor *quaternion_2);
  void set_quaternion_3(sensor::Sensor *quaternion_3);
  void set_temperature(sensor::Sensor *temperature);

 protected:

  RateArg update_rate_{RATE_0_2HZ};

  void process_onupdate_triggers();
  std::vector<UpdateTrigger *> triggers_onupdate_;
  int trigger_every{1};
  int trigger_countdown{1};

  sensor::Sensor *acceleration_{nullptr};
  sensor::Sensor *acceleration_x_{nullptr};
  sensor::Sensor *acceleration_y_{nullptr};
  sensor::Sensor *acceleration_z_{nullptr};
  sensor::Sensor *angular_velocity_{nullptr};
  sensor::Sensor *angular_velocity_x_{nullptr};
  sensor::Sensor *angular_velocity_y_{nullptr};
  sensor::Sensor *angular_velocity_z_{nullptr};
  sensor::Sensor *roll_angle_{nullptr};
  sensor::Sensor *pitch_angle_{nullptr};
  sensor::Sensor *yaw_angle_{nullptr};
  sensor::Sensor *battery_level_{nullptr};
  sensor::Sensor *battery_voltage_{nullptr};
  sensor::Sensor *magnetic_flux_density_{nullptr};
  sensor::Sensor *magnetic_flux_density_x_{nullptr};
  sensor::Sensor *magnetic_flux_density_y_{nullptr};
  sensor::Sensor *magnetic_flux_density_z_{nullptr};
  sensor::Sensor *quaternion_norm_{nullptr};
  sensor::Sensor *quaternion_0_{nullptr};
  sensor::Sensor *quaternion_1_{nullptr};
  sensor::Sensor *quaternion_2_{nullptr};
  sensor::Sensor *quaternion_3_{nullptr};
  sensor::Sensor *temperature_{nullptr};

  bool get_char_handles();
  uint16_t status_handle_{0};
  uint16_t command_handle_{0};

  bool configure_notifications();
  void process_notification(esp_ble_gattc_cb_param_t *param);
  bool is_valid_data(WitMotionData const *data);

  void add_extra_data(RegisterNumber reg);
  void start_reading_extra_data();
  void continue_reading_extra_data();
  void read_one_time_data(RegisterNumber reg);
  std::vector<RegisterNumber> extra_data_reg_{};
  bool reading_extra_data_{false};
  int extra_data_index_{0};

  void extract_data_and_dispatch(WitMotionData const *data);
  void dispatch_acceleration();
  void dispatch_angle();
  void dispatch_angular_velocity();
  void dispatch_battery();
  void dispatch_magnetic_flux_density();
  void dispatch_quaternion();
  void dispatch_temperature();
  uint16_t version_no_{0};
  float ax_{0.0};
  float ay_{0.0};
  float az_{0.0};
  float wx_{0.0};
  float wy_{0.0};
  float wz_{0.0};
  float roll_{0.0};
  float pitch_{0.0};
  float yaw_{0.0};
  float hx_{0.0};
  float hy_{0.0};
  float hz_{0.0};
  float q0_{0.0};
  float q1_{0.0};
  float q2_{0.0};
  float q3_{0.0};
  float temp_{0.0};
  float battery_volts_{0.0};

#ifdef USE_TIME
  void set_clock();
  void setup_time();
  time::RealTimeClock *time_id_{nullptr};
#endif
};

}  // namespace witmotion
}  // namespace esphome

#endif // USE_ESP32
