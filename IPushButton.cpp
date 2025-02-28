#include <Arduino.h>
#include "IPushButton.h"


IPushButton::IPushButton()
{
    reset();
}

IPushButton::IPushButton(size_type pinID_, size_type pinMODE_)
    : IPushButton()
{
    pin_id = pinID_;
    pin_mode = pinMODE_;
    is_inverted = (pinMODE_ >> 1);
    pinMode(pin_id, pin_mode); // set pinmode
    is_enabled = true;
}


void IPushButton::onPushFn() {}

void IPushButton::onHoldFn() {}

void IPushButton::onReleaseFn() {}

void IPushButton::onDelayFn() {}

void IPushButton::onIdleFn() {}


bool IPushButton::isEnabled() const
{
    return is_enabled;
}

bool IPushButton::isDebounceDelay() const
{
    return millis() < (max(push_timestamp, release_timestamp) + debounce_delay);
}

bool IPushButton::isRepeatDelay() const
{
    return millis() + acceleration.offset < (is_local ? 
        hold_timestamp + repeat_delay : 
        push_timestamp + (repeat_delay * cycle_count));
}

bool IPushButton::isInverted() const
{
    return is_inverted;
}

bool IPushButton::isLocalTime() const
{
    return is_local;
}

bool IPushButton::isAccelerated() const
{
    return is_accelerated;
}

IPushButton::size_type IPushButton::id() const
{
    return pin_id;
}

void IPushButton::id(size_type value_)
{
    pin_id = value_;
    pinMode(pin_id, pin_mode);
}

IPushButton::size_type IPushButton::mode() const
{
    return pin_mode;
}

void IPushButton::mode(size_type value_)
{
    pin_mode = value_;
    pinMode(pin_id, pin_mode);
}

IPushButton::time_type IPushButton::debounceDelay() const
{
    return debounce_delay;
}

void IPushButton::debounceDelay(time_type value_)
{
    debounce_delay = value_;
}

IPushButton::time_type IPushButton::repeatDelay() const
{
    return repeat_delay;
}

void IPushButton::repeatDelay(time_type value_)
{
    repeat_delay = value_;
}

IPushButton::time_type IPushButton::pushTime() const
{
    return push_timestamp;
}

IPushButton::time_type IPushButton::releaseTime() const
{
    return release_timestamp;
}

IPushButton::cnt_type IPushButton::rapidCount() const
{
    return rapid_count;
}

void IPushButton::invert(bool value_)
{
    is_inverted = value_;
}

void IPushButton::useLocalTime(bool value_)
{
    is_local = value_;
}

void IPushButton::accelerate(bool value_)
{
    is_accelerated = value_;
}

void IPushButton::enable(bool value_)
{
    is_enabled = value_;
}

IPushButton::eState IPushButton::state() const
{
    return current_state;
}

void IPushButton::update()
{
	// If disabled.
    if (!is_enabled) {
		return;
	}

    // Within the debounce delay.
    if (isDebounceDelay()) {
		// Still pushed.
        if (is_pushed) {
            current_state = eState::DELAY;
            onDelayFn();
        }
		// Released.
        else {
            current_state = eState::IDLE;
            onIdleFn();
		}
		return;
    }

	bool isActiveNow = bool(digitalRead(pin_id)) ^ is_inverted;

    // Was not pushed.
    if (!is_pushed and !isActiveNow) {
        current_state = eState::IDLE;
        onIdleFn();
		return;
    }

    // Is just pushed.
    if (!is_pushed) {
        bool is_mash_{ release_timestamp < push_timestamp + repeat_delay };
        is_pushed = true;
        ++cycle_count;
        push_timestamp = millis();
        current_state = eState::PUSH;
        // Rapid push handling.
        if (is_mash_ && (push_timestamp < release_timestamp + repeat_delay)) {
            ++rapid_count;
            current_state |= eState::RAPID; // hold the both `PUSH` and `RAPID` states
        }
        else { rapid_count = 0; }
        onPushFn();
		return;
    }

    // Was released.
    if (!isActiveNow) {
        is_pushed = false;
        cycle_count = 0;
        release_timestamp = millis(); // debounce on release also
        current_state = eState::RELEASE;
        acceleration.offset = 0;
        onReleaseFn();
		return;
    }

    // Is holded.
    if (!isRepeatDelay()) {
        hold_timestamp = millis();
        ++cycle_count;
        current_state = eState::HOLD;
        // Acceleration handling.
        if (is_accelerated) { AccOffsetCalc__(); }
        onHoldFn();
		return;
    }

    // Within the repeat delay.
    {
        current_state = eState::DELAY;
        onDelayFn();
		return;
    }
}

void IPushButton::reset()
{
    debounce_delay = 50;
    repeat_delay = 0;
    hold_timestamp = 0;
    push_timestamp = 0;
    release_timestamp = 0;
    cycle_count = 0;
    rapid_count = 0;
    is_enabled = false;
    is_pushed = false;
    is_inverted = false;
    is_local = false;
    is_accelerated = false;
    pin_id = 0;
    pin_mode = 0;
    current_state = eState::IDLE;
    acceleration = { 0, 0, 0 };
}


void IPushButton::AccOffsetCalc__()
{
	if (repeat_delay >= acceleration.threshold + acceleration.value * (cycle_count - 1)) {
		acceleration.offset += acceleration.value * (is_local ? 1 : cycle_count - 1);
	}
	else {
		acceleration.offset += repeat_delay - acceleration.threshold - (is_local ? acceleration.offset : 0);
	}
}

IPushButton::time_type IPushButton::accelerationValue() const
{
    return acceleration.value;
}

void IPushButton::accelerationValue(time_type value_)
{
    acceleration.value = value_;
}

IPushButton::time_type IPushButton::accelerationThreshold() const
{
    return acceleration.threshold;
}

void IPushButton::accelerationThreshold(time_type value_)
{
    acceleration.threshold = value_;
}




