#include "addons/addon.hpp"
#include "gpaddon.h"
#include "gamepad_state.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

class UartXboxAddon : public GPAddon {
public:
    void setup() override {
        uart_init(uart0, 115200);
        gpio_set_function(0, GPIO_FUNC_UART); // TX
        gpio_set_function(1, GPIO_FUNC_UART); // RX
    }

    void preprocess(GamepadState *state) override {
        const uint8_t packetSize = 16;
        uint8_t data[packetSize];

        while (uart_is_readable(uart0)) {
            for (int i = 0; i < packetSize; ++i) {
                data[i] = uart_getc(uart0);
            }

            state->lx = data[0];
            state->ly = data[1];
            state->rx = data[2];
            state->ry = data[3];
            state->lt = data[4];
            state->rt = data[5];

            // Buttons: bits 0–13 (máximo 14 botões nesse exemplo)
            uint16_t buttons = 0;
            for (int i = 0; i < 2; i++) {
                buttons |= (data[6 + i] << (i * 8));
            }
            state->buttons = buttons;

            // D-Pad hat (0–8)
            state->hat = data[8];
        }
    }

    bool available() override {
        return true;
    }
};

GP_ADDON(UartXboxAddon);
