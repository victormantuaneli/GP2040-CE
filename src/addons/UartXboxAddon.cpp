#include "gpaddon.h"
#include "gamepad.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define UART_PACKET_SIZE 15

class UartXboxAddon : public GPAddon {
public:
    void setup() override {
        uart_init(UART_ID, BAUD_RATE);
        gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
        gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    }

    void process(Gamepad * gamepad) override {
        if (uart_is_readable(UART_ID)) {
            uint8_t data[UART_PACKET_SIZE];
            int read = uart_read_blocking(UART_ID, data, UART_PACKET_SIZE);
            if (read == UART_PACKET_SIZE) {
                gamepad->state.lx = data[0];
                gamepad->state.ly = data[1];
                gamepad->state.rx = data[2];
                gamepad->state.ry = data[3];
                gamepad->state.lt = data[4];
                gamepad->state.rt = data[5];

                gamepad->state.buttons = (data[6] << 0) |
                                         (data[7] << 8) |
                                         (data[8] << 16) |
                                         (data[9] << 24);

                gamepad->state.miscButtons = (data[10] << 0) |
                                             (data[11] << 8);

                gamepad->state.dpad = data[12];
            }
        }
    }
};

GP_ADDON(UartXboxAddon);
