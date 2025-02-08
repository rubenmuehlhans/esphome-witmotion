#pragma once
#ifdef USE_ESP32

namespace esphome {
namespace witmotion {

namespace espbt = esphome::esp32_ble_tracker;

// GATT Attributes
//
// Service - Generic Attribute Profile
//         UUID: 00001801-0000-1000-8000-00805f9b34fb
//         Primary: yes

// Service - Vendor specific
//         UUID: 0000ffe5-0000-1000-8000-00805f9a34fb
//         Primary: yes
static const espbt::ESPBTUUID WITMOTION_SERVICE_UUID = espbt::ESPBTUUID::from_raw("0000ffe5-0000-1000-8000-00805f9a34fb");

// Characteristic - Vendor specific
//         UUID: 0000ffe9-0000-1000-8000-00805f9a34fb
//         Service: /org/bluez/hci0/dev_DF_BE_12_20_70_8C/service000b
//         Flags: write-without-response
//         Flags: write
//         MTU: 0x00f7
static const espbt::ESPBTUUID WITMOTION_COMMAND_UUID = espbt::ESPBTUUID::from_raw("0000ffe9-0000-1000-8000-00805f9a34fb");

// Characteristic - Vendor specific
//         UUID: 0000ffe4-0000-1000-8000-00805f9a34fb
//         Service: /org/bluez/hci0/dev_DF_BE_12_20_70_8C/service000b
//         Notifying: no
//         Flags: notify
//         MTU: 0x00f7
static const espbt::ESPBTUUID WITMOTION_STATUS_UUID = espbt::ESPBTUUID::from_raw("0000ffe4-0000-1000-8000-00805f9a34fb");

// Descriptor - Client Characteristic Configuration
//         UUID: 00002902-0000-1000-8000-00805f9b34fb
//         Characteristic: /org/bluez/hci0/dev_DF_BE_12_20_70_8C/service000b/char000e

}  // namespace witmotion
}  // namespace esphome

#endif // USE_ESP32
