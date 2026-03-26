#include <Arduino.h>
#include <PicoCallback.h>

// PicoCallback can be constructed and assigned from non-capturing lambdas.
// The lambda is converted to a plain function pointer, so no heap allocation
// occurs.  Capturing lambdas require the PICOCALLBACK_USE_STD_FUNCTION mode.

struct Range {
    int lo;
    int hi;
};

void setup() {
    Serial.begin(115200);

    // Construct directly from a non-capturing lambda
    PicoCallback<void> greet([] { Serial.println("Hello from a lambda!"); });
    greet();

    // Assign a different lambda to an existing callback
    PicoCallback<void> action;
    action = [] { Serial.println("First action"); };
    action();

    action = [] { Serial.println("Second action"); };
    action();

    // Lambda with parameters
    PicoCallback<void, int, int> printRange(
        [](int lo, int hi) {
            Serial.print("Range: ");
            Serial.print(lo);
            Serial.print("..");
            Serial.println(hi);
        });
    printRange(1, 10);

    // Lambda that returns a value
    PicoCallback<int, int> square([](int x) { return x * x; });
    Serial.print("7 squared = ");
    Serial.println(square(7));

    // Lambda with a context pointer — the lambda receives the context as its
    // first argument, just like a free function would
    Range window = {5, 20};
    PicoCallback<bool, int> inRange(
        [](const Range * r, int value) { return value >= r->lo && value <= r->hi; },
        &window);

    Serial.print("12 in [5, 20]: ");
    Serial.println(inRange(12) ? "yes" : "no");

    Serial.print("42 in [5, 20]: ");
    Serial.println(inRange(42) ? "yes" : "no");
}

void loop() {}
