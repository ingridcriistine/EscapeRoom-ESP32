#include "fase1.h"
#include "hardware.h"

static Cor senha[3] = {AMARELO, VERDE, VERMELHO};
static Cor resposta[3];

static int quantidade = 0;
static int tentativas = 0;

static bool concluida = false;
static bool perdida = false;

static void adicionarCor(Cor cor);
static void conferirSenha();

void iniciarFase1() {
  quantidade = 0;
  tentativas = 0;
  concluida = false;
  perdida = false;

  for (int i = 0; i < 3; i++) {
    resposta[i] = VERDE;
  }

  apagarLeds();
  sincronizarBotoes();

  Serial.println();
  Serial.println("========================");
  Serial.println("FASE 1 - Descobrir a senha");
  Serial.println("========================");
  Serial.println("Informe tres cores e pressione START.");
}

void executarFase1(bool apertou[]) {
  if (apertou[3]) {
    if (quantidade == 3) {
      conferirSenha();
    } else {
      Serial.println(
        "Informe tres cores antes de confirmar."
      );
    }

    return;
  }

  if (quantidade >= 3) {
    return;
  }

  if (apertou[0]) {
    adicionarCor(VERDE);
  }

  if (apertou[1]) {
    adicionarCor(AMARELO);
  }

  if (apertou[2]) {
    adicionarCor(VERMELHO);
  }
}

void adicionarCor(Cor cor) {
  if (quantidade >= 3) {
    return;
  }

  resposta[quantidade] = cor;
  quantidade++;

  Serial.print("Cor registrada: ");

  if (cor == VERDE) {
    Serial.println("Verde");
  }
  else if (cor == AMARELO) {
    Serial.println("Amarelo");
  }
  else if (cor == VERMELHO) {
    Serial.println("Vermelho");
  }

  Serial.print("Cores informadas: ");
  Serial.println(quantidade);
}

void conferirSenha() {
  tentativas++;

  apagarLeds();

  for (int i = 0; i < 3; i++) {
    if (resposta[i] == senha[i]) {
      int pino = (senha[i] == VERDE)   ? LED_VERDE
              : (senha[i] == AMARELO) ? LED_AMARELO
                                      : LED_VERMELHO;
      digitalWrite(pino, HIGH);
    }
  }

  bool acertou =
    resposta[0] == senha[0] &&
    resposta[1] == senha[1] &&
    resposta[2] == senha[2];

  Serial.println();
  Serial.print("Tentativa ");
  Serial.print(tentativas);
  Serial.println(" de 5.");

  if (acertou) {
    Serial.println("Senha correta!");
    Serial.println("Fase 1 concluida.");

    somSucesso();
    sincronizarBotoes();
    concluida = true;

    return;
  }

  if (tentativas >= 5) {
    Serial.println("Senha incorreta.");
    Serial.println("Tentativas esgotadas.");
    perdida = true;

    return;
  }

  quantidade = 0;

  int restantes = 5 - tentativas;

  Serial.println("Senha incorreta.");
  Serial.print("Tentativas restantes: ");
  Serial.println(restantes);

  somTentativas(restantes);
  sincronizarBotoes();

  Serial.println(
    "Informe uma nova sequencia completa."
  );
}

bool fase1Concluida() {
  return concluida;
}

bool fase1Perdida() {
  return perdida;
}