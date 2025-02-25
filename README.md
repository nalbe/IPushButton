
# IPushButton Library


## Overview
`IPushButton` is a C++ library designed to handle push button events such as push, hold, release, delay, and mash.
It provides a simple interface to manage button states and events with customizable debounce and repeat delays.


## Features
- Handle different button states: idle, push, hold, release, delay, and mash.
- Customizable debounce and repeat delays.
- Support for inversed button states.
- Easy integration with existing projects.

- 
## Installation
To use the `IPushButton` library in your project, follow these steps:

1. Clone the repository: `git clone https://github.com/nalbe/IPushButton.git`
2. Include the header file in your project: `#include "IPushButton.h"`
3. Create an instance of the `IPushButton` class: `IPushButton button(pin, mode);`
4. Call the `update()` function in your loop: `button.update();`    
5. Handle button events using the `onPushFn`, `onHoldFn`, `onReleaseFn`, `onDelayFn`, and `onIdleFn` functions.
	Or use the `getState()` function to get the current button state.
6. Customize the debounce and repeat delays using the `setDebounceDelay()` and `setRepeatDelay()` functions.


## Usage
Here is an example of how to use the `IPushButton` class:
```cpp
#include "IPushButton.h"

// Define a custom push button class inheriting from IPushButton
class MyPushButton : public IPushButton
{
protected:
	void onPushFn() override	{ /* Handle push event */ }
	void onHoldFn() override	{ /* Handle hold event */ }
	void onReleaseFn() override	{ /* Handle release event */ }
	void onDelayFn() override	{ /* Handle delay event */ }
	void onIdleFn() override	{ /* Handle idle event */ }
};

// Create an instance of the custom push button class. Set pin number and mode. 
MyPushButton button(2, IPushButton::Mode::PULLUP);

// Setup function
void setup()
{
	// Set debounce delay to 50 ms
	button.setDebounceDelay(50);
	// Set repeat delay to 500 ms
	button.setRepeatDelay(500);
}

// Update the button state in the loop.
void loop()
{
	// Update the button state
	button.update();
	// ... your code here
}
```

Another way to use the `IPushButton` class is to handle button events directly in the loop:
```cpp
#include "IPushButton.h"

// Create an instance of the IPushButton class. Set pin number and mode.
IPushButton button(2, IPushButton::Mode::PULLUP);

// Setup function
void setup()
{
	// Set debounce delay to 50 ms
	button.setDebounceDelay(50);
	// Set repeat delay to 500 ms
	button.setRepeatDelay(500);
}

// Update the button state in the loop.
void loop()
{
	// Update the button state
	button.update();
	// Handle button events
	switch (button.getState())
	{
	case IPushButton::State::PUSH:
		// Handle push event
		break;
	case IPushButton::State::HOLD:
		// Handle hold event
		break;
	case IPushButton::State::RELEASE:
		// Handle release event
		break;
	case IPushButton::State::DELAY:
		// Handle delay event
		break;
	case IPushButton::State::IDLE:
		// Handle idle event
		break;
	}
	// ... your code here
}
```


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
| Is within debounce delay?         |
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
|   • Detect mash (quick push):     |
|     - Increment mashCount         |
|     - Set state = PUSH_WITH_MASH  |
|   • Else: Reset mashCount         |
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
| Was the button pushed and now     |
| not pushed?                       |
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
| Is the button held and not within |
| repeat delay?                     |
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
- `bool isInversed() const`
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
- `cnt_type mashCount() const`
- `void inverse()`
- `void enable()`
- `void disable()`
- `eState state() const`
- `void update()`
- `void reset()`

### Virtual Methods
- `virtual void ~IPushButton()`
- `virtual void onPushFn()`
- `virtual void onHoldFn()`
- `virtual void onReleaseFn()`
- `virtual void onDelayFn()`
- `virtual void onIdleFn()`


## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

