#include <NewPing.h>

#define TIME_OUT 1000000

class FastPing{
    private:
    unsigned long _left_sensor, _right_sensor, _front_sensor;
    unsigned long _start_time;
    uint8_t _left_pin, _right_pin, _front_pin, _trigger;
    static FastPing* _instance;
    void pingAll();

    public:
    FastPing(uint8_t trigger, uint8_t left_echo_pin, uint8_t right_echo_pin, uint8_t front_echo_pin);
    void waitForAll();
    void recv_left();
    void recv_right();
    void recv_front();
    static FastPing* getDefaultInstance();
};
