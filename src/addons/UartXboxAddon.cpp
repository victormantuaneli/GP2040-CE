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

bool UartXboxInput::available() {
    return true;
}
// Função para inicializar a UART
void UartXboxInput::setup() {
    // Configura o UART
    uart_init(UART_ID, UART_BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    
    // Configura pinos de transmissão e recepção do UART
    uart_set_fifo_enabled(UART_ID, true);
    
    // Inicializa variáveis de controle
    buttonA = false;
    buttonB = false;
    buttonX = false;
    buttonY = false;
    buttonL = false;
    buttonR = false;
    dpadUp = false;
    dpadDown = false;
    dpadLeft = false;
    dpadRight = false;
    buttonSelect = false;
    buttonStart = false;

    nextTimer = getMillis();
}

// Função de processamento dos dados recebidos via UART
void UartXboxInput::process() {
    if (nextTimer < getMillis()) {
        // Leitura dos dados via UART
        uint8_t buffer[64];
			
				uart_read_blocking(UART_ID, buffer, sizeof(buffer));
				
				// Processa os dados e mapeia para os botões
				buttonA = (buffer[0] & 0x01) != 0;
				buttonB = (buffer[0] & 0x02) != 0;
				buttonX = (buffer[0] & 0x04) != 0;
				buttonY = (buffer[0] & 0x08) != 0;
				buttonL = (buffer[0] & 0x10) != 0;
				buttonR = (buffer[0] & 0x20) != 0;
				dpadUp = (buffer[0] & 0x40) != 0;
				dpadDown = (buffer[0] & 0x80) != 0;

				// Atualiza os valores dos joysticks com dados fictícios
				leftX = getJoystickXValue();  // Função fictícia
				leftY = getJoystickYValue();  // Função fictícia
				rightX = leftX;
				rightY = leftY;

        nextTimer = getMillis() + uIntervalMS;
    }

    // Atualiza o estado do gamepad
    Gamepad *gamepad = Storage::getInstance().GetGamepad();

    gamepad->state.lx = leftX;
    gamepad->state.ly = leftY;
    gamepad->state.rx = rightX;
    gamepad->state.ry = rightY;

    gamepad->hasAnalogTriggers = false;
    gamepad->hasLeftAnalogStick = true;

    // Mapeia os botões para o estado do gamepad
    if (buttonA) gamepad->state.buttons |= GAMEPAD_MASK_B2;
    if (buttonB) gamepad->state.buttons |= GAMEPAD_MASK_B1;
    if (buttonX) gamepad->state.buttons |= GAMEPAD_MASK_B4;
    if (buttonY) gamepad->state.buttons |= GAMEPAD_MASK_B3;
    if (buttonL) gamepad->state.buttons |= GAMEPAD_MASK_L1;
    if (buttonR) gamepad->state.buttons |= GAMEPAD_MASK_R1;
    if (buttonSelect) gamepad->state.buttons |= GAMEPAD_MASK_S1;
    if (buttonStart) gamepad->state.buttons |= GAMEPAD_MASK_S2;
    if (dpadUp) gamepad->state.dpad |= GAMEPAD_MASK_UP;
    if (dpadDown) gamepad->state.dpad |= GAMEPAD_MASK_DOWN;
    if (dpadLeft) gamepad->state.dpad |= GAMEPAD_MASK_LEFT;
    if (dpadRight) gamepad->state.dpad |= GAMEPAD_MASK_RIGHT;
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
