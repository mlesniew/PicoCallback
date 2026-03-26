#include <Arduino.h>
#include <PicoCallback.h>

// A PicoCallback object can be reassigned to point to a different function at
// any time, making it easy to change behaviour at runtime.

void greetMorning() { Serial.println("Good morning!"); }
void greetEvening() { Serial.println("Good evening!"); }
void greetNight() { Serial.println("Good night!"); }

PicoCallback<void> greet;

void setup() {
    Serial.begin(115200);

    // Initially unset — isSet() returns false
    Serial.print("Is set: ");
    Serial.println(greet.isSet() ? "yes" : "no");

    greet = greetMorning;
    greet();  // "Good morning!"

    greet = greetEvening;
    greet();  // "Good evening!"

    greet = greetNight;
    greet();  // "Good night!"

    // Clear the callback
    greet = nullptr;
    Serial.print("Is set after reset: ");
    Serial.println(greet.isSet() ? "yes" : "no");
}

void loop() {}
