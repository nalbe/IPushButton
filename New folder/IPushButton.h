#pragma once
#include <cstdint>
#include <Arduino.h>

// Interface for a push button with various states and functionalities
class IPushButton
{
public:
    using self_type     = IPushButton;
    using size_type     = byte;
    using time_type     = decltype(millis());
    using cnt_type      = time_type;

protected:
    // Enum to represent the state of the push button
    typedef struct { enum eState : byte { idle = 1 << 0, push = 1 << 1, hold = 1 << 2, release = 1 << 3, delay = 1 << 4, mash = 1 << 5 }; }PushButtonState_;

public:
    using eState = PushButtonState_::eState;

protected:
    time_type debounce_delay;  // default: 50 ms.
    time_type repeat_delay;
    time_type push_timestamp;
    time_type release_timestamp;
    cnt_type cycle_count;
    cnt_type mash_count;
    size_type pin_id;
    size_type pin_mode;
    bool is_enabled;
    bool is_pushed;
    bool is_inversed;
	bool is_accelerated;
    eState current_state;

	// The acceleration is achieved by reducing the delay in registering repeated clicks. -->
	// <-- The shorter the delay, the more often the acceleration is triggered.
	// The result is a smooth acceleration effect.
	struct {
		// Stored original value for delay. Restore `repeat_delay` value when the button is released.
		cnt_type stored_value;
		// Acceleration threshold. The button repeat delay will be reduced until threshold is reached.
		cnt_type threshold;
		// Acceleration delta. The button repeat delay will be reduced by %delta% every %period% click.
		cnt_type delta;
		// Acceleration period. The button repeat delay will be reduced every %period% click.
		cnt_type period;
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

public:
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
    bool isInversed() const;
    
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

    // mashCount() returns the number of rapid clicks (e.g., for double-click detection).
    // Example:   
    //    if(!((mashCount() + 1) % n)) { /* Match each n-th click. */ };
    //    if (mashCount() & 1) { /* Match each odd click. */ };
    cnt_type mashCount() const;

    // Inverse the button state
    void inverse();

    // Toggle acceleration
    void accelerate();

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


	cnt_type accelerationDelta() const;

	cnt_type accelerationThreshold() const;

	void accelerationDelta(cnt_type);

	void accelerationThreshold(cnt_type);

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
