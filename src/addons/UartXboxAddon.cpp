#include "addons/UartXboxInput.h"
#include "drivermanager.h"
#include "storagemanager.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "helper.h"

// Definições de UART
#define UART_ID uart0
#define UART_BAUD_RATE 115200
#define UART_TX_PIN 0  // Defina o pino TX
#define UART_RX_PIN 1  // Defina o pino RX

// Variáveis de controle dos botões
bool buttonA = false;
bool buttonB = false;
bool buttonX = false;
bool buttonY = false;
bool buttonL = false;
bool buttonR = false;
bool dpadUp = false;
bool dpadDown = false;
bool dpadLeft = false;
bool dpadRight = false;
bool buttonSelect = false;
bool buttonStart = false;

uint16_t leftX = 0;
uint16_t leftY = 0;
uint16_t rightX = 0;
uint16_t rightY = 0;

uint32_t nextTimer = 0;
uint32_t uIntervalMS = 5;  // Intervalo entre as leituras em milissegundos
// Definindo constantes para o tamanho do pacote e os valores de cabeçalho e rodapé
#define FRAME_SIZE 64    // Defina o tamanho correto do pacote, dependendo do seu protocolo
#define HEADER 0xAA      // Exemplo de valor para o cabeçalho, substitua com o valor correto
#define FOOTER 0x55      // Exemplo de valor para o rodapé, substitua com o valor correto

void UartXboxInput::setup() {
    uart_init(UART_ID, UART_BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(UART_ID, true);
}

bool UartXboxInput::available() {
    return uart_is_readable(UART_ID);
}

void UartXboxInput::process() {
    if (uart_is_readable(UART_ID)) {
        uint8_t buffer[FRAME_SIZE];

        // Espera por um pacote válido
			  uart_read_blocking(UART_ID, buffer, sizeof(buffer));
        if (buffer[0] == HEADER) {
            uart_read_blocking(UART_ID, buffer + 1, FRAME_SIZE - 1);

            if (buffer[17] != FOOTER) return;

            // Validação simples de checksum
            uint8_t sum = 0;
            for (int i = 1; i <= 15; i++) sum += buffer[i];
            if (sum % 256 != buffer[16]) return;

            // Interpretar os dados
            uint8_t buttons_lo = buffer[1];
            uint8_t buttons_hi = buffer[2];
            uint16_t lx = buffer[3] | (buffer[4] << 8);
            uint16_t ly = buffer[5] | (buffer[6] << 8);
            uint16_t rx = buffer[7] | (buffer[8] << 8);
            uint16_t ry = buffer[9] | (buffer[10] << 8);
            uint16_t lt = buffer[11] | (buffer[12] << 8);
            uint16_t rt = buffer[13] | (buffer[14] << 8);
            uint8_t dpad = buffer[15];

            Gamepad *gamepad = Storage::getInstance().GetGamepad();

            gamepad->state.lx = lx;
            gamepad->state.ly = ly;
            gamepad->state.rx = rx;
            gamepad->state.ry = ry;
            gamepad->state.lt = lt;
            gamepad->state.rt = rt;

            gamepad->hasAnalogTriggers = true;
            gamepad->hasLeftAnalogStick = true;
            gamepad->hasRightAnalogStick = true;

            uint16_t buttons = buttons_lo | (buttons_hi << 8);

            if (buttons & (1 << 0)) gamepad->state.buttons |= GAMEPAD_MASK_B2;  // A
            if (buttons & (1 << 1)) gamepad->state.buttons |= GAMEPAD_MASK_B1;  // B
            if (buttons & (1 << 2)) gamepad->state.buttons |= GAMEPAD_MASK_B4;  // X
            if (buttons & (1 << 3)) gamepad->state.buttons |= GAMEPAD_MASK_B3;  // Y
            if (buttons & (1 << 4)) gamepad->state.buttons |= GAMEPAD_MASK_L1;  // LB
            if (buttons & (1 << 5)) gamepad->state.buttons |= GAMEPAD_MASK_R1;  // RB
            if (buttons & (1 << 6)) gamepad->state.buttons |= GAMEPAD_MASK_S1;  // View
            if (buttons & (1 << 7)) gamepad->state.buttons |= GAMEPAD_MASK_S2;  // Menu
            if (buttons & (1 << 8)) gamepad->state.buttons |= GAMEPAD_MASK_A1;  // Xbox
            if (buttons & (1 << 9)) gamepad->state.buttons |= GAMEPAD_MASK_L3;  // L3
            if (buttons & (1 << 10)) gamepad->state.buttons |= GAMEPAD_MASK_R3; // R3

            if (dpad & 0x01) gamepad->state.dpad |= GAMEPAD_MASK_UP;
            if (dpad & 0x02) gamepad->state.dpad |= GAMEPAD_MASK_DOWN;
            if (dpad & 0x04) gamepad->state.dpad |= GAMEPAD_MASK_LEFT;
            if (dpad & 0x08) gamepad->state.dpad |= GAMEPAD_MASK_RIGHT;
        }
    }
}

// Funções fictícias para simulação de leitura dos joysticks (substitua com sua lógica real)
uint16_t UartXboxInput::getJoystickXValue() {
    return 0;  // Substitua com a leitura real do joystick
}

uint16_t UartXboxInput::getJoystickYValue() {
    return 0;  // Substitua com a leitura real do joystick
}

uint16_t UartXboxInput::map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
