#include "IPushButton.h"


IPushButton::IPushButton()
{
    reset();
}

IPushButton::IPushButton(size_type id_, size_type mode_)
    : IPushButton()
{
    pin_id = id_;
    pin_mode = mode_;
    is_inverted = (mode_ >> 1);
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
        local_timestamp + repeat_delay : 
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

void IPushButton::id(size_type id_)
{
    pin_id = id_;
    pinMode(pin_id, pin_mode);
}

IPushButton::size_type IPushButton::mode() const
{
    return pin_mode;
}

void IPushButton::mode(size_type mode_)
{
    pin_mode = mode_;
    pinMode(pin_id, pin_mode);
}

IPushButton::time_type IPushButton::debounceDelay() const
{
    return debounce_delay;
}

void IPushButton::debounceDelay(time_type delay_)
{
    debounce_delay = delay_;
}

IPushButton::time_type IPushButton::repeatDelay() const
{
    return repeat_delay;
}

void IPushButton::repeatDelay(time_type delay_)
{
    repeat_delay = delay_;
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

void IPushButton::inverte()
{
    is_inverted = !is_inverted;
}

void IPushButton::useLocalTime(bool b_)
{
    is_local = b_;
}

void IPushButton::useAcceleration(bool b_)
{
    is_accelerated = b_;
}

void IPushButton::enable()
{
    is_enabled = true;
}

void IPushButton::disable()
{
    is_enabled = false;
}

IPushButton::eState IPushButton::state() const
{
    return current_state;
}

void IPushButton::update()
{
    if (!is_enabled) { return; }

    // If within the debounce delay, check if the button is still pushed.
    if (isDebounceDelay()) {
        if (is_pushed) {
            current_state = eState::DELAY;
            onDelayFn();
        }
        else {
            current_state = eState::IDLE;
            onIdleFn();
        }
    }
    // If button wasn't pushed, check it is.
    else if (!(is_pushed || bool(digitalRead(pin_id)) ^ is_inverted)) {
        current_state = eState::IDLE;
        onIdleFn();
    }
    // If just pushed.
    else if (!is_pushed) {
        bool is_mash_{ release_timestamp < push_timestamp + repeat_delay };
        is_pushed = true;
        ++cycle_count;
        push_timestamp = millis();
        current_state = eState::PUSH;
        onPushFn();
        // Rapid push handling.
        if (is_mash_ && (push_timestamp < release_timestamp + repeat_delay)) {
            ++rapid_count;
			current_state |= eState::RAPID; // hold the both `PUSH` and `RAPID` states
        }
        else { rapid_count = 0; }
    }
    // If button was pushed, check it still is.
    else if (!(bool(digitalRead(pin_id)) ^ is_inverted)) {
        is_pushed = false;
        cycle_count = 0;
        release_timestamp = millis(); // debounce on release also
        current_state = eState::RELEASE;
        onReleaseFn();
		acceleration.offset = 0;
    }
    // If holded.
    else if (!isRepeatDelay()) {
        local_timestamp = millis();
        ++cycle_count;
        current_state = eState::HOLD;
        onHoldFn();
        // Acceleration handling.
        if (is_accelerated) { accelerationOffset(); }
    }
    // Set state to delay.
    else {
        current_state = eState::DELAY;
        onDelayFn();
    }
}

void IPushButton::reset()
{
    debounce_delay = 50;
    repeat_delay = 0;
    local_timestamp = 0;
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
    acceleration = { 0, 0, 0, 1 };
}


void IPushButton::accelerationOffset()
{
    if (is_local) {
        if (repeat_delay >= acceleration.threshold + acceleration.value * acceleration.ratio * (cycle_count - 1)) {
            acceleration.offset = acceleration.value * acceleration.ratio * (cycle_count - 1);
        }
        else { acceleration.offset = repeat_delay - acceleration.threshold; }
    }
    else {
        if (repeat_delay >= acceleration.threshold + acceleration.value * acceleration.ratio * (cycle_count - 1)) {
            acceleration.offset = (cycle_count * acceleration.value * acceleration.ratio * (cycle_count - 1)) >> 1;
        }
        else { acceleration.offset += repeat_delay - acceleration.threshold; }
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

IPushButton::cnt_type IPushButton::accelerationRatio() const
{
    return acceleration.ratio;
}

void IPushButton::accelerationRatio(cnt_type value_)
{
    acceleration.ratio = value_;
}


