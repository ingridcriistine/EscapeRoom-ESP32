#include "fase3.h"
#include "hardware.h"

static bool concluida = false;
static bool perdida = false;

void iniciarFase3() {
  concluida = false;
  perdida = false;

  apagarLeds();
  sincronizarBotoes();

  Serial.println();
  Serial.println("========================");
  Serial.println("FASE 3 - Restaurar a energia");
  Serial.println("========================");

  Serial.println("Fase 3 ainda nao implementada.");
}

void executarFase3(bool apertou[]) {
  // Implementar fase 3 aqui.
}

bool fase3Concluida() {
  return concluida;
}

bool fase3Perdida() {
  return perdida;
}