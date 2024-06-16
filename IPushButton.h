#pragma once
#include <cstdint>
#include <Arduino.h>



class IPushButton
{
public:
    using self_type = IPushButton;
    using size_type = uint8_t;

protected:
    typedef struct{ enum eState : size_type { none, idle =1<<0, push =1<<1, hold =1<<2, release =1<<3, delay =1<<4 }; }PushButtonState_;

public:
    using eState = PushButtonState_::eState;

protected:
    size_t debounce_delay;  // default: 50 ms.
    size_t repeat_delay;
    size_t push_timestamp;
    size_t release_timestamp;
    size_t cycle_count;
    bool is_enabled;
    bool is_pushed;
    bool is_inversed;
    size_type pin_id;
    size_type pin_mode;
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

    IPushButton(const self_type&);

    IPushButton(self_type&&);

    IPushButton(size_type, size_type);

    self_type& operator=(const self_type&);

    self_type& operator=(self_type&&);


    bool isEnabled() const;

    bool isDebounceDelay() const;

    bool isRepeatDelay() const;

    bool isInversed() const;

    size_type id() const;

    void id(size_type);

    size_type mode() const;

    void mode(size_type);

    size_t debounceDelay() const;

    void debounceDelay(size_t);

    size_t repeatDelay() const;

    void repeatDelay(size_t);

    size_t pushTime() const;

    size_t releaseTime() const;

    void inverse();

    void enable();

    void disable();

    eState state() const;

    void update();

    void reset();

};




constexpr IPushButton::eState operator|(IPushButton::eState lhs_, IPushButton::eState rhs_)
{
    return static_cast<IPushButton::eState>(static_cast<IPushButton::size_type>(lhs_) | static_cast<IPushButton::size_type>(rhs_));
}





