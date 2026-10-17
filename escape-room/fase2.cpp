#include "fase2.h"
#include "hardware.h"

static const unsigned long INTERVALO_LED = 300;

static Cor sequencia[4] = {VERDE, AMARELO, VERMELHO, AMARELO};

static int posicao = 0;
static int tentativas = 0;
static int acertos = 0;

static unsigned long ultimoMovimento = 0;

static bool concluida = false;
static bool perdida = false;

static void atualizarMovimento();
static void registrarTentativa();

void iniciarFase2() {
  tentativas = 0;
  acertos = 0;
  concluida = false;
  perdida = false;
  posicao = 0;

  apagarLeds();
  acenderLed(sequencia[posicao]);

  ultimoMovimento = millis();

  sincronizarBotoes();

  Serial.println();
  Serial.println("========================");
  Serial.println("FASE 2 - Interromper o sinal");
  Serial.println("========================");

  Serial.println(
    "Pressione B2 quando o LED amarelo estiver aceso."
  );

  Serial.println(
    "Consiga 3 acertos em ate 5 tentativas."
  );
}

void executarFase2(bool apertou[]) {
  atualizarMovimento();

  if (apertou[1]) {
    registrarTentativa();
  }
}

void atualizarMovimento() {
  if (millis() - ultimoMovimento >= INTERVALO_LED) {
    ultimoMovimento = millis();
    posicao++;

    if (posicao >= 4) {
      posicao = 0;
    }

    acenderLed(sequencia[posicao]);
  }
}

void registrarTentativa() {
  tentativas++;

  bool acertou =
    sequencia[posicao] == AMARELO;

  Serial.println();

  Serial.print("Tentativa ");
  Serial.print(tentativas);
  Serial.println(" de 5.");

  if (acertou) {
    acertos++;
    Serial.println("ACERTO!");

    Serial.print("Acertos: ");
    Serial.print(acertos);
    Serial.println(" de 3.");
  }

  else {
    Serial.println("ERRO!");
  }

  if (acertos >= 3) {

    Serial.println();
    Serial.println("Fase 2 concluida!");

    somSucesso();
    sincronizarBotoes();
    concluida = true;

    return;
  }

  if (tentativas >= 5) {
    Serial.println();
    Serial.println("Tentativas esgotadas.");
    perdida = true;

    return;
  }

  int restantes = 5 - tentativas;

  Serial.print("Tentativas restantes: ");
  Serial.println(restantes);

  somTentativas(restantes);
  sincronizarBotoes();

  posicao = 0;
  acenderLed(sequencia[posicao]);

  ultimoMovimento = millis();
}

bool fase2Concluida() {
  return concluida;
}

bool fase2Perdida() {
  return perdida;
}