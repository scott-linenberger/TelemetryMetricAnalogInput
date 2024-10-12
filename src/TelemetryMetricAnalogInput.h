#ifndef LINENTOOLS_TELEM_METRIC_ANALOG_INPUT_H
#define LINENTOOLS_TELEM_METRIC_ANALOG_INPUT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoMqttClient.h>
#include <TelemetryMetricReporter.h>

class TelemetryMetricAnalogInput : public TelemetryMetricReporter {
private:
  uint8_t _pin;
  String _metricName;

protected:
  String _getTopicEvents() override;

public:
  TelemetryMetricAnalogInput(
    uint8_t pin,
    String metricName,
    String topicMqttBase,
    bool retainEventMessages,
    uint8_t qosEventMessages)
    : TelemetryMetricReporter(
      metricName,  // call super constructor
      topicMqttBase,
      retainEventMessages,
      qosEventMessages),
      _pin(pin), _metricName(metricName){};
  void begin(MqttClient* mqttClient) override;
  void publishMetric() override;
  void publishConfiguration() override;
  float getMetricValue() override;
};


#endif