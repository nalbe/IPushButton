

/*

//GPIO FUNCTIONS
#define INPUT             0x00
#define INPUT_PULLUP      0x02
#define INPUT_PULLDOWN_16 0x04 // PULLDOWN only possible for pin16
#define OUTPUT            0x01
#define OUTPUT_OPEN_DRAIN 0x03
#define WAKEUP_PULLUP     0x05
#define WAKEUP_PULLDOWN   0x07
#define SPECIAL           0xF8 //defaults to the usable BUSes uart0rx/tx uart1tx and hspi
#define FUNCTION_0        0x08
#define FUNCTION_1        0x18
#define FUNCTION_2        0x28
#define FUNCTION_3        0x38
#define FUNCTION_4        0x48

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

//Interrupt Modes
#define RISING    0x01
#define FALLING   0x02
#define CHANGE    0x03
#define ONLOW     0x04
#define ONHIGH    0x05
#define ONLOW_WE  0x0C
#define ONHIGH_WE 0x0D

#define DEFAULT 1
#define EXTERNAL 0


// ...


#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))


// ...



#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


// ...



#define bit(b) (1UL << (b))
#define _BV(b) (1UL << (b))

*/





#pragma once
#include <windows.h>    // GetAsyncKeyState()
#include <chrono>       // Time since epoch in ms
#include <iostream>
#include <string>
#include <array>





#ifndef Arduino_DBG_
#define Arduino_DBG_




//typedef bool boolean;     //redefenition
typedef uint16_t word;
typedef uint8_t byte;



#ifndef min
#define min(x, y)       (((x) < (y)) ? (x) : (y))
#endif // min

#ifndef max
#define max(x, y)       (((x) > (y)) ? (x) : (y))
#endif // max

#ifndef lowByte
#define lowByte(w)      ((uint8_t) ((w) & 0xff))
#endif // lowByte

#ifndef highByte
#define highByte(w)     ((uint8_t) ((w) >> 8))
#endif // highByte

#ifndef LOW
#define LOW             0x0
#endif // LOW

#ifndef HIGH
#define HIGH            0x1
#endif // HIGH

#ifndef INPUT
#define INPUT           0x00
#endif // INPUT

#ifndef INPUT_PULLUP
#define INPUT_PULLUP    0x02
#endif // INPUT_PULLUP

#ifndef OUTPUT
#define OUTPUT          0x01
#endif // OUTPUT


// #include "Pins_Arduino.h"
#define LED_BUILTIN 2

static const uint8_t D0 = 'v';
static const uint8_t D1 = 'b';
static const uint8_t D2 = 'z';
static const uint8_t D3 = 'q';
static const uint8_t D4 = 'e';
static const uint8_t D5 = 'a';
static const uint8_t D6 = 'd';
static const uint8_t D7 = 'w';
static const uint8_t D8 = 'x';
static const uint8_t D9 = 's';
static const uint8_t D10 = 'y';
static const uint8_t D11 = 'w';
static const uint8_t D12 = 'd';
static const uint8_t D13 = 'a';
static const uint8_t D14 = 'e';
static const uint8_t D15 = 'q';

#ifndef PIN_A0
#define PIN_A0 ('k')
#endif /* PIN_A0 */

static const uint8_t A0 = PIN_A0;



using Pin_Mode_Array = std::array<uint8_t, 256>;
static Pin_Mode_Array pma{};


inline void pinMode(uint8_t pin_, uint8_t pin_mode_)
{
    pma[pin_] = pin_mode_;
}


inline unsigned long millis()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

inline int digitalRead(uint8_t pin_)
{
    return (!!(GetAsyncKeyState(pin_- 32) & 0x8000)) ^ (pma[pin_] >> 1);
}


inline void delay(size_t ms)
{
    Sleep(ms);
}


class Serial_
{
public:
    void begin(size_t) const {}

    void print(std::string s) const
    {
        std::cout << s;
    }

    template < typename T, typename = std::enable_if_t<std::is_integral_v<T>, T> > void print(T s) const
    {
        std::cout << std::to_string(s);
    }

    void println(std::string s) const
    {
        print(s);
        std::cout << '\n';
    }

    template<typename T> void println(T s) const
    {
        print(s);
        std::cout << '\n';
    }

};
static constexpr Serial_ Serial;





#endif // !Arduino_DBG_





