#include "gamepad.h"
#include "gpaddon.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "storagemanager.h"

class UartXboxAddon : public GPAddon {
public:
    void setup() override {
        uart_init(uart0, 115200);
        gpio_set_function(0, GPIO_FUNC_UART);  // UART0 TX
        gpio_set_function(1, GPIO_FUNC_UART);  // UART0 RX
    }

    void process(Gamepad * gamepad) override {
        const uint8_t packetSize = 16;
        uint8_t data[packetSize];

        while (uart_is_readable(uart0)) {
            // Lê 1 byte para ver se é o início esperado (opcional, se tiver um header de sync)
            int read = uart_read_blocking(uart0, data, packetSize);
            if (read == packetSize) {
                gamepad->lx = data[0];
                gamepad->ly = data[1];
                gamepad->rx = data[2];
                gamepad->ry = data[3];
                gamepad->lt = data[4];
                gamepad->rt = data[5];

                gamepad->buttons = (data[6] << 0) |
                                   (data[7] << 1) |
                                   (data[8] << 2) |
                                   (data[9] << 3) |
                                   (data[10] << 4) |
                                   (data[11] << 5) |
                                   (data[12] << 6) |
                                   (data[13] << 7);

                gamepad->dpad = data[14]; // Hat switch
            }
        }
    }

    bool available() override {
        return true; // ou checar alguma flag via Storage se quiser
    }
};

GP_ADDON(UartXboxAddon);
