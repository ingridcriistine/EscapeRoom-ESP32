#include "hardware.h"

const int LED_VERMELHO = 19;
const int LED_AMARELO = 18;
const int LED_VERDE = 5;

const int LEDs[3] = {
  LED_VERDE,
  LED_AMARELO,
  LED_VERMELHO
};

const int BOTAO_B1 = 17;
const int BOTAO_B2 = 16;
const int BOTAO_B3 = 4;
const int BOTAO_START = 2;
const int BUZZ = 0;

const int botoes[4] = {
  BOTAO_B1,
  BOTAO_B2,
  BOTAO_B3,
  BOTAO_START
};

int estado[4];
int leituraAnterior[4];

unsigned long tempoMudanca[4] = {
  0, 0, 0, 0
};

const unsigned long DEBOUNCE = 50;

void configurarHardware() {
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(BUZZ, OUTPUT);

  apagarLeds();

  for (int i = 0; i < 4; i++) {
    pinMode(botoes[i], INPUT_PULLUP);
    estado[i] = digitalRead(botoes[i]);
    leituraAnterior[i] = estado[i];
  }
}

void lerBotoes(bool apertou[]) {

  for (int i = 0; i < 4; i++) {
    int leitura = digitalRead(botoes[i]);

    if (leitura != leituraAnterior[i]) {
      tempoMudanca[i] = millis();
      leituraAnterior[i] = leitura;
    }

    if (millis() - tempoMudanca[i] >= DEBOUNCE) {
      if (leitura != estado[i]) {
        estado[i] = leitura;

        if (estado[i] == LOW) {
          apertou[i] = true;
        }
      }
    }
  }
}

void sincronizarBotoes() {
  for (int i = 0; i < 4; i++) {
    estado[i] = digitalRead(botoes[i]);
    leituraAnterior[i] = estado[i];
    tempoMudanca[i] = millis();
  }
}

void apagarLeds() {
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);
}

void acenderLed(Cor cor) {
  apagarLeds();

  if (cor == VERDE) {
    digitalWrite(LED_VERDE, HIGH);
  }
  else if (cor == AMARELO) {
    digitalWrite(LED_AMARELO, HIGH);
  }
  else if (cor == VERMELHO) {
    digitalWrite(LED_VERMELHO, HIGH);
  }
}

void somTentativas(int restantes) {

  int intervalo = restantes * 100;

  tone(BUZZ, 2000);
  delay(100);
  noTone(BUZZ);

  delay(intervalo);

  tone(BUZZ, 2000);
  delay(100);
  noTone(BUZZ);
}

void somSucesso() {

  for (int i = 0; i < 3; i++) {

    tone(BUZZ, 2500);

    delay(100);

    noTone(BUZZ);

    delay(70);
  }
}

void somDerrota() {

  tone(BUZZ, 400);
  delay(350);
  noTone(BUZZ);

  delay(100);

  tone(BUZZ, 300);
  delay(500);
  noTone(BUZZ);

  for (int i = 0; i < 3; i++) {

    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_AMARELO, HIGH);
    digitalWrite(LED_VERMELHO, HIGH);

    delay(200);
    apagarLeds();
    delay(200);
  }
}