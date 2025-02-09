#ifdef USE_ESP32

#include <algorithm>
#include <cmath>

#include "witmotion.h"
#include "witmotion_struct.h"
#include "witmotion_ble.h"

#include "esphome/core/log.h"

namespace esphome {
namespace witmotion {

namespace espbt = esphome::esp32_ble_tracker;

//TODO remove
/// Log the contents of a variable length packet as hex bytes
void logIHexPacket(char* label, const uint8_t* buf, size_t length) {
  char output[(length * 3) + 1];

  size_t offset = 0;
  for (size_t i = 0; i < length; ++i) {
    offset += sprintf(output + offset, "%02X ", buf[i]);
  }

  ESP_LOGI(TAG, "%s (%d bytes): %s", label, length, output);
}

/* Component overrides */

void WitMotion::dump_config() {
  //TODO see examples
}

void WitMotion::loop() {
}

void WitMotion::setup() {
  if (sizeof(WitMotionData) != RAW_DATA_BUFFER_SIZE) {
    ESP_LOGE(TAG, "Data structure error: sizeof(WitMotionData) != RAW_DATA_BUFFER_SIZE");
  }

  if (sizeof(WitMotionCommand) != RAW_COMMAND_BUFFER_SIZE) {
    ESP_LOGE(TAG, "Data structure error: sizeof(WitMotionCommand) != RAW_COMMAND_BUFFER_SIZE");
  }
}

/* BLEClient overrides */

void WitMotion::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                       esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    //TODO handle disconnect

    case ESP_GATTC_SEARCH_CMPL_EVT:
      this->connected_ = this->get_char_handles() && this->configure_notifications();
      if (this->connected_) {
        this->read_one_time_data(VERSION);
      }
      break;

    case ESP_GATTC_NOTIFY_EVT:
      this->process_notification(param);
      break;

    case ESP_GATTC_WRITE_CHAR_EVT:
      if ((param->write.conn_id == this->parent_->get_conn_id()) && (param->write.status != ESP_GATT_OK)) {
        ESP_LOGE(TAG, "esp_ble_gattc_write_char failed, handle=%d, status=%d", param->write.handle, param->write.status);
      }
      break;

    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      if (param->write.status != ESP_GATT_OK) {
        ESP_LOGE(TAG, "esp_ble_gattc_reg_for_notify failed, handle=%d, status=%d",
                 param->reg_for_notify.handle, param->reg_for_notify.status);
      }
      break;
    }
  }
}

/* Our interface */

void WitMotion::set_update_rate(RateArg rate) {
  this->update_rate_ = rate;
}

void WitMotion::set_battery_level(sensor::Sensor *battery_level) {
  this->battery_level_ = battery_level;
  this->add_extra_data(BATTERY);
}

void WitMotion::set_battery_voltage(sensor::Sensor *battery_voltage) {
  this->battery_voltage_ = battery_voltage;
  this->add_extra_data(BATTERY);
}

void WitMotion::set_magnetic_flux_density(sensor::Sensor *magnetic_flux_density) {
  this->magnetic_flux_density_ = magnetic_flux_density;
  this->add_extra_data(HX);
}

void WitMotion::set_magnetic_flux_density_x(sensor::Sensor *magnetic_flux_density_x) {
  this->magnetic_flux_density_x_ = magnetic_flux_density_x;
  this->add_extra_data(HX);
}

void WitMotion::set_magnetic_flux_density_y(sensor::Sensor *magnetic_flux_density_y) {
  this->magnetic_flux_density_y_ = magnetic_flux_density_y;
  this->add_extra_data(HX);
}

void WitMotion::set_magnetic_flux_density_z(sensor::Sensor *magnetic_flux_density_z) {
  this->magnetic_flux_density_z_ = magnetic_flux_density_z;
  this->add_extra_data(HX);
}

void WitMotion::set_quaternion_0(sensor::Sensor *quaternion_0) {
  this->quaternion_0_ = quaternion_0;
  this->add_extra_data(Q0);
}

void WitMotion::set_quaternion_1(sensor::Sensor *quaternion_1) {
  this->quaternion_1_ = quaternion_1;
  this->add_extra_data(Q0);
}

void WitMotion::set_quaternion_2(sensor::Sensor *quaternion_2) {
  this->quaternion_2_ = quaternion_2;
  this->add_extra_data(Q0);
}

void WitMotion::set_quaternion_3(sensor::Sensor *quaternion_3) {
  this->quaternion_3_ = quaternion_3;
  this->add_extra_data(Q0);
}

void WitMotion::set_temperature(sensor::Sensor *temperature) {
  this->temperature_ = temperature;
  this->add_extra_data(TEMP);
}

/* Internals */

void WitMotion::add_extra_data(RegisterNumber reg) {
  if (find(this->extra_data_reg_.begin(), this->extra_data_reg_.end(), reg) ==
      this->extra_data_reg_.end()) {
    this->extra_data_reg_.push_back(reg);
  }
}

bool WitMotion::configure_notifications() {
  WitMotionCommand cmd = {0};
  cmd.set_rate.compose(this->update_rate_);
  uint8_t status = esp_ble_gattc_write_char(this->parent_->get_gattc_if(), this->parent_->get_conn_id(),
                                            this->command_handle_, sizeof(cmd), cmd.raw.ptr(),
                                            ESP_GATT_WRITE_TYPE_NO_RSP, ESP_GATT_AUTH_REQ_NONE);
  if (status) {
    ESP_LOGE(TAG, "esp_ble_gattc_write_char failed, handle=%d, status=%d", this->command_handle_, status);
    return false;
  }

  status = esp_ble_gattc_register_for_notify(this->parent_->get_gattc_if(), this->parent_->get_remote_bda(),
                                               this->status_handle_);
  if (status) {
    ESP_LOGE(TAG, "esp_ble_gattc_register_for_notify failed, handle=%d, status=%d", this->status_handle_, status);
    return false;
  }

  return true;
}

void WitMotion::continue_reading_extra_data() {
  if (this->reading_extra_data_ && (this->extra_data_reg_.size() > 0)) {
    if (++this->extra_data_index_ < this->extra_data_reg_.size()) {
      this->read_one_time_data(this->extra_data_reg_[this->extra_data_index_]);
    } else {
      this->reading_extra_data_ = false;
    }
  }
}

void WitMotion::dispatch_acceleration() {
  if (this->acceleration_x_) {
    this->acceleration_x_->publish_state(this->ax_);
  }

  if (this->acceleration_y_) {
    this->acceleration_y_->publish_state(this->ay_);
  }

  if (this->acceleration_z_) {
    this->acceleration_z_->publish_state(this->az_);
  }

  if (this->acceleration_) {
    float mag = sqrt(pow(double(this->ax_), 2.0) + pow(double(this->ay_), 2.0) + pow(double(this->az_), 2.0));
    this->acceleration_->publish_state(mag);
  }
}

void WitMotion::dispatch_angle() {
  if (this->roll_angle_) {
    this->roll_angle_->publish_state(this->roll_);
  }

  if (this->pitch_angle_) {
    this->pitch_angle_->publish_state(this->pitch_);
  }

  if (this->yaw_angle_) {
    this->yaw_angle_->publish_state(this->yaw_);
  }
}

void WitMotion::dispatch_angular_velocity() {
  if (this->angular_velocity_x_) {
    this->angular_velocity_x_->publish_state(this->wx_);
  }

  if (this->angular_velocity_y_) {
    this->angular_velocity_y_->publish_state(this->wy_);
  }

  if (this->angular_velocity_z_) {
    this->angular_velocity_z_->publish_state(this->wz_);
  }

  if (this->angular_velocity_) {
    float mag = sqrt(pow(double(this->wx_), 2.0) + pow(double(this->wy_), 2.0) + pow(double(this->wz_), 2.0));
    this->angular_velocity_->publish_state(mag);
  }
}

void WitMotion::dispatch_battery() {
    if (this->battery_level_) {
      this->battery_level_->publish_state(BatteryVoltageToLevel(this->battery_volts_));
    }

    if (this->battery_voltage_) {
      this->battery_voltage_->publish_state(this->battery_volts_);
    }
}

void WitMotion::dispatch_magnetic_flux_density() {
  if (this->magnetic_flux_density_x_) {
    this->magnetic_flux_density_x_->publish_state(this->hx_);
  }

  if (this->magnetic_flux_density_y_) {
    this->magnetic_flux_density_y_->publish_state(this->hy_);
  }

  if (this->magnetic_flux_density_z_) {
    this->magnetic_flux_density_z_->publish_state(this->hz_);
  }

  if (this->magnetic_flux_density_) {
    float mag = sqrt(pow(double(this->hx_), 2.0) + pow(double(this->hy_), 2.0) + pow(double(this->hz_), 2.0));
    this->magnetic_flux_density_->publish_state(mag);
  }
}

void WitMotion::dispatch_quaternion() {
  if (this->quaternion_0_) {
    this->quaternion_0_->publish_state(this->q0_);
  }

  if (this->quaternion_1_) {
    this->quaternion_1_->publish_state(this->q1_);
  }

  if (this->quaternion_2_) {
    this->quaternion_2_->publish_state(this->q2_);
  }

  if (this->quaternion_3_) {
    this->quaternion_3_->publish_state(this->q3_);
  }
}

void WitMotion::dispatch_temperature() {
  if (this->temperature_) {
    this->temperature_->publish_state(this->temp_);
  }
}

void WitMotion::extract_data_and_dispatch(WitMotionData const *data) {
  if (data->header.flag == DEFAULT_DATA) {
    data->default_data.decode_acceleration(&this->ax_, &this->ay_, &this->az_);
    this->dispatch_acceleration();

    data->default_data.decode_angular_velocity(&this->wx_, &this->wy_, &this->wz_);
    this->dispatch_angular_velocity();

    data->default_data.decode_angle(&this->roll_, &this->pitch_, &this->yaw_);
    this->dispatch_angle();
  } else {
    switch (data->single_return.RegL) {
      case VERSION:
        data->version.decode(&this->version_no_);
        break;

      case HX:
        data->magnetic_field.decode(&this->hx_, &this->hy_, &this->hz_);
        this->dispatch_magnetic_flux_density();
        break;

      case Q0:
        data->quaternion.decode(&this->q0_, &this->q1_, &this->q2_, &this->q3_);
        this->dispatch_quaternion();
        break;

      case TEMP:
        data->temperature.decode(&this->temp_);
        this->dispatch_temperature();
        break;

      case BATTERY:
        data->battery.decode(&this->battery_volts_);
        this->dispatch_battery();
        break;
    }
  }
}

bool WitMotion::get_char_handles() {
  bool success = true;
  esphome::ble_client::BLECharacteristic *chr;

  chr = this->parent_->get_characteristic(WITMOTION_SERVICE_UUID, WITMOTION_COMMAND_UUID);
  if (chr == nullptr) {
    ESP_LOGE(TAG, "Command characteristic not found");
    success = false;
  } else {
    this->command_handle_ = chr->handle;
  }

  chr = this->parent_->get_characteristic(WITMOTION_SERVICE_UUID, WITMOTION_STATUS_UUID);
  if (chr == nullptr) {
    ESP_LOGE(TAG, "Status characteristic not found");
    success = false;
  } else {
    this->status_handle_ = chr->handle;
  }

  return success;
}

bool WitMotion::is_valid_data(WitMotionData const *data){
  return (data->header.heading == HEADING) &&
         ((data->header.flag == DEFAULT_DATA) || (data->header.flag == SINGLE_RETURN_DATA));
}

void WitMotion::process_notification(esp_ble_gattc_cb_param_t *param) {
  // At higher update rates, we sometimes see multiple records combined into a single notification.
  uint8_t *value = param->notify.value;
  uint8_t value_len = param->notify.value_len;

  while (value_len >= sizeof(WitMotionData)) {
    WitMotionData const *data = reinterpret_cast<WitMotionData const *>(value);

    if (this->is_valid_data(data)) {
      this->extract_data_and_dispatch(data);

      if (data->header.flag == DEFAULT_DATA) {
        this->start_reading_extra_data();
      } else {
        this->continue_reading_extra_data();
      }
    } else {
      break;
    }

    value += sizeof(WitMotionData);
    value_len -= sizeof(WitMotionData);
  }

  //TODO remove
  uint8_t const *data2 = reinterpret_cast<uint8_t const *>(param->notify.value);
  logIHexPacket("Notification:", data2, param->notify.value_len);
}

void WitMotion::read_one_time_data(RegisterNumber reg) {
  WitMotionCommand cmd = {0};

  cmd.read_register.compose(reg);
  uint8_t status = esp_ble_gattc_write_char(this->parent_->get_gattc_if(), this->parent_->get_conn_id(),
                                            this->command_handle_, sizeof(cmd), cmd.raw.ptr(),
                                            ESP_GATT_WRITE_TYPE_NO_RSP, ESP_GATT_AUTH_REQ_NONE);
  if (status) {
    ESP_LOGE(TAG, "esp_ble_gattc_write_char failed, handle=%d, status=%d", this->command_handle_, status);
  }
}

void WitMotion::start_reading_extra_data() {
  this->extra_data_index_ = 0;
  if (!this->reading_extra_data_ && (this->extra_data_reg_.size() > 0)) {
    this->reading_extra_data_ = true;
    this->read_one_time_data(this->extra_data_reg_[this->extra_data_index_]);
  }
}

}  // namespace witmotion
}  // namespace esphome

#endif // USE_ESP32
