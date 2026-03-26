#include <Arduino.h>
#include <PicoCallback.h>

// PicoCallback can invoke methods on objects by using a free function that
// takes a pointer to the object as its context.

class Led {
public:
    Led(int pin, const char * label) : pin(pin), label(label), state(false) {}

    void begin() { pinMode(pin, OUTPUT); }

    void turnOn() {
        state = true;
        digitalWrite(pin, HIGH);
        Serial.print(label);
        Serial.println(" ON");
    }

    void turnOff() {
        state = false;
        digitalWrite(pin, LOW);
        Serial.print(label);
        Serial.println(" OFF");
    }

    bool isOn() const { return state; }

private:
    int pin;
    const char * label;
    bool state;
};

Led led1(2, "LED1");
Led led2(4, "LED2");

// Free functions acting as method trampolines
void turnOnLed(Led * led) { led->turnOn(); }
void turnOffLed(Led * led) { led->turnOff(); }

void setup() {
    Serial.begin(115200);
    led1.begin();
    led2.begin();

    PicoCallback<void> onCb1(turnOnLed, &led1);
    PicoCallback<void> offCb1(turnOffLed, &led1);
    PicoCallback<void> onCb2(turnOnLed, &led2);

    onCb1();   // LED1 ON
    onCb2();   // LED2 ON
    offCb1();  // LED1 OFF
}

void loop() {}
