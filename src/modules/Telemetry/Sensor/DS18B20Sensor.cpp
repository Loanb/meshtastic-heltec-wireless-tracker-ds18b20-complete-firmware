#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR && defined(HELTEC_TRACKER_V1_1) && __has_include(<DallasTemperature.h>) && \
    __has_include(<OneWire.h>)

#include "../mesh/generated/meshtastic/telemetry.pb.h"
#include "DS18B20Sensor.h"
#include "TelemetrySensor.h"

DS18B20Sensor::DS18B20Sensor()
    : TelemetrySensor(meshtastic_TelemetrySensorType_SENSOR_UNSET, "DS18B20"), oneWire(DS18B20_PIN), ds18b20(&oneWire)
{
}

bool DS18B20Sensor::initDevice(TwoWire *bus, ScanI2C::FoundDevice *dev)
{
    LOG_INFO("Init sensor: %s on GPIO %d", sensorName, DS18B20_PIN);

    ds18b20.begin();
    ds18b20.setWaitForConversion(true);

    if (ds18b20.getDeviceCount() < 1) {
        LOG_WARN("DS18B20 not detected on GPIO %d", DS18B20_PIN);
        status = 0;
        initialized = true;
        return false;
    }

    status = 1;
    initialized = true;
    ds18b20.requestTemperatures();
    LOG_INFO("DS18B20 initialized on GPIO %d", DS18B20_PIN);
    return true;
}

bool DS18B20Sensor::getMetrics(meshtastic_Telemetry *measurement)
{
    measurement->which_variant = meshtastic_Telemetry_environment_metrics_tag;

    ds18b20.requestTemperatures();
    float tempC = ds18b20.getTempCByIndex(0);

    if (tempC == DEVICE_DISCONNECTED_C) {
        LOG_WARN("DS18B20 invalid temperature read on GPIO %d", DS18B20_PIN);
        return false;
    }

    measurement->variant.environment_metrics.has_temperature = true;
    measurement->variant.environment_metrics.temperature = tempC;
    LOG_INFO("DS18B20 temperature: %.2f C", tempC);
    return true;
}

#endif
