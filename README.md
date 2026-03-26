# PicoCallback

A tiny, heap-free callback library for Arduino, ESP8266 and ESP32.

PicoCallback gives you a clean, reusable type for storing and calling a
callback function.  It works like a lightweight version of `std::function`.
You can pass it around, store it in a struct or class, and call it later.  
All without any heap allocation!

```cpp
PicoCallback<void> onButtonPress;

// Somewhere during setup…
onButtonPress = [] { Serial.println("Button pressed!"); };

// Somewhere in the event loop…
if (buttonPressed) {
    onButtonPress();
}
```

The template parameter is the return type.  Parameters come after it:

```cpp
PicoCallback<void>           // void callback, no parameters
PicoCallback<int>            // returns int, no parameters
PicoCallback<void, int>      // void callback, one int parameter
PicoCallback<float, int, int> // returns float, takes two ints
```


## Why not just use `std::function`?

`std::function` is great, but it has a cost that matters on microcontrollers:

- It **allocates on the heap** when the stored callable is large enough.
- Typical instances can take 12–16 bytes of memory or more.
- It pulls in more code, increasing binary size.

PicoCallback helps with all of that:

- **Zero heap allocations**.
- Each instance is exactly **8 bytes**: one function pointer + one context pointer.
- Tiny code footprint.

The trade-off is that PicoCallback only supports **non-capturing** lambdas and
plain functions (see [Limitations](#limitations)).  For most embedded use cases
that's all you need.


## Quickstart

### Free functions

```cpp
void sayHello() {
    Serial.println("Hello!");
}

PicoCallback<void> cb = sayHello;
cb(); // prints "Hello!"
```

### Non-capturing lambdas

```cpp
PicoCallback<void> cb = [] { Serial.println("Hello from a lambda!"); };
cb();
```

### Callbacks with parameters

```cpp
PicoCallback<void, int, int> add([](int a, int b) {
    Serial.println(a + b);
});

add(3, 4); // prints 7
```

### Callbacks that return a value

```cpp
PicoCallback<int, int> square([](int x) { return x * x; });

int result = square(5); // result = 25
```

### Callbacks with a context pointer

If you need to refer to some state inside the callback, pass a pointer to it
as the context.  The callback receives it as its first argument:

```cpp
int threshold = 10;

PicoCallback<bool, int> isAbove(
    [](const int* t, int value) { return value > *t; },
    &threshold
);

isAbove(15); // true
isAbove(5);  // false
```

The context can be a pointer to any type — a plain variable, a struct, or an
object.

### Calling object methods

Member functions aren't directly assignable, but a small free function (or
lambda) acting as a trampoline does the job nicely:

```cpp
class Buzzer {
public:
    void beep() { /* … */ }
};

Buzzer buzzer;

PicoCallback<void> cb([](Buzzer* b) { b->beep(); }, &buzzer);
cb(); // calls buzzer.beep()
```

### Checking and clearing a callback

```cpp
PicoCallback<void> cb;

cb.isSet(); // false

cb = [] { Serial.println("Hi!"); };
cb.isSet(); // true

cb = nullptr; // or cb.reset()
cb.isSet(); // false
```

### Reassigning at runtime

PicoCallback objects can be reassigned freely.  This makes it easy to change
behaviour at runtime:

```cpp
PicoCallback<void> action = [] { Serial.println("Mode A"); };
action(); // "Mode A"

action = [] { Serial.println("Mode B"); };
action(); // "Mode B"
```


## Limitations

PicoCallback only supports **non-capturing lambdas** and free functions.  A
capturing lambda (one that captures a local variable using `[x]` or `[&]`) 
cannot be converted to a plain function pointer and will cause a compile
error:

```cpp
int value = 42;

// This will NOT compile — capturing lambda
PicoCallback<void> cb([value] { Serial.println(value); });

// Use a context pointer instead
PicoCallback<void> cb([](const int* v) { Serial.println(*v); }, &value);
```


## `PICOCALLBACK_USE_STD_FUNCTION`

If you define `PICOCALLBACK_USE_STD_FUNCTION` before including the header (or
via a build flag), PicoCallback switches its implementation to use
`std::function` under the hood.  The public API stays exactly the same -- your
code doesn't need to change at all.

```ini
; platformio.ini
build_flags = -DPICOCALLBACK_USE_STD_FUNCTION
```

In this mode, capturing lambdas work too:

```cpp
int value = 42;

// Works when PICOCALLBACK_USE_STD_FUNCTION is defined
PicoCallback<void> cb([value] { Serial.println(value); });
```

The downside is that you get all the usual `std::function` costs back: heap
allocations and higher memory usage.

This define is mainly aimed at **library developers** who want to accept
callbacks from their users.  By basing your API on `PicoCallback`, your library
is heap-free by default -- great for resource-constrained boards.  But if a
user's project is fine with `std::function`'s overhead, they can flip the 
define and gain full capturing-lambda support without changing any of their 
callback code.  Best of both worlds!