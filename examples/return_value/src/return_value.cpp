#include <Arduino.h>
#include <PicoCallback.h>

// PicoCallback can wrap functions that return a value.  The return type is the
// first template parameter.

int readTemperature() {
    // Simulated sensor reading
    return 42;
}

int readHumidity() {
    // Simulated sensor reading
    return 75;
}

// A PicoCallback that returns an int
PicoCallback<int> getSensorValue;

void setup() {
    Serial.begin(115200);

    getSensorValue = readTemperature;
    int temp = getSensorValue();
    Serial.print("Temperature: ");
    Serial.println(temp);

    getSensorValue = readHumidity;
    int hum = getSensorValue();
    Serial.print("Humidity: ");
    Serial.println(hum);
}

void loop() {}
