#ifndef _UART_XBOX_INPUT_H_
#define _UART_XBOX_INPUT_H_

#include <stdint.h>
#include "gpaddon.h"

#define UART_ID uart1            // Pode ser uart0 se preferir
#define UART_BAUDRATE 115200      // Ajuste conforme o dispositivo
#define UART_TX_PIN 0          // Ajuste conforme necessário
#define UART_RX_PIN 1           // Ajuste conforme necessário

class UartXboxInput : public GPAddon {
public:
    virtual bool available();       // Verifica se deve ativar o addon
    virtual void setup();           // Configura a UART
    virtual void process();         // Lê dados UART e atualiza Gamepad
    virtual void preprocess() {}
    virtual void postprocess(bool sent) {}
    virtual void reinit() {}

private:
    uint32_t nextPollMs = 0;
    bool buttonA = false;
    bool buttonB = false;
};

#endif // _UART_XBOX_INPUT_H_
