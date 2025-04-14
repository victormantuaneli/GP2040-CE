#ifndef _UART_XBOX_INPUT_H_
#define _UART_XBOX_INPUT_H_

#include <stdint.h>

class UartXboxInput : public GPAddon {
public:
    virtual bool available();
    virtual void setup();       // SNESpad Setup
    virtual void process();     // SNESpad Process
    virtual void preprocess() {}
    virtual void postprocess(bool sent) {}
    virtual void reinit() {}
    virtual std::string name() { return "UART Addon"; }
private:

};

#endif // _UART_XBOX_INPUT_H_
