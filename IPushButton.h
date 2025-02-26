#pragma once
#include <cstdint>
#include <Arduino.h>


class IPushButton
{
public:
    using self_type     = IPushButton;
    using size_type     = byte;
    using time_type     = decltype(millis()); // Follow the Arduino convention for time type
    using cnt_type      = time_type;

protected:
    // Enum to represent the state of the push button
    typedef struct { enum eState : byte { IDLE = 1 << 0, PUSH = 1 << 1, HOLD = 1 << 2, RELEASE = 1 << 3, DELAY = 1 << 4, RAPID = 1 << 5 }; }PushButtonState_;

public:
    using eState = PushButtonState_::eState;

protected:
    time_type debounce_delay;  // default: 50 ms
    time_type repeat_delay;
    time_type local_timestamp;
    time_type push_timestamp;
    time_type release_timestamp;
    cnt_type cycle_count;
    cnt_type rapid_count;
    size_type pin_id;
    size_type pin_mode;
    bool is_enabled;
    bool is_pushed;
    bool is_inverted;
    bool is_local;
    bool is_accelerated;
    eState current_state;

    struct {
        cnt_type offset;
        cnt_type threshold;
        cnt_type value;
        cnt_type ratio; // default: 1
    } acceleration;

protected:
    // Function to handle push event
    virtual void onPushFn();
    // Function to handle hold event
    virtual void onHoldFn();
    // Function to handle release event
    virtual void onReleaseFn();
    // Function to handle delay event
    virtual void onDelayFn();
    // Function to handle idle event
    virtual void onIdleFn();

private:
    // Acceleration offset calculation
    void accelerationOffset();

public:
    // Default destructor
    virtual ~IPushButton() = default;

    // Default constructor
    IPushButton();

    // Constructor with parameters
    IPushButton(size_type, size_type);

    // Check if the button is enabled
    bool isEnabled() const;

    // Check if debounce delay is active
    bool isDebounceDelay() const;

    // Check if repeat delay is active
    bool isRepeatDelay() const;

    // Check if the button is inversed
    bool isInverted() const;
    
    // Check if the button is used global or local time anchor
    bool isLocalTime() const;
    
    // Check if the button is accelerated
    bool isAccelerated() const;

    // Get the ID of the button
    size_type id() const;

    // Set the ID of the button
    void id(size_type);

    // Get the mode of the button
    size_type mode() const;

    // Set the mode of the button
    void mode(size_type);

    // Get the debounce delay
    time_type debounceDelay() const;

    // Set the debounce delay
    void debounceDelay(time_type);

    // Get the repeat delay
    time_type repeatDelay() const;

    // Set the repeat delay
    void repeatDelay(time_type);

    // Get the push timestamp
    time_type pushTime() const;

    // Get the release timestamp
    time_type releaseTime() const;

    // Returns the count of rapid clicks
    // `1` for a double-click, `2` for a triple-click, and so on
    // Example usage:
    //    if (!((rapidCount() + 1) % n)) { /* match every n-th click */ }
    //    if (rapidCount() & 1) { /* match every odd click */ }
    cnt_type rapidCount() const;

    // Set the button to use local time anchor
    void useLocalTime(bool);

    // Inverte logic by default if pinMode is INPUT_PULLUP
    void inverte();

    // Enable the button
    void enable();

    // Disable the button
    void disable();

    // Get the current state of the button
    eState state() const;

    // Update the button state
    void update();

    // Reset the button state
    void reset();


    // Toggle acceleration
    void useAcceleration(bool);

    // Get the acceleration value
    time_type accelerationValue() const;

    // Set the acceleration value
    void accelerationValue(time_type);

    // Get the acceleration threshold
    time_type accelerationThreshold() const;

    // Set the acceleration threshold
    void accelerationThreshold(time_type);

    // Get the acceleration threshold
    cnt_type accelerationRatio() const;

    // Set the acceleration threshold
    void accelerationRatio(cnt_type);

};





// Overload the bitwise OR operator for eState
constexpr IPushButton::eState operator|(IPushButton::eState lhs_, IPushButton::eState rhs_)
{
    return static_cast<IPushButton::eState>(static_cast<byte>(lhs_) | static_cast<byte>(rhs_));
}

// Overload the bitwise OR assignment operator for eState
inline IPushButton::eState operator|=(IPushButton::eState& lhs_, IPushButton::eState rhs_)
{
    return lhs_ = lhs_ | rhs_;
}



