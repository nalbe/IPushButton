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
    eState current_state;

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
};
