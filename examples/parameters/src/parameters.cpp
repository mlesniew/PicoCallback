#include <Arduino.h>
#include <PicoCallback.h>

// PicoCallback supports callbacks with parameters.  The parameter types follow
// the return type in the template argument list:
//   PicoCallback<ReturnType, Param1Type, Param2Type, ...>

void printSum(int a, int b) {
    Serial.print(a);
    Serial.print(" + ");
    Serial.print(b);
    Serial.print(" = ");
    Serial.println(a + b);
}

void printProduct(int a, int b) {
    Serial.print(a);
    Serial.print(" * ");
    Serial.print(b);
    Serial.print(" = ");
    Serial.println(a * b);
}

// Callback with a context — accumulates a running total
void accumulate(int * total, int value) {
    *total += value;
    Serial.print("Running total: ");
    Serial.println(*total);
}

void setup() {
    Serial.begin(115200);

    // Two-parameter callback returning void
    PicoCallback<void, int, int> mathOp;

    mathOp = printSum;
    mathOp(3, 4);  // 3 + 4 = 7

    mathOp = printProduct;
    mathOp(3, 4);  // 3 * 4 = 12

    // Callback with a context and one parameter
    int total = 0;
    PicoCallback<void, int> addToTotal(accumulate, &total);

    addToTotal(10);  // Running total: 10
    addToTotal(5);   // Running total: 15
    addToTotal(20);  // Running total: 35
}

void loop() {}
