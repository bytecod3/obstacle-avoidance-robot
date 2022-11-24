#include "fast_ping.h"

FastPing* FastPing::_instance = nullptr;

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


FastPing::FastPing(uint8_t trigger, uint8_t left_echo_pin, uint8_t right_echo_pin, uint8_t front_echo_pin): _trigger(trigger){
    if(_instance == nullptr)
        _instance = this;

    pinMode(left_echo_pin, INPUT);
    pinMode(right_echo_pin, INPUT);
    pinMode(front_echo_pin, INPUT);
    pinMode(trigger, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(left_echo_pin), left_echo_isr, FALLING);
    attachInterrupt(digitalPinToInterrupt(right_echo_pin), right_echo_isr, FALLING);
    attachInterrupt(digitalPinToInterrupt(front_echo_pin), front_echo_isr, FALLING);
}


uint8_t FastPing::waitForAll(){
    pingAll();
    while ((micros() - _start_time) < TIME_OUT)
    {
        if(_left_sensor <= TIME_OUT && _right_sensor <= TIME_OUT && _front_sensor <= TIME_OUT)
            break;
    }

    uint8_t d_flag = 0;
    if(_front_sensor > KEEP_OUT_TIME)d_flag |= FRONT;
    if(_left_sensor > KEEP_OUT_TIME)d_flag |= LEFT;
    if(_right_sensor > KEEP_OUT_TIME)d_flag |= RIGHT;
#if DEBUG
    Serial.print("Left: "); Serial.print(_left_sensor); Serial.print("\t");
    Serial.print("Front: "); Serial.print(_front_sensor); Serial.print("\t");
    Serial.print("Right: "); Serial.print(_right_sensor); Serial.print("\t");
    Serial.println();
#endif
    return d_flag;
}

void FastPing::pingAll(){
    _left_sensor = _right_sensor = _front_sensor = TIME_OUT + 1;
    digitalWrite(_trigger, 0);
    delayMicroseconds(4);
    digitalWrite(_trigger, 1);
	delayMicroseconds(10);
	digitalWrite(_trigger, 0);
    _start_time = (long)micros();
}

void FastPing::recv_left(){
    _left_sensor = (long)micros() - _start_time;
}

void FastPing::recv_right(){
    _right_sensor = (long)micros() - _start_time;
}

void FastPing::recv_front(){
    _front_sensor = (long)micros() - _start_time;
}

FastPing* FastPing::getDefaultInstance(){
    return _instance;
}

uint8_t FastPing::greatest() const {
    if(_right_sensor > _left_sensor){
        if(_right_sensor > _front_sensor){
            return RIGHT;
        }
        return FRONT;
    }else{
        if(_left_sensor > _front_sensor){
            return LEFT;
        }
        return FRONT;
    }
}
