# Projeto: Controle de Servomotor por PWM

Este projeto demonstra o uso do módulo PWM do Raspberry Pi Pico para controlar o ângulo de um servomotor e observar o comportamento do LED integrado na placa BitDogLab (no GPIO 12). O servomotor é controlado pelo GPIO 22 a 50 Hz (20 ms de período), variando o pulso entre ~500 µs (0°) e ~2400 µs (180°). Adicionalmente, o LED no GPIO 12 pode ser configurado em modo PWM para variar sua intensidade em sincronia com o movimento do servo, ou simplesmente para análise de possíveis interferências quando o servo se movimenta.

## Funcionalidades Principais
1. **Controle do Servomotor (GPIO 22)**
   - Frequência aproximada de 50 Hz (período de 20 ms).
   - Três posições iniciais: 0°, 90° e 180°, cada uma mantida por 5 s.
   - Rotina de movimentação periódica suave (±5 µs a cada 10 ms) entre 0° e 180°.

2. **LED no GPIO 12 (BitDogLab)**
   - Observa-se o comportamento do LED enquanto o servo se movimenta.
   - É possível configurar também em PWM (por exemplo, 1 kHz) e relacionar a intensidade com o pulso do servo.

3. **Período do PWM**
   - Para o servo, período de 20 ms (50 Hz) com divisor e wrap adequados.
   - Para o LED, caso queira PWM independente, define-se outro slice com frequência diferente.

4. **Demonstrar Observações**
   - Em hardware real, podemos notar variações no LED quando o servo se movimenta (ruído elétrico, etc.).
   - No simulador Wokwi, isso pode não ser aparente.

## Organização do Código
- **tarefa_pwm.c**:
  - Configura o PWM para o servo no GPIO 22 (frequência 50 Hz) e, opcionalmente, um PWM independente no GPIO 12 para o LED.
  - Ajusta o servo em 180°, 90° e 0°, cada um por 5 s, e depois movimenta suavemente o ângulo.
  - Funções auxiliares para definir o pulso do servo e, se desejado, o duty cycle do LED.

## Como Clonar o Projeto
1. Abra o terminal e navegue até a pasta desejada:
   ```bash
   git clone https://github.com/usuario/tarefa_pwm.git
   cd tarefa_pwm
   ```
2. Abra o Visual Studio Code na pasta:
   ```bash
   code .
   ```

## Como Executar o Projeto usando a Extensão Raspberry Pi Pico Project no VSCode
1. **Instale a Extensão**: Procure por "Raspberry Pi Pico Project" no marketplace do VSCode.
2. **Configure o SDK e Toolchain**: Certifique-se de que o [Pico SDK](https://github.com/raspberrypi/pico-sdk) está instalado e configurado.
3. **Abra o Projeto**: A extensão deve detectar o `CMakeLists.txt` e criar a estrutura de build.
4. **Selecione o Projeto**: Na barra inferior, escolha o kit de compilação.
5. **Compile**: Use o menu "Pico" ou atalho de "Build" para gerar o arquivo `.uf2`.
6. **Gravar no Pico**: Conecte o Raspberry Pi Pico em modo bootsel e arraste o `.uf2` para a unidade.
7. **Teste**: Se usar um simulador (ex.: Wokwi), inclua o servo no GPIO 22 e o LED (com resistor) no GPIO 12. Em hardware real, observe o servo e o LED variando.

## Conexões de Hardware
- **Servomotor**:
  - Sinal (controle) -> GPIO 22
  - Alimentação (3V3 ou 5V, dependendo do servo) e GND
- **LED BitDogLab**:
  - Pino de controle -> GPIO 12
  - Resistor e alimentação adequados

## Observações Finais
- Ajuste o valor de pulso mínimo (500 µs) e máximo (2400 µs) conforme a faixa do seu servo.
- Se desejar ver mudança de intensidade no LED, configure PWM e ajuste seu duty cycle conforme o servo.
- Em hardware real, analise se há flicker ou variações no LED quando o servo se movimenta.

---
**Autor**: Iago Virgílio

**Licença**: [MIT License](LICENSE)

