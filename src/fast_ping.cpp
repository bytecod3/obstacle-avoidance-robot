#include "fast_ping.h"

void left_echo_isr(){
    FastPing* ping = FastPing::getDefaultInstance();
    ping->recv_left();
}

void right_echo_isr(){
    FastPing* ping = FastPing::getDefaultInstance();
    ping->recv_right();
}

void front_echo_isr(){
    FastPing* ping = FastPing::getDefaultInstance();
    ping->recv_front();
}


FastPing::FastPing(uint8_t trigger, uint8_t left_echo_pin, uint8_t right_echo_pin, uint8_t front_echo_pin): _trigger(trigger), _left_pin(left_echo_pin), _right_pin(right_echo_pin){
    if(_instance == nullptr)
        _instance = this;
    attachInterrupt(left_echo_pin, left_echo_isr, FALLING);
    attachInterrupt(right_echo_pin, right_echo_isr, FALLING);
    attachInterrupt(front_echo_pin, front_echo_isr, FALLING);
}


void FastPing::waitForAll(){
    pingAll();

    while ((micros() - _start_time) < TIME_OUT)
    {
        if(_left_sensor != -1 && _right_sensor != -1 && _front_sensor != -1)
            break;
    }
}

void FastPing::pingAll(){
    _left_sensor = _right_sensor = _front_sensor = -1;
    digitalWrite(_trigger, 0);
    delayMicroseconds(4);
    digitalWrite(_trigger, 1);
	delayMicroseconds(10);
	digitalWrite(_trigger, 0);
    _start_time = micros();
}

void FastPing::recv_left(){
    _left_sensor = micros();
}

void FastPing::recv_right(){
    _right_sensor = micros();
}

void FastPing::recv_front(){
    _front_sensor = micros();
}

FastPing* FastPing::getDefaultInstance(){
    return _instance;
}