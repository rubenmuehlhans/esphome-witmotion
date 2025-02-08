import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import ble_client, sensor
from esphome.const import (
    CONF_ACCELERATION,
    CONF_ACCELERATION_X,
    CONF_ACCELERATION_Y,
    CONF_ACCELERATION_Z,
    CONF_ID,
    CONF_TEMPERATURE,
    DEVICE_CLASS_TEMPERATURE,
    ICON_ACCELERATION,
    ICON_ACCELERATION_X,
    ICON_ACCELERATION_Y,
    ICON_ACCELERATION_Z,
    ICON_GYROSCOPE_X,
    ICON_GYROSCOPE_Y,
    ICON_GYROSCOPE_Z,
    ICON_MAGNET,
    ICON_THERMOMETER,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_DEGREE_PER_SECOND,
    UNIT_DEGREES,
    UNIT_G,
    UNIT_MICROTESLA,
)

CODEOWNERS = ["@jpmv27"]
DEPENDENCIES = ["ble_client"]

CONF_ANGULAR_VELOCITY = "angular_velocity"
CONF_ANGULAR_VELOCITY_X = "angular_velocity_x"
CONF_ANGULAR_VELOCITY_Y = "angular_velocity_y"
CONF_ANGULAR_VELOCITY_Z = "angular_velocity_z"
CONF_MAGNETIC_FLUX_DENSITY = "magnetic_flux_density"
CONF_MAGNETIC_FLUX_DENSITY_X = "magnetic_flux_density_x"
CONF_MAGNETIC_FLUX_DENSITY_Y = "magnetic_flux_density_y"
CONF_MAGNETIC_FLUX_DENSITY_Z = "magnetic_flux_density_z"
CONF_PITCH_ANGLE = "pitch_angle"
CONF_QUATERNION_0 = "quaternion_0"
CONF_QUATERNION_1 = "quaternion_1"
CONF_QUATERNION_2 = "quaternion_2"
CONF_QUATERNION_3 = "quaternion_3"
CONF_ROLL_ANGLE = "roll_angle"
CONF_UPDATE_RATE = "update_rate"
CONF_YAW_ANGLE = "yaw_angle"

witmotion_ns = cg.esphome_ns.namespace("witmotion")
WitMotion = witmotion_ns.class_("WitMotion", cg.Component, ble_client.BLEClientNode)

WitMotionUpdateRate = witmotion_ns.enum("RateArg")
UPDATE_RATES = {
    "0.2 Hz": WitMotionUpdateRate.RATE_0_2HZ,
    "0.5 Hz": WitMotionUpdateRate.RATE_0_5HZ,
    "1 Hz":   WitMotionUpdateRate.RATE_1HZ,
    "2 Hz":   WitMotionUpdateRate.RATE_2HZ,
    "5 Hz":   WitMotionUpdateRate.RATE_5HZ,
    "10 Hz":  WitMotionUpdateRate.RATE_10HZ,
    "20 Hz":  WitMotionUpdateRate.RATE_20HZ,
    "50 Hz":  WitMotionUpdateRate.RATE_50HZ,
    "100 Hz": WitMotionUpdateRate.RATE_100HZ,
    "200 Hz": WitMotionUpdateRate.RATE_200HZ,
}

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(WitMotion),
            cv.Optional(CONF_ACCELERATION): sensor.sensor_schema(
                unit_of_measurement=UNIT_G,
                accuracy_decimals=3,
                icon=ICON_ACCELERATION,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ACCELERATION_X): sensor.sensor_schema(
                unit_of_measurement=UNIT_G,
                accuracy_decimals=3,
                icon=ICON_ACCELERATION_X,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ACCELERATION_Y): sensor.sensor_schema(
                unit_of_measurement=UNIT_G,
                accuracy_decimals=3,
                icon=ICON_ACCELERATION_Y,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ACCELERATION_Z): sensor.sensor_schema(
                unit_of_measurement=UNIT_G,
                accuracy_decimals=3,
                icon=ICON_ACCELERATION_Z,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ANGULAR_VELOCITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_DEGREE_PER_SECOND,
                accuracy_decimals=3,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ANGULAR_VELOCITY_X): sensor.sensor_schema(
                unit_of_measurement=UNIT_DEGREE_PER_SECOND,
                accuracy_decimals=3,
                icon=ICON_GYROSCOPE_X,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ANGULAR_VELOCITY_Y): sensor.sensor_schema(
                unit_of_measurement=UNIT_DEGREE_PER_SECOND,
                accuracy_decimals=3,
                icon=ICON_GYROSCOPE_Y,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ANGULAR_VELOCITY_Z): sensor.sensor_schema(
                unit_of_measurement=UNIT_DEGREE_PER_SECOND,
                accuracy_decimals=3,
                icon=ICON_GYROSCOPE_Z,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ROLL_ANGLE): sensor.sensor_schema(
                unit_of_measurement=UNIT_DEGREES,
                accuracy_decimals=2,
                icon=ICON_GYROSCOPE_X,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_PITCH_ANGLE): sensor.sensor_schema(
                unit_of_measurement=UNIT_DEGREES,
                accuracy_decimals=2,
                icon=ICON_GYROSCOPE_Y,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_YAW_ANGLE): sensor.sensor_schema(
                unit_of_measurement=UNIT_DEGREES,
                accuracy_decimals=2,
                icon=ICON_GYROSCOPE_Z,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_MAGNETIC_FLUX_DENSITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROTESLA,
                accuracy_decimals=3,
                icon=ICON_MAGNET,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_MAGNETIC_FLUX_DENSITY_X): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROTESLA,
                accuracy_decimals=3,
                icon=ICON_MAGNET,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_MAGNETIC_FLUX_DENSITY_Y): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROTESLA,
                accuracy_decimals=3,
                icon=ICON_MAGNET,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_MAGNETIC_FLUX_DENSITY_Z): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROTESLA,
                accuracy_decimals=3,
                icon=ICON_MAGNET,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_QUATERNION_0): sensor.sensor_schema(
                accuracy_decimals=5,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_QUATERNION_1): sensor.sensor_schema(
                accuracy_decimals=5,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_QUATERNION_2): sensor.sensor_schema(
                accuracy_decimals=5,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_QUATERNION_3): sensor.sensor_schema(
                accuracy_decimals=5,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                icon=ICON_THERMOMETER,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_UPDATE_RATE, default="0.2 Hz"): cv.enum(UPDATE_RATES, upper=False),
        }
    )
    .extend(ble_client.BLE_CLIENT_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)

    for t in ["acceleration", "angular_velocity", "magnetic_flux_density"]:
        for d in ["", "_x", "_y", "_z"]:
            key = f"{t}{d}"
            if key in config:
                sens = await sensor.new_sensor(config[key])
                cg.add(getattr(var, f"set_{key}")(sens))

    for key in ["roll_angle", "pitch_angle", "yaw_angle"]:
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, f"set_{key}")(sens))

    for n in range(4):
        key = f"quaternion_{n}"
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, f"set_{key}")(sens))

    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature(sens))

    if CONF_UPDATE_RATE in config:
        cg.add(var.set_update_rate(config[CONF_UPDATE_RATE]))
