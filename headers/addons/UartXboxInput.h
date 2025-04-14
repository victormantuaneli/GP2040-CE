#ifndef _UART_XBOX_INPUT_H_
#define _UART_XBOX_INPUT_H_

#include <stdint.h>

class UartXboxInput {
public:
    void setup();
    void process();

private:
    uint16_t getJoystickXValue();
    uint16_t getJoystickYValue();
};

#endif // _UART_XBOX_INPUT_H_
