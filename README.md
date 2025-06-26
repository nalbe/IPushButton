
# IPushButton Library


## Overview
`IPushButton` is a C++ library designed to handle push button events such as push, hold, release etc.
It provides a simple interface to manage button states and events with different customization.


## Features  
- Handles different button states: idle, push, hold, release, delay, and rapid.  
- Customizable debounce and repeat delays using the `debounceDelay()` and `repeatDelay()` methods.  
- Detects rapid clicks (double, triple, etc.).  
- Acceleration feature to increase the button repeat rate (e.g., fast scrolling).  
- Supports inverted button states.  
- Allows switching between different time measurement methods.  
- Easily integrates with existing projects.  


## Installation
To use the `IPushButton` library in your project, follow these steps:
1. Clone the repository: `git clone https://github.com/nalbe/IPushButton.git`
2. Include the header files in your project: `#include <Arduino.h>`; `#include "IPushButton.h"`
3. Create an instance of the `IPushButton` class: `IPushButton button(pin, mode);` (replace `pin` and `mode` with appropriate values)
4. Call the `update()` function in your loop: `button.update();`    
5. Handle button events by overriding virtual methods (`onPushFn`, `onHoldFn`, etc.) or using `getState()` to check the current state


## Usage
Here is an example of how to use the `IPushButton` class:
```cpp
#include "IPushButton.h"

// Define a custom push button class inheriting from IPushButton
class MyPushButton : public IPushButton
{
protected:
    void onPushFn() override    { /* Handle push event */ }
    void onHoldFn() override    { /* Handle hold event */ }
    void onReleaseFn() override { /* Handle release event */ }
    void onDelayFn() override   { /* Handle delay event */ }
    void onIdleFn() override    { /* Handle idle event */ }
};

// Create an instance with pin XX and mode YY.
MyPushButton button(D9, INPUT_PULLUP);

// Setup function
void setup()
{
    // Set debounce delay to 60 ms (default is 50 ms)
    button.debounceDelay(60);
    // Set repeat delay to 250 ms (default is 0 ms)
    button.repeatDelay(250);
}

// Update the button state in the loop.
void loop()
{
    // Update the button state
    button.update();

    // your code here
}
```

Another way is to handle events directly in the loop:
```cpp
#include "IPushButton.h"

IPushButton button(D9, INPUT_PULLUP);

void setup()
{
    Serial.begin(115200);
    button.debounceDelay(60);
    button.repeatDelay(250);
}

void loop()
{
    button.update();

    // Handle button events
    if (button.state() & IPushButton::eState::PUSH) {
        // Handle push event
        Serial.println("push");
    }
    else if (button.state() & IPushButton::eState::HOLD) {
        // Handle hold event
        Serial.println("hold");
    }
    else if (button.state() & IPushButton::eState::RELEASE) {
        // Handle release event
        Serial.println("release");
    }
    else if (button.state() & IPushButton::eState::DELAY) {
        // Handle delay event
        //Serial.println("..");
    }
    else if (button.state() & IPushButton::eState::IDLE) {
        // Handle idle event
        //Serial.println(".");
    }
    if ((button.state() & IPushButton::eState::RAPID) and (button.rapidCount() & 1)) {
        // Handle rapid event
        Serial.println("dbl_click");
    }

    // your code here
}
```

## Possible output:
```cpp
// Short push:
push
release
// Hold a while:
push
hold
hold
hold
release
// Double click:
push
release
push
dbl_click
release
// 4 rapid clicks interpreted as 2 double:
push
release
push
dbl_click
release
push
release
push
dbl_click
release
```


## **Library Documentation**


### **Dependencies**
This library depends on the Arduino framework. To use this library, ensure that `Arduino.h` is included in your project before including this library's header file.

#### **Notes**
- If you are using the Arduino IDE, `Arduino.h` is automatically included for you, so you don’t need to explicitly include it in your sketch.
- If you are using a non-Arduino environment, ensure that the Arduino framework is properly set up and `Arduino.h` is available.


### Time Measurement Method Switching

This feature allows users to choose how timestamps are updated when interacting with a button. There are two modes:

- **Global Timestamp:** The timestamp updates only when the button is pressed.
  The actual time between events may be shorter than the repeat delay, depending on the overall loop delay.

- **Local Timestamp:** The timestamp updates continuously on every cycle while the button is pressed.
  The actual time between events may be longer than the repeat delay, depending on the overall loop delay.

By switching between these methods, user can balance performance and accuracy based on their needs.


### API Reference

#### Constructors
- `IPushButton()`
- `IPushButton(pin_t pin, pin_t mode)`

#### Methods
- `bool isEnabled() const`
- `bool isDebounceDelay() const`
- `bool isRepeatDelay() const`
- `bool isInverted() const`
- `bool isLocalTime() const`
- `bool isAccelerated() const`
- `eState state() const`
- `pin_t id() const`
- `pin_t mode() const`
- `millis_t debounceDelay() const`
- `millis_t repeatDelay() const`
- `millis_t pushTime() const`
- `millis_t releaseTime() const`
- `size_type rapidCount() const`
- `size_type accelerationValue() const`
- `size_type accelerationThreshold() const`
- `void id(pin_t)`
- `void mode(pin_t)`
- `void debounceDelay(millis_t)`
- `void repeatDelay(millis_t)`
- `void useLocalTime(bool)`
- `void accelerationValue(size_type)`
- `void accelerationThreshold(size_type)`
- `void invert(bool)`
- `void accelerate(bool)`
- `void enable(bool)`
- `void disable()`
- `void update()`
- `void reset()`

#### Virtual Methods
- `virtual ~IPushButton()`
- `virtual void onPushFn()`
- `virtual void onHoldFn()`
- `virtual void onReleaseFn()`
- `virtual void onDelayFn()`
- `virtual void onIdleFn()`


### License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

