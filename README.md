
# IPushButton Library


## Overview
`IPushButton` is a C++ library designed to handle push button events such as push, hold, release, delay, and rapid clicks.
It provides a simple interface to manage button states and events with customizable debounce and repeat delays.


## Features
- Handle different button states: idle, push, hold, release, delay, and mash.
- Customizable debounce and repeat delays.
- Support for inversed button states.
- Easy integration with existing projects.


## Installation
To use the `IPushButton` library in your project, follow these steps:
1. Clone the repository: `git clone https://github.com/nalbe/IPushButton.git`
2. Include the header file in your project: `#include "IPushButton.h"`
3. Create an instance of the `IPushButton` class: `IPushButton button(pin, mode);` (replace `pin` and `mode` with appropriate values)
4. Call the `update()` function in your loop: `button.update();`    
5. Handle button events by overriding virtual methods (`onPushFn`, `onHoldFn`, etc.) or using `getState()` to check the current state.
6. Customize debounce and repeat delays using `debounceDelay()` and `repeatDelay()` methods.
7. Detect rapid clicks via `rapidcount()` and state checks.
8. Use acceleration feature to increase the repeat rate of the button.


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


## Time anchor
By default, time intervals between events calculated using global timestamp.
Call `useLocalTime(true)` to switch to local timestamp.


## `update()` function pseudocode:
```cpp
+-----------------------------------+
| Start update()                    |
+-----------------------------------+
            |
            v
+-----------------------------------+
| Is the button enabled?            |
| - No: Exit function               |
| - Yes: Continue                   |
+-----------------------------------+
            |
            v
+-----------------------------------+
| Is within debounce delay period?  |
| - Yes:                            |
|   • If pushed: Set state = DELAY, |
|     call onDelayFn()              |
|   • Else: Set state = IDLE,       |
|     call onIdleFn()               |
| - No: Continue                    |
+-----------------------------------+
            |
            v
+-----------------------------------+
| Is the button not pushed?         |
| - Yes: Set state = IDLE,          |
|   call onIdleFn()                 |
| - No: Continue                    |
+-----------------------------------+
            |
            v
+-----------------------------------+
| Is the button just pushed?        |
| - Yes:                            |
|   • Detect rapid (quick) push:    |
|     - Increment rapidCount        |
|     - Set state = PUSH_WITH_MASH  |
|   • Else: Reset rapidCount        |
|   • Set isPushed = true           |
|   • Increment cycleCount          |
|   • Update pushTimestamp          |
|   • Set state = PUSH              |
|   • Call onPushFn()               |
| - No: Continue                    |
+-----------------------------------+
            |
            v
+-----------------------------------+
| Was the button pushed and         |
| released?                         |
| - Yes:                            |
|   • Set isPushed = false          |
|   • Reset cycleCount              |
|   • Update releaseTimestamp       |
|   • Set state = RELEASE           |
|   • Call onReleaseFn()            |
| - No: Continue                    |
+-----------------------------------+
            |
            v
+-----------------------------------+
| Is the button held past repeat    |
| delay?                            |
| - Yes:                            |
|   • Increment cycleCount          |
|   • Set state = HOLD              |
|   • Call onHoldFn()               |
| - No: Continue                    |
+-----------------------------------+
            |
            v
+-----------------------------------+
| Default:                          |
| • Set state = DELAY               |
| • Call onDelayFn()                |
+-----------------------------------+
            |
            v
+-----------------------------------+
| End update()                      |
+-----------------------------------+
```


## API Reference
### Constructors
- `IPushButton()`
- `IPushButton(size_type pin, size_type mode)`

### Methods
- `bool isEnabled() const`
- `bool isDebounceDelay() const`
- `bool isRepeatDelay() const`
- `bool isInverted() const`
- `bool isLocalTime() const`
- `size_type id() const`
- `void id(size_type)`
- `size_type mode() const`
- `void mode(size_type)`
- `time_type debounceDelay() const`
- `void debounceDelay(time_type)`
- `time_type repeatDelay() const`
- `void repeatDelay(time_type)`
- `time_type pushTime() const`
- `time_type releaseTime() const`
- `cnt_type rapidCount() const`
- `void useLocalTime(bool)`
- `void inverse()`
- `void enable()`
- `void disable()`
- `eState state() const`
- `void update()`
- `void reset()`
- `void useAcceleration(bool)`
- `bool isAccelerated() const`
- `time_type accelerationValue() const`
- `void accelerationValue(time_type)`
- `time_type accelerationThreshold() const`
- `void accelerationThreshold(time_type)`


### Virtual Methods
- `virtual ~IPushButton()`
- `virtual void onPushFn()`
- `virtual void onHoldFn()`
- `virtual void onReleaseFn()`
- `virtual void onDelayFn()`
- `virtual void onIdleFn()`


## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

