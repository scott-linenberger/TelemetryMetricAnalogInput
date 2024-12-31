/**
 * COMPATIBLE WITH ESP8266 & ESP32
 * ....................................
 * Tested Device List:
 * - Adafruit Feather Huzzah ESP8266
 * - Xiao ESP32C3
 * - Xiao ESP32C6
 * - Sparkfun Thing Plus ESP32-C6
 * ....................................
 */
#include <Arduino.h>
#include <TelemetryNode.h>
#include <TelemetryMetricAnalogInput.h>
/* Configure the telemetry node */
#include "TELEM_CONFIG.h"

/* Connections */
WiFiClient wiFiClient;
MqttClient mqttClient(wiFiClient);

/* Telemetry Node */
TelemetryNode telemNode = TelemetryNode(
  wiFiClient,
  mqttClient,
  TELEM_CONFIG
);

/**
 * Example where the analog sensor is recording soil moisture
 */
TelemetryMetricAnalogInput metricAnalogSensorReading = TelemetryMetricAnalogInput(
  A1, // sensor pin
  "moisture", // MQTT metric name
  "my-mqtt-server-root/soil-monitor/moisture/0", // MQTT topic for analog metric publishes
  false, // Send MQTT messages as retained?
  0 // MQTT message with QOS
  );

void setup() {
    /* begin the telemetry node */
    telemNode.begin();

    /* connect TelemetryNode to network & MQTT */
    telemNode.connect();

    // do MQTT pub/sub
    mqttClient.onMessage(onMqttOnMessage);
    /* subscribe to telemetry node actions to respond to action requests */
    mqttClient.subscribe(TELEM_CONFIG.topic.incoming_actions, 1);

    /* configure metric run type */
    metricAnalogSensorReading.setRunTypeInterval(60000);  // publish every 60 seconds
    // metricBatteryVoltage.setRunTypeOnChange(100, 30.0); // poll and publish on threshold change

      /* start metric(s) */
  metricAnalogSensorReading.begin(telemNode.getMqttClient());

}

void onMqttOnMessage(int messageSize) {
  /* process incoming messages so telemetry node can respond to action requests */
  JsonDocument json = telemNode.processIncomingMessage(messageSize);
  /* respond to incoming action requests */
  metricAnalogSensorReading.onMessage(json);
}

void loop() {
    telemNode.run();
    yield();
    metricAnalogSensorReading.run();
    yield();
}