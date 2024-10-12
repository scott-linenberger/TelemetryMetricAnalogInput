#include "TelemetryMetricAnalogInput.h"

void TelemetryMetricAnalogInput::begin(MqttClient *mqttClient) {
  TelemetryMetricReporter::begin(mqttClient);
}

String TelemetryMetricAnalogInput::_getTopicEvents() {
  return _topicMqttBase + "/events";
}

float TelemetryMetricAnalogInput::getMetricValue() {
  return analogRead(_pin);
}

void TelemetryMetricAnalogInput::publishMetric() {
  String stringTopic = _topicMqttBase + "/" + _metricName;
  int topicLength = stringTopic.length() + 1;
  char sensorTopic[topicLength];

  stringTopic.toCharArray(sensorTopic, topicLength);

  _mqttClient->beginMessage(
    sensorTopic,
    _retainEvents,
    _qosEvents);

  if (_runType == METRIC_ON_INTERVAL_WITH_SMOOTHING) {
    _mqttClient->print(getSmoothedMetricValue());
  } else {
    _mqttClient->print(getMetricValue());
  }

  _mqttClient->endMessage();
  _mqttClient->flush();
}

void TelemetryMetricAnalogInput::publishConfiguration() {
  JsonDocument json = _getBaseConfig();

  /* add custom json */
  _publishJson(
    json,
    _topicMqttBase + "/config",
    _retainEvents,
    _qosEvents
    );
}
