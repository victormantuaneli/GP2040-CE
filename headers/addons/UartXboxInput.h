#ifndef _UART_XBOX_INPUT_H_
#define _UART_XBOX_INPUT_H_

#include <stdint.h>
#include "addons/gpaddon.h"
#include "gpaddon.h"
#include "gamepad.h"

class UartXboxInput : public GPAddon {
public:
    virtual bool available();
    virtual void setup();       // SNESpad Setup
    virtual void process();     // SNESpad Process
    virtual void preprocess() {}
    virtual void postprocess(bool sent) {}
    virtual void reinit() {}
private:

};

#endif // _UART_XBOX_INPUT_H_
