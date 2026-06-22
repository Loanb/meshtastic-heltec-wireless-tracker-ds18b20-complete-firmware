#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR && defined(HELTEC_TRACKER_V1_1) && __has_include(<DallasTemperature.h>) && \
    __has_include(<OneWire.h>)

#pragma once

#include "../mesh/generated/meshtastic/telemetry.pb.h"
#include "TelemetrySensor.h"
#include <DallasTemperature.h>
#include <OneWire.h>

class DS18B20Sensor : public TelemetrySensor
{
  private:
    static const uint8_t DS18B20_PIN = 5;
    OneWire oneWire;
    DallasTemperature ds18b20;

  public:
    DS18B20Sensor();
    virtual bool getMetrics(meshtastic_Telemetry *measurement) override;
    virtual bool initDevice(TwoWire *bus, ScanI2C::FoundDevice *dev) override;
};

#endif
