An esphome component to read data from a WitMotion WT901BLECL5.0 BLE 5.0 AHRS IMU sensor.

Reports acceleration, angular velocity, magnetic flux density, roll/pitch/yaw angle,
quaternion, and temperature.


```
esp32_ble_tracker:
  scan_parameters:
    active: true

ble_client:
  - mac_address: "XX:XX:XX:XX:XX:XX"
    id: wt901_client

witmotion:
  id: my_witmotion
  ble_client_id: wt901_client
  update_rate: 10 Hz
  trigger_every: 1
  time_id: ha_time

sensor:
  - platform: witmotion
    witmotion_id: my_witmotion
    acceleration_x:
      name: "Accel X"
    acceleration_y:
      name: "Accel Y"
    acceleration_z:
      name: "Accel Z"
    roll_angle:
      name: "Roll"
    pitch_angle:
      name: "Pitch"
    yaw_angle:
      name: "Yaw"

```
