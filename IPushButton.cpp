#include "IPushButton.h"



IPushButton::IPushButton()
{
    reset();
}

IPushButton::IPushButton(const self_type& other_)
{
    *this = other_;
}

IPushButton::IPushButton(self_type&& other_)
{
    *this = std::move(other_);
}

IPushButton::IPushButton(size_type id_, size_type mode_)
{
    reset();
    pin_id = id_;
    pin_mode = mode_;
    is_inversed = (mode_ >> 1);
    pinMode(pin_id, pin_mode);  // set pinmode
    is_enabled = true;
}

IPushButton::self_type& IPushButton::operator=(const self_type& other_)
{
    this->debounce_delay = other_.debounce_delay;
    this->repeat_delay = other_.repeat_delay;
    this->push_timestamp = other_.push_timestamp;
    this->release_timestamp = other_.release_timestamp;
    this->cycle_count = other_.cycle_count;
    this->is_enabled = other_.is_enabled;
    this->is_pushed = other_.is_pushed;
    this->is_inversed = other_.is_inversed;
    this->pin_id = other_.pin_id;
    this->pin_mode = other_.pin_mode;
    this->current_state = other_.current_state;
    return *this;
}

IPushButton::self_type& IPushButton::operator=(self_type&& other_)
{
    this->debounce_delay = other_.debounce_delay;
    this->repeat_delay = other_.repeat_delay;
    this->push_timestamp = other_.push_timestamp;
    this->release_timestamp = other_.release_timestamp;
    this->cycle_count = other_.cycle_count;
    this->is_enabled = other_.is_enabled;
    this->is_pushed = other_.is_pushed;
    this->is_inversed = other_.is_inversed;
    this->pin_id = other_.pin_id;
    this->pin_mode = other_.pin_mode;
    this->current_state = other_.current_state;
    return *this;
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
    return millis() < ((push_timestamp > release_timestamp ? push_timestamp : release_timestamp) + debounce_delay);
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
}

IPushButton::size_type IPushButton::mode() const
{
    return pin_mode;
}

void IPushButton::mode(size_type mode_)
{
    pin_mode = mode_;
}

size_t IPushButton::debounceDelay() const
{
    return debounce_delay;
}

void IPushButton::debounceDelay(size_t delay_)
{
    debounce_delay = delay_;
}

size_t IPushButton::repeatDelay() const
{
    return repeat_delay;
}

size_t IPushButton::pushTime() const
{
    return push_timestamp;
}

size_t IPushButton::releaseTime() const
{
    return release_timestamp;
}

void IPushButton::repeatDelay(size_t delay_)
{
    repeat_delay = delay_;
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
        is_pushed = true;
        ++cycle_count;
        push_timestamp = millis();
        current_state = eState::push;
        onPushFn();
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
    is_enabled = false;
    is_pushed = false;
    is_inversed = false;
    pin_id = 0;
    pin_mode = 0;
    current_state = eState::idle;
}




