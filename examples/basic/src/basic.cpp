#include <Arduino.h>
#include <PicoCallback.h>

PicoCallback<void> callback;

void setup() {
    Serial.begin(115200);

    callback = [] { Serial.println("Hello from PicoCallback"); };
}

void loop() {
    callback();
    delay(1000);
}
