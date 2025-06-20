#include <Custom/test/Arduino.h>



class IPushButton
{
public:
	using self_type  = IPushButton;
	using pin_t      = byte;
	using millis_t   = decltype(millis()); // follow the Arduino convention for time type
	using size_type  = millis_t;


protected:
	// Enum to represent the state of the push button
	typedef struct { enum eState : byte {
		NONE,
		IDLE     = 1 << 0,
		PUSH     = 1 << 1,
		HOLD     = 1 << 2,
		RELEASE  = 1 << 3,
		DELAY    = 1 << 4,
		RAPID    = 1 << 5
	}; } PushButtonState_;


public:
	using eState = PushButtonState_::eState;


protected:
	millis_t debounce_delay;  // default: 50 ms
	millis_t repeat_delay;
	millis_t hold_timestamp;
	millis_t push_timestamp;
	millis_t release_timestamp;
	size_type cycle_count;
	size_type rapid_count;
	pin_t pin_id;
	pin_t pin_mode;
	bool is_enabled;
	bool is_pushed;
	bool is_inverted;
	bool is_local;
	bool is_accelerated;
	eState current_state;

	struct {
		size_type offset;
		size_type threshold;
		size_type value;
	} acceleration;


protected:
	// Function to handle push event
	virtual void onPushFn() {};
	// Function to handle hold event
	virtual void onHoldFn() {};
	// Function to handle release event
	virtual void onReleaseFn() {};
	// Function to handle delay event
	virtual void onDelayFn() {};
	// Function to handle idle event
	virtual void onIdleFn() {};


private:
	// Private helper function for calculate acceleration offset
	void AccOffsetCalc__()
	{
		if (repeat_delay >= acceleration.threshold + acceleration.value * (cycle_count - 1)) {
			acceleration.offset += acceleration.value * (is_local
				? 1
				: cycle_count - 1);
		}
		else {
			acceleration.offset += repeat_delay - acceleration.threshold - (is_local
				? acceleration.offset
				: 0);
		}
	}


public:
	// Default destructor
	virtual ~IPushButton() = default;

	// Default constructor
	IPushButton()
	{
		reset();
	}

	// Constructor with parameters
	IPushButton(pin_t pinID_, pin_t pinMODE_)
		: IPushButton()
	{
		pin_id = pinID_;
		pin_mode = pinMODE_;
		is_inverted = (pinMODE_ >> 1);
		pinMode(pin_id, pin_mode); // set pinmode
		enable();
	}


public:
	// Check if the button is enabled
	bool isEnabled() const
	{
		return is_enabled;
	}

	// Check if the debounce delay is active
	bool isDebounceDelay() const
	{
		return millis() < (max(push_timestamp, release_timestamp) + debounce_delay);
	}

	// Check if the repeat delay is active
	bool isRepeatDelay() const
	{
		return millis() + acceleration.offset < (is_local
			? hold_timestamp + repeat_delay
			: push_timestamp + (repeat_delay * cycle_count));
	}

	// Check if the button logic is inverted
	bool isInverted() const
	{
		return is_inverted;
	}

	// Check if local or global time is used
	bool isLocalTime() const
	{
		return is_local;
	}

	// Check if the button is accelerated
	bool isAccelerated() const
	{
		return is_accelerated;
	}

	// Get the state of the button
	eState state() const
	{
		return current_state;
	}

	// Get the ID of the button
	pin_t id() const
	{
		return pin_id;
	}

	// Get the mode of the button
	pin_t mode() const
	{
		return pin_mode;
	}

	// Get the debounce delay
	millis_t debounceDelay() const
	{
		return debounce_delay;
	}

	// Get the repeat delay
	millis_t repeatDelay() const
	{
		return repeat_delay;
	}

	// Get the push timestamp
	millis_t pushTime() const
	{
		return push_timestamp;
	}

	// Get the release timestamp
	millis_t releaseTime() const
	{
		return release_timestamp;
	}

	// Get the rapid clicks count
	size_type rapidCount() const
	{
		return rapid_count;
	}

	// Get the acceleration value
	millis_t accelerationValue() const
	{
		return acceleration.value;
	}

	// Get the acceleration threshold
	millis_t accelerationThreshold() const
	{
		return acceleration.threshold;
	}


	// Set the ID of the button
	void id(pin_t value_)
	{
		pin_id = value_;
		pinMode(pin_id, pin_mode);
	}

	// Set the mode of the button
	void mode(pin_t value_)
	{
		pin_mode = value_;
		pinMode(pin_id, pin_mode);
	}

	// Set the debounce delay
	void debounceDelay(millis_t value_)
	{
		debounce_delay = value_;
	}

	// Set the repeat delay
	void repeatDelay(millis_t value_)
	{
		repeat_delay = value_;
	}

	// Set the button to use local time anchor
	void useLocalTime(bool value_)
	{
		is_local = value_;
	}

	// Set the acceleration value
	void accelerationValue(millis_t value_)
	{
		acceleration.value = value_;
	}

	// Set the acceleration threshold
	void accelerationThreshold(millis_t value_)
	{
		acceleration.threshold = value_;
	}

	// Invert logic. Used by default if pinMode is set to INPUT_PULLUP
	void invert(bool value_)
	{
		is_inverted = value_;
	}

	// Enable / Disable acceleration
	void accelerate(bool value_)
	{
		is_accelerated = value_;
	}

	// Enable / Disable the button
	void enable(bool value_ = true)
	{
		is_enabled = value_;
		current_state = value_
			? eState::IDLE
			: eState::NONE;
	}

	// Disable the button
	void disable()
	{
		enable(false);
	}

	// Update the button state
	void update()
	{
		// Return if disabled
		if (!is_enabled) { return; }

		// Within the debounce delay
		if (isDebounceDelay()) {
			// Still pushed
			if (is_pushed) {
				current_state = eState::DELAY;
				onDelayFn();
			}
			// Released
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
				current_state = static_cast<eState>(current_state | eState::RAPID); // hold the both `PUSH` and `RAPID` states
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

	// Reset the button state
	void reset()
	{
		debounce_delay     = 50;
		repeat_delay       = {};
		hold_timestamp     = {};
		push_timestamp     = {};
		release_timestamp  = {};
		cycle_count        = {};
		rapid_count        = {};
		is_enabled         = {};
		is_pushed          = {};
		is_inverted        = {};
		is_local           = {};
		is_accelerated     = {};
		pin_id             = {};
		pin_mode           = {};
		current_state      = {};
		acceleration       = {};
	}

};



// Bitwise OR operator
constexpr IPushButton::eState operator|(const IPushButton::eState& lhs_, const IPushButton::eState& rhs_)
{
	return static_cast<IPushButton::eState>(
		static_cast<byte>(lhs_) | static_cast<byte>(rhs_)
		);
}
// Bitwise OR assignment operator
inline IPushButton::eState& operator|=(IPushButton::eState& lhs_, const IPushButton::eState& rhs_)
{
	return lhs_ = (lhs_ | rhs_);
}
// Bitwise AND operator
constexpr IPushButton::eState operator&(const IPushButton::eState& lhs_, const IPushButton::eState& rhs_)
{
	return static_cast<IPushButton::eState>(
		static_cast<byte>(lhs_) & static_cast<byte>(rhs_)
		);
}
// Bitwise AND assignment operator
inline IPushButton::eState& operator&=(IPushButton::eState& lhs_, const IPushButton::eState& rhs_)
{
	return lhs_ = (lhs_ & rhs_);
}


