#include <Arduino.h>
#include <PicoCallback.h>

// PicoCallback supports passing a context pointer alongside the function.
// The context can be a pointer to any type — a struct, a plain variable, etc.
// The callback function receives it as its first argument.

struct SensorConfig {
    const char * name;
    int baseline;
};

void printReading(const SensorConfig * sensor) {
    Serial.print(sensor->name);
    Serial.print(" reading: ");
    Serial.println(sensor->baseline + random(10));
}

void printCounter(const int * count) {
    Serial.print("Count: ");
    Serial.println(*count);
}

SensorConfig tempSensor = {"Temperature", 20};
SensorConfig pressureSensor = {"Pressure", 1013};
int counter = 7;

void setup() {
    Serial.begin(115200);

    // Bind the same function to two different contexts
    PicoCallback<void> cbTemp(printReading, &tempSensor);
    PicoCallback<void> cbPressure(printReading, &pressureSensor);
    PicoCallback<void> cbCounter(printCounter, &counter);

    cbTemp();
    cbPressure();
    cbCounter();
}

void loop() {}
