#pragma once
#include <cstdint>
#include <Arduino.h>



class IPushButton
{
public:
    using self_type     = IPushButton;
    using size_type     = byte;
    using time_type     = decltype(millis());
    using cnt_type      = time_type;

protected:
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
    virtual void onPushFn();

    virtual void onHoldFn();

    virtual void onReleaseFn();

    virtual void onDelayFn();

    virtual void onIdleFn();

public:
    virtual ~IPushButton() = default;

    IPushButton();

    IPushButton(size_type, size_type);


    bool isEnabled() const;

    bool isDebounceDelay() const;

    bool isRepeatDelay() const;

    bool isInversed() const;

    size_type id() const;

    void id(size_type);

    size_type mode() const;

    void mode(size_type);

    time_type debounceDelay() const;

    void debounceDelay(time_type);

    time_type repeatDelay() const;

    void repeatDelay(time_type);

    time_type pushTime() const;

    time_type releaseTime() const;

    cnt_type mashCount() const;
    // mashCount() returns the number of rapid clicks (e.g., for double-click detection).
    // Example:   
    //    if(!((mashCount() + 1) % n)) { /* Match each n-th click. */ };
    //    if (mashCount() & 1) { /* Match each odd click. */ };

    void inverse();

    void enable();

    void disable();

    eState state() const;

    void update();

    void reset();

};




constexpr IPushButton::eState operator|(IPushButton::eState lhs_, IPushButton::eState rhs_)
{
    return static_cast<IPushButton::eState>(static_cast<byte>(lhs_) | static_cast<byte>(rhs_));
}

inline IPushButton::eState operator|=(IPushButton::eState& lhs_, IPushButton::eState rhs_)
{
    return lhs_ = lhs_ | rhs_;
}





