#ifndef _UART_XBOX_INPUT_H_
#define _UART_XBOX_INPUT_H_

#include <string>
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
		virtual std::string name() override { return "UartXboxInput"; }


private:
    uint32_t nextPollMs = 0;
    bool buttonA = false;
    bool buttonB = false;
    uint16_t leftX = 0;
    uint16_t leftY = 0;


    uint16_t getJoystickXValue();
    uint16_t getJoystickYValue();

};

#endif // _UART_XBOX_INPUT_H_
