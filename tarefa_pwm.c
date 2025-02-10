#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definição dos pinos
#define SERVO_PIN 22
#define LED_RGB_PIN 12   // LED RGB (BitDogLab), para observação

// Frequência de 50Hz => período de 20ms => 20000 microssegundos
// Exemplo de cálculo para wrap:
// Se usamos clock padrão ~125MHz, e queremos ~50Hz, definimos um wrap que facilite configurar os pulsos.
// Vamos definir um clock divisor e um wrap que permita medir pulsos em microssegundos.

int main() {
    stdio_init_all();

    // ---------------------------
    // Configuração do SERVO_PIN
    // ---------------------------
    // 1. Definir função do pino como PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);

    // 2. Obter o slice do PWM
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    // Observando uma possível abordagem:
    //   - Precisamos de ~20ms de período.
    //   - Se clock é ~125MHz (padrão), podemos definir um divisor que facilite medir em microssegundos.
    // Exemplo: Divisor de 125 => cada tick de PWM vale 1 microsegundo (125MHz / 125 = 1MHz).
    // Assim, se wrap = 20000, o período de PWM será 20000us = 20ms => 50Hz.

    // 3. Configurar wrap e divisor
    //    wrap = 20000 => contagem de 0 a 20000-1 ticks
    pwm_set_wrap(slice_num, 20000);
    pwm_set_clkdiv(slice_num, 125.0f);  // Divisor = 125 => 1 tick = 1us

    // 4. Habilitar o PWM, mas setar inicialmente nível 0
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(SERVO_PIN), 0);
    pwm_set_enabled(slice_num, true);

    // ---------------------------
    // Configuração do LED_RGB_PIN
    // ---------------------------
    gpio_init(LED_RGB_PIN);
    gpio_set_dir(LED_RGB_PIN, GPIO_OUT);
    gpio_put(LED_RGB_PIN, false);

    // ---------------------------
    // Ajuste do servo (passos 2, 3 e 4 da tarefa)
    // ---------------------------

    // 1) Frequência de 50Hz já definida => OK.

    // 2) Ciclo ativo = 2400us => aprox 180 graus.
    printf("Movendo para 180 graus (pulso ~2400us)\n");
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(SERVO_PIN), 2400);
    sleep_ms(5000);  // Aguarda 5s

    // 3) Ciclo ativo = 1470us => aprox 90 graus.
    printf("Movendo para 90 graus (pulso ~1470us)\n");
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(SERVO_PIN), 1470);
    sleep_ms(5000);  // Aguarda 5s

    // 4) Ciclo ativo = 500us => aprox 0 graus.
    printf("Movendo para 0 graus (pulso ~500us)\n");
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(SERVO_PIN), 500);
    sleep_ms(5000);  // Aguarda 5s

    // ---------------------------
    // 5) Rotina para movimentação periódica suave entre 0 e 180 graus
    // ---------------------------
    // A movimentação suave pode ser implementada incrementando ou decrementando
    // o pulso em ~5us a cada 10ms, até atingir 500us ou 2400us.
    // Esse loop roda indefinidamente.
    printf("Movimentação suave entre 0 e 180 graus...\n");

    while (true) {
        // Sobe de 500us até 2400us
        for (int pulse = 500; pulse <= 2400; pulse += 5) {
            pwm_set_chan_level(slice_num, pwm_gpio_to_channel(SERVO_PIN), pulse);
            sleep_ms(10);  // Atraso de 10ms para suavidade
        }

        // Desce de 2400us até 500us
        for (int pulse = 2400; pulse >= 500; pulse -= 5) {
            pwm_set_chan_level(slice_num, pwm_gpio_to_channel(SERVO_PIN), pulse);
            sleep_ms(10);  // Atraso de 10ms
        }
    }

    return 0;
}
