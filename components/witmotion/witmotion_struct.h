#pragma once
#ifdef USE_ESP32

#include <cstdint>

namespace esphome {
namespace witmotion {

//
// Based on WT901BLECL datasheet v23-0420
//

enum {
  RAW_DATA_BUFFER_SIZE       = 20,
  RAW_COMMAND_BUFFER_SIZE    = 5
};

enum RegisterNumber : uint8_t {
  SAVE		= 0x00,
  CALSW		= 0x01,
  RSV	   	= 0x02,
  RATE		= 0x03,
  BAUD		= 0x04,
  AXOFFSET	= 0x05,
  AYOFFSET	= 0x06,
  AZOFFSET	= 0x07,
  GXOFFSET	= 0x08,
  GYOFFSET	= 0x09,
  GZOFFSET	= 0x0a,
  HXOFFSET	= 0x0b,
  HYOFFSET	= 0x0c,
  HZOFFSET	= 0x0d,
  D0MODE	= 0x0e,
  D1MODE	= 0x0f,
  D2MODE	= 0x10,
  D3MODE	= 0x11,
  VERSION	= 0x2e,
  YYMM		= 0x30,
  DDHH		= 0x31,
  MMSS		= 0x32,
  MS	   	= 0x33,
  AX	   	= 0x34,
  AY	   	= 0x35,
  AZ	   	= 0x36,
  GX	   	= 0x37,
  GY	   	= 0x38,
  GZ	   	= 0x39,
  HX	   	= 0x3a,
  HY	   	= 0x3b,
  HZ	   	= 0x3c,
  ROLL		= 0x3d,
  PITCH		= 0x3e,
  YAW	   	= 0x3f,
  TEMP		= 0x40,
  D0STATUS	= 0x41,
  D1STATUS	= 0x42,
  D2STATUS	= 0x43,
  D3STATUS	= 0x44,
  Q0	    = 0x51,
  Q1	    = 0x52,
  Q2	    = 0x53,
  Q3	    = 0x54,
  BATTERY   = 0x64, // Battery voltage in 1/100 V, per support@wit-motion.com
};

//
// RECEIVED DATA
//

enum HeadingOctet : uint8_t {
  HEADING = 0x55
};

enum FlagOctet : uint8_t {
  DEFAULT_DATA          = 0x61,
  SINGLE_RETURN_DATA    = 0x71
};

struct RawData {
  uint8_t data[RAW_DATA_BUFFER_SIZE];
};

struct PacketHeader {
  HeadingOctet    heading;
  FlagOctet       flag;
};

struct DefaultData {
  HeadingOctet    heading;    // 0x55
  FlagOctet       flag;       // 0x61
  uint8_t         axL;
  uint8_t         axH;
  uint8_t         ayL;
  uint8_t         ayH;
  uint8_t         azL;
  uint8_t         azH;
  uint8_t         wxL;
  uint8_t         wxH;
  uint8_t         wyL;
  uint8_t         wyH;
  uint8_t         wzL;
  uint8_t         wzH;
  uint8_t         RollL;
  uint8_t         RollH;
  uint8_t         PitchL;
  uint8_t         PitchH;
  uint8_t         YawL;
  uint8_t         YawH;

  void decode_acceleration(float *ax, float *ay, float *az) const;
  void decode_angular_velocity(float *wx, float *wy, float *wz) const;
  void decode_angle(float *roll, float *pitch, float *yaw) const;
};

struct SingleReturnDataPacket {
  HeadingOctet    heading;    // 0x55
  FlagOctet       flag;       // 0x71
  RegisterNumber  RegL;
  uint8_t         RegH;       // 0x00
  struct {
      uint8_t low;
      uint8_t high;
  }               reg[8];
};

struct MagneticFieldOutput {
  HeadingOctet    heading;    // 0x55
  FlagOctet       flag;       // 0x71
  RegisterNumber  RegL;       // 0x3a
  uint8_t         RegH;       // 0x00
  uint8_t         HxL;
  uint8_t         HxH;
  uint8_t         HyL;
  uint8_t         HyH;
  uint8_t         HzL;
  uint8_t         HzH;

  void decode(float *hx, float *hy, float *hz) const;
};

struct QuaternionOutput {
  HeadingOctet    heading;    // 0x55
  FlagOctet       flag;       // 0x71
  RegisterNumber  RegL;       // 0x51
  uint8_t         RegH;       // 0x00
  uint8_t         Q0L;
  uint8_t         Q0H;
  uint8_t         Q1L;
  uint8_t         Q1H;
  uint8_t         Q2L;
  uint8_t         Q2H;
  uint8_t         Q3L;
  uint8_t         Q3H;

  void decode(float *q0, float *q1, float *q2, float *q3) const;
};

struct TemperatureOutput {
  HeadingOctet    heading;    // 0x55
  FlagOctet       flag;       // 0x71
  RegisterNumber  RegL;       // 0x40
  uint8_t         RegH;       // 0x00
  uint8_t         TL;
  uint8_t         TH;

  void decode(float *temperature) const;
};

struct BatteryVoltageOutput {
  HeadingOctet    heading;    // 0x55
  FlagOctet       flag;       // 0x71
  RegisterNumber  RegL;       // 0x64
  uint8_t         RegH;       // 0x00
  uint8_t         VL;
  uint8_t         VH;

  void decode(float *voltage) const;
};

struct VersionOutput {
  HeadingOctet    heading;    // 0x55
  FlagOctet       flag;       // 0x71
  RegisterNumber  RegL;       // 0x2e
  uint8_t         RegH;       // 0x00
  uint8_t         VersionL;
  uint8_t         VersionH;

  void decode(uint16_t *version) const;
};

union WitMotionData {
  RawData                 raw;
  PacketHeader            header;
  DefaultData             default_data;
  SingleReturnDataPacket  single_return;
  MagneticFieldOutput     magnetic_field;
  QuaternionOutput        quaternion;
  TemperatureOutput       temperature;
  BatteryVoltageOutput    battery;
  VersionOutput           version;
};

//
// TRANSMITTED COMMANDS
//

enum CommandCode : uint8_t {
  SAVE_SETTINGS   = 0x00,
  CALIBRATION     = 0x01,
  SET_RATE        = 0x03,
  READ_REGISTER   = 0x27
};

enum SaveArg : uint8_t {
  SAVE_CURRENT    = 0x00,
  RESET_DEFAULT   = 0x01
};

enum CalibrationArg : uint8_t {
  QUIT            = 0x00,
  ACCELEROMETER   = 0x01,
  MAGNETIC        = 0x07
};

enum RateArg : uint8_t {
  RATE_0_2HZ  = 0X01,
  RATE_0_5HZ  = 0X02,
  RATE_1HZ    = 0X03,
  RATE_2HZ    = 0X04,
  RATE_5HZ    = 0X05,
  RATE_10HZ   = 0X06,
  RATE_20HZ   = 0X07,
  RATE_50HZ   = 0X08,
  RATE_100HZ  = 0X09,
  RATE_200HZ  = 0X0A
};

struct RawCommand {
  uint8_t         cmd[RAW_COMMAND_BUFFER_SIZE];

  uint8_t* ptr() { return reinterpret_cast<uint8_t*>(cmd); }
};

struct GenericCommand {
  uint8_t         h1;         // 0xff
  uint8_t         h2;         // 0xaa
  CommandCode     cmd;
  uint8_t         argL;
  uint8_t         argH;
};

struct SaveSettingsCommand {
  uint8_t         h1;         // 0xff
  uint8_t         h2;         // 0xaa
  CommandCode     cmd;        // 0x00
  SaveArg         setL;
  uint8_t         setH;       // 0x00
};

struct CalibrationCommand {
  uint8_t         h1;         // 0xff
  uint8_t         h2;         // 0xaa
  CommandCode     cmd;        // 0x01
  CalibrationArg  calL;
  uint8_t         calH;       // 0x00
};

struct SetRateCommand {
  uint8_t         h1;         // 0xff
  uint8_t         h2;         // 0xaa
  CommandCode     cmd;        // 0x03
  RateArg         rateL;
  uint8_t         rateH;      // 0x00

  void compose(RateArg rate);
};

struct ReadRegisterCommand {
  uint8_t         h1;         // 0xff
  uint8_t         h2;         // 0xaa
  CommandCode     cmd;        // 0x27
  RegisterNumber  regL;
  uint8_t         regH;       // 0x00

  void compose(RegisterNumber reg);
};

union WitMotionCommand {
  RawCommand          raw;
  GenericCommand      generic;
  SaveSettingsCommand cwsave_settingsss;
  CalibrationCommand  calibration;
  SetRateCommand      set_rate;
  ReadRegisterCommand read_register;
};

//
// Utility functions
//

float BatteryVoltageToLevel(float voltage);
float RateEnumToHertz(RateArg rate);

}  // namespace witmotion
}  // namespace esphome

#endif // USE_ESP32
