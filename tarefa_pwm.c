#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// --------------------------
// Definições de pinos
// --------------------------
#define SERVO_PIN     22  // Servo no GPIO 22
#define LED_PWM_PIN   12  // LED no GPIO 12, controlado por PWM

// --------------------------
// Definições do servo
// --------------------------
// Frequência ~50 Hz = período 20 ms = 20000 us
#define SERVO_PERIOD_US     20000

// Pulsos típicos para ângulos
#define SERVO_0_US          500   // ~0°
#define SERVO_90_US         1470  // ~90°
#define SERVO_180_US        2400  // ~180°

// --------------------------
// Definições do LED PWM
// --------------------------
// Vamos definir o LED para 1 kHz, por exemplo
// Se clock de ~125MHz, definimos divisor de 125 => 1MHz
// e wrap de 1000 => período = 1000 us => 1 kHz
#define LED_WRAP  1000
#define LED_DIV   125.0f

// Mapeamos o pulso do servo (500..2400us) para o duty do LED (0..1000)
static inline uint16_t map_servo_pulse_to_led_level(uint16_t pulse_us) {
    // pulse_us em [500..2400]
    if (pulse_us < SERVO_0_US)     pulse_us = SERVO_0_US;   // saturação
    if (pulse_us > SERVO_180_US)   pulse_us = SERVO_180_US;  // saturação

    // Fazemos regra de 3:
    // servo_pulse = 500 => led_level=0
    // servo_pulse = 2400 => led_level=1000
    // led_level = (pulse_us - 500) * 1000 / (2400 - 500)
    return (uint16_t)(((pulse_us - SERVO_0_US) * LED_WRAP) / (SERVO_180_US - SERVO_0_US));
}

// --------------------------
// Variáveis globais para PWM do servo
// --------------------------
static uint servo_slice_num;
static uint servo_channel;

// Função para ajustar pulso do servo
static void set_servo_pulse(uint16_t pulse_us) {
    if (pulse_us < SERVO_0_US)   pulse_us = SERVO_0_US;
    if (pulse_us > SERVO_180_US) pulse_us = SERVO_180_US;
    // Ajusta duty no servo (1 tick = 1 us)
    pwm_set_chan_level(servo_slice_num, servo_channel, pulse_us);
}

// --------------------------
// Variáveis globais para PWM do LED
// --------------------------
static uint led_slice_num;
static uint led_channel;

// Função para ajustar duty do LED em [0..1000]
static void set_led_level(uint16_t level) {
    if (level > LED_WRAP) level = LED_WRAP;
    pwm_set_chan_level(led_slice_num, led_channel, level);
}

// --------------------------
// Programa Principal
// --------------------------
int main() {
    stdio_init_all();

    // --------------------------
    // Configuração PWM do Servo
    // --------------------------
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    servo_slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    servo_channel   = pwm_gpio_to_channel(SERVO_PIN);

    // Divisor de 125 => 1 tick = 1 us; wrap=20000 => período=20000 us => 50Hz
    pwm_set_clkdiv(servo_slice_num, 125.0f);
    pwm_set_wrap(servo_slice_num, SERVO_PERIOD_US);

    pwm_set_chan_level(servo_slice_num, servo_channel, 0);
    pwm_set_enabled(servo_slice_num, true);

    // --------------------------
    // Configuração PWM do LED
    // --------------------------
    gpio_set_function(LED_PWM_PIN, GPIO_FUNC_PWM);
    led_slice_num = pwm_gpio_to_slice_num(LED_PWM_PIN);
    led_channel   = pwm_gpio_to_channel(LED_PWM_PIN);

    // Frequência ~1kHz: wrap=1000, divisor=125 => 125MHz/125=1MHz => período=1000us => 1kHz
    pwm_set_clkdiv(led_slice_num, LED_DIV);
    pwm_set_wrap(led_slice_num, LED_WRAP);

    pwm_set_chan_level(led_slice_num, led_channel, 0);
    pwm_set_enabled(led_slice_num, true);

    // --------------------------
    // Movimentos iniciais do servo (180°, 90°, 0°), cada 5s
    // --------------------------
    // 180° => 2400us
    printf("Servo ~180 graus, LED max (2400us)\n");
    set_servo_pulse(SERVO_180_US);
    set_led_level(map_servo_pulse_to_led_level(SERVO_180_US)); // LED = 100% approx
    sleep_ms(5000);

    // 90° => 1470us
    printf("Servo ~90 graus, LED ~50%% (1470us)\n");
    set_servo_pulse(SERVO_90_US);
    set_led_level(map_servo_pulse_to_led_level(SERVO_90_US));
    sleep_ms(5000);

    // 0° => 500us
    printf("Servo ~0 graus, LED min (500us)\n");
    set_servo_pulse(SERVO_0_US);
    set_led_level(map_servo_pulse_to_led_level(SERVO_0_US));
    sleep_ms(5000);

    // --------------------------
    // Movimentação suave
    // --------------------------
    printf("Movimentação suave: servo e LED intensidade...\n");
    bool subindo = true;
    uint16_t pulse = SERVO_0_US;

    while (true) {
        set_servo_pulse(pulse);

        // LED intensidade proporcional ao pulso
        uint16_t led_level = map_servo_pulse_to_led_level(pulse);
        set_led_level(led_level);

        // Incrementos suaves
        sleep_ms(10);

        if (subindo) {
            pulse += 5;
            if (pulse >= SERVO_180_US) {
                pulse = SERVO_180_US;
                subindo = false;
            }
        } else {
            pulse -= 5;
            if (pulse <= SERVO_0_US) {
                pulse = SERVO_0_US;
                subindo = true;
            }
        }
    }

    return 0;
}
