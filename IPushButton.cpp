#include "IPushButton.h"



IPushButton::IPushButton() :
    debounce_delay{ 50 },
    repeat_delay{},
    push_timestamp{},
    release_timestamp{},
    cycle_count{},
    mash_count{},
    is_enabled{},
    is_pushed{},
    is_inversed{},
    pin_id{},
    pin_mode{},
    current_state{ eState::idle }
{}

IPushButton::IPushButton(size_type id_, size_type mode_)
    : IPushButton()
{
    reset();
    pin_id = id_;
    pin_mode = mode_;
    is_inversed = (mode_ >> 1);
    pinMode(pin_id, pin_mode);  // set pinmode
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
    return millis() < (push_timestamp + (repeat_delay * cycle_count));
}

bool IPushButton::isInversed() const
{
    return is_inversed;
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

IPushButton::cnt_type IPushButton::mashCount() const
{
    return mash_count;
}

void IPushButton::inverse()
{
    is_inversed = !is_inversed;
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

    if (isDebounceDelay()) {
        if (is_pushed) {
            current_state = eState::delay;
            onDelayFn();
        }
        else {
            current_state = eState::idle;
            onIdleFn();
        }
    }
    // If button wasn't pushed, check it is.
    else if (!(is_pushed || bool(digitalRead(pin_id)) ^ is_inversed)) {
        current_state = eState::idle;
        onIdleFn();
    }
    // If just pushed.
    else if (!is_pushed) {
        bool is_mash_{ release_timestamp < push_timestamp + repeat_delay };
        is_pushed = true;
        ++cycle_count;
        push_timestamp = millis();
        current_state = eState::push;
        onPushFn();
        // Count seq fast push.
        if (is_mash_ && (push_timestamp < release_timestamp + repeat_delay)) {
            ++mash_count;
            current_state |= eState::mash;
        }
        else { mash_count = 0; }
    }
    // If button was pushed, check it still is.
    else if (!(bool(digitalRead(pin_id)) ^ is_inversed)) {
        is_pushed = false;
        cycle_count = 0;
        release_timestamp = millis();   // debounce on release also
        current_state = eState::release;
        onReleaseFn();
    }
    // If holded.
    else if (!isRepeatDelay()) {
        ++cycle_count;
        current_state = eState::hold;
        onHoldFn();
    }
    else {
        current_state = eState::delay;
        onDelayFn();
    }
    return;
}

void IPushButton::reset()
{
    debounce_delay = 50;
    repeat_delay = 0;
    push_timestamp = 0;
    release_timestamp = 0;
    cycle_count = 0;
    mash_count = 0;
    is_enabled = false;
    is_pushed = false;
    is_inversed = false;
    pin_id = 0;
    pin_mode = 0;
    current_state = eState::idle;
}



