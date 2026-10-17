#include "hardware.h"
#include "fase1.h"
#include "fase2.h"
#include "fase3.h"

enum Fase {
  AGUARDANDO_INICIO,
  FASE_1,
  FASE_2,
  FASE_3,
  FIM_JOGO
};

Fase faseAtual = AGUARDANDO_INICIO;

void setup() {
  Serial.begin(115200);

  configurarHardware();

  Serial.println();
  Serial.println("========================");
  Serial.println("      ESCAPE ROOM");
  Serial.println("========================");
  Serial.println("Pressione START para comecar.");
}

void loop() {
  bool apertou[4] = {false, false, false, false};
  lerBotoes(apertou);

  switch (faseAtual) {

    case AGUARDANDO_INICIO:
      if (apertou[3]) {
        iniciarPartida();
      }
      break;

    case FASE_1:
      executarFase1(apertou);

      if (fase1Concluida()) {
        iniciarFase2();
        faseAtual = FASE_2;
      }
      else if (fase1Perdida()) {
        encerrarComDerrota();
      }
      break;

    case FASE_2:
      executarFase2(apertou);

      if (fase2Concluida()) {
        iniciarFase3();
        faseAtual = FASE_3;
      }
      else if (fase2Perdida()) {
        encerrarComDerrota();
      }
      break;

    case FASE_3:
      executarFase3(apertou);

      if (fase3Concluida()) {
        encerrarComVitoria();
      }
      else if (fase3Perdida()) {
        encerrarComDerrota();
      }
      break;

    case FIM_JOGO:
      if (apertou[3]) {
        iniciarPartida();
      }
      break;
  }
}

void iniciarPartida() {
  apagarLeds();

  sincronizarBotoes();

  Serial.println();
  Serial.println("========================");
  Serial.println("      NOVA PARTIDA");
  Serial.println("========================");

  iniciarFase1();

  faseAtual = FASE_1;
}

void encerrarComDerrota() {
  somDerrota();
  sincronizarBotoes();

  faseAtual = FIM_JOGO;

  Serial.println();
  Serial.println("========================");
  Serial.println("      FIM DE JOGO");
  Serial.println("========================");
  Serial.println("Pressione START para reiniciar.");
}

void encerrarComVitoria() {
  somSucesso();
  sincronizarBotoes();

  faseAtual = FIM_JOGO;

  Serial.println();
  Serial.println("========================");
  Serial.println("        VITORIA!");
  Serial.println("========================");
  Serial.println("Voce escapou!");
  Serial.println("Pressione START para jogar novamente.");
}