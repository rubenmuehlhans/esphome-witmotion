#ifdef USE_ESP32

#include "witmotion_struct.h"

namespace esphome {
namespace witmotion {

static int16_t combine_low_high(uint8_t low, uint8_t high) {
  uint16_t data = ((high << 8) | low);
  return static_cast<int16_t>(data);
}

void DefaultData::decode_acceleration(float *ax, float *ay, float *az) const {
  // Output in G
  *ax = combine_low_high(axL, axH) * 16.0 / 32768.0;
  *ay = combine_low_high(ayL, ayH) * 16.0 / 32768.0;
  *az = combine_low_high(azL, azH) * 16.0 / 32768.0;
}

void DefaultData::decode_angular_velocity(float *wx, float *wy, float *wz) const {
  // Output in degrees per second
  *wx = combine_low_high(wxL, wxH) * 2000.0 / 32768.0;
  *wy = combine_low_high(wyL, wyH) * 2000.0 / 32768.0;
  *wz = combine_low_high(wzL, wzH) * 2000.0 / 32768.0;
}

void DefaultData::decode_angle(float *roll, float *pitch, float *yaw) const {
  // Output in degrees
  *roll =  combine_low_high(RollL, RollH)   * 2000.0 / 32768.0;
  *pitch = combine_low_high(PitchL, PitchH) * 2000.0 / 32768.0;
  *yaw =   combine_low_high(YawL, YawH)     * 2000.0 / 32768.0;
}

void MagneticFieldOutput::decode(float *hx, float *hy, float *hz) const {
  // Output in microTeslas. Datasheet equation is for milliGauss, 1 mG = 0.1 uT
  *hx = combine_low_high(HxL, HxH) * 0.1;
  *hy = combine_low_high(HyL, HyH) * 0.1;
  *hz = combine_low_high(HzL, HzH) * 0.1;
}

void QuaternionOutput::decode(float *q0, float *q1, float *q2, float *q3) const {
  *q0 = combine_low_high(Q0L, Q0H) / 32768.0;
  *q1 = combine_low_high(Q1L, Q1H) / 32768.0;
  *q2 = combine_low_high(Q2L, Q2H) / 32768.0;
  *q3 = combine_low_high(Q3L, Q3H) / 32768.0;
}

void TemperatureOutput::decode(float *temperature) const {
  // Output in degrees Celsius
  *temperature = combine_low_high(TL, TH) / 100.0;
}

void BatteryVoltageOutput::decode(float *voltage) const {
  // Output in Volts
  *voltage = combine_low_high(VL, VH) / 100.0;
}

void VersionOutput::decode(uint16_t *version) const {
  *version = combine_low_high(VersionL, VersionH);
}

void ReadRegisterCommand::compose(RegisterNumber reg) {
  h1 = 0xff;
  h2 = 0xaa;
  cmd = READ_REGISTER;
  regL = reg;
  regH = 0x00;
}

void SetRateCommand::compose(RateArg rate) {
  h1 = 0xff;
  h2 = 0xaa;
  cmd = SET_RATE;
  rateL = rate;
  rateH = 0x00;
}

void SetYearMonthCommand::compose(ESPTime const& date_time) {
  h1 = 0xff;
  h2 = 0xaa;
  cmd = SET_YYMM;
  year = date_time.year - 1900;
  month = date_time.month;
}

void SetDayHourCommand::compose(ESPTime const& date_time) {
  h1 = 0xff;
  h2 = 0xaa;
  cmd = SET_DDHH;
  day = date_time.day_of_month;
  hour = date_time.hour;
}

void SetMinuteSecondsCommand::compose(ESPTime const& date_time) {
  h1 = 0xff;
  h2 = 0xaa;
  cmd = SET_MMSS;
  minute = date_time.minute;
  seconds = date_time.second;
}

void SetMillisecondsCommand::compose(ESPTime const& date_time) {
  h1 = 0xff;
  h2 = 0xaa;
  cmd = SET_MS;
  milliL = 0;
  milliH = 0;
}

float BatteryVoltageToLevel(float voltage)
{
  // Conversion table provided by support@wit-motion.com
  static struct entry { float voltage; float level; } const table[] =
    { { 3.40,   5.0 },
      { 3.50,  10.0 },
      { 3.68,  15.0 },
      { 3.70,  20.0 },
      { 3.73,  30.0 },
      { 3.77,  40.0 },
      { 3.79,  50.0 },
      { 3.82,  60.0 },
      { 3.87,  75.0 },
      { 3.93,  90.0 },
      { 3.97, 100.0 } }; // > 3.96 => >= 3.97

  float level = 0.0;

  for (int i = 0; i < sizeof(table)/sizeof(entry); i++) {
    if (voltage >= table[i].voltage ) {
      level = table[i].level;
    } else {
      break;
    }
  }

  return level;
}

float RateEnumToHertz(RateArg rate) {
  float hertz = 0.0;

  switch (rate) {
    case RATE_0_2HZ:
      hertz = 0.2;
      break;

    case RATE_0_5HZ:
      hertz = 0.5;
      break;

    case RATE_1HZ:
      hertz = 1.0;
      break;

    case RATE_2HZ:
      hertz = 2.0;
      break;

    case RATE_5HZ:
      hertz = 5.0;
      break;

    case RATE_10HZ:
      hertz = 10.0;
      break;

    case RATE_20HZ:
      hertz = 20.0;
      break;

    case RATE_50HZ:
      hertz = 50.0;
      break;

    case RATE_100HZ:
      hertz = 100.0;
      break;

    case RATE_200HZ:
      hertz = 200.0;
      break;
  }

  return hertz;
}

}  // namespace witmotion
}  // namespace esphome

#endif // USE_ESP32
