#include <Arduino.h>
#define DEBUG 1

#define TIME_OUT 100000
#define KEEP_OUT_TIME 2000

enum direction_t{
    U_TURN      = 0,
    LEFT        = 1 << 2,
    FRONT       = 1 << 1,
    RIGHT       = 1
};

class FastPing{
    private:
    long _left_sensor{}, _right_sensor{}, _front_sensor{};
    long _start_time{};
    uint8_t _trigger;
    static FastPing* _instance;
    void pingAll();

    public:
    FastPing(uint8_t trigger, uint8_t left_echo_pin, uint8_t right_echo_pin, uint8_t front_echo_pin);
    uint8_t waitForAll();
    uint8_t greatest() const;
    void recv_left();
    void recv_right();
    void recv_front();
    static FastPing* getDefaultInstance();
};
