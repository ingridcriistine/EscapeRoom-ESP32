#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>

extern const int LED_VERMELHO;
extern const int LED_AMARELO;
extern const int LED_VERDE;

extern const int BOTAO_B1;
extern const int BOTAO_B2;
extern const int BOTAO_B3;
extern const int BOTAO_START;
extern const int BUZZ;

enum Cor {
  VERDE = 1,
  AMARELO = 2,
  VERMELHO = 3
};

void configurarHardware();

void lerBotoes(bool apertou[]);
void sincronizarBotoes();

void apagarLeds();
void acenderLed(Cor cor);

void somTentativas(int restantes);
void somSucesso();
void somDerrota();

#endif