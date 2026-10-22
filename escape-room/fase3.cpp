#include "fase3.h"
#include "hardware.h"

static bool concluida = false;
static bool perdida = false;

static int tentativasRestantes;

const int INDICE_VERDE = 0;
const int INDICE_AMARELO = 1;
const int INDICE_VERMELHO = 2;

static bool estadoLED[3] = {LOW, LOW, LOW};

static bool flagEstadoAlterado = false;

void acenderLedsEstadoAtual() {
  for (int i=0; i<3; i++) {
    digitalWrite(
      LEDs[i],
      estadoLED[i]
    );
  }
}

static void sortearLEDs() {
  static const bool estadosIniciais[][3] = {
    {LOW,  LOW,  LOW },
    {LOW,  LOW,  HIGH},
    {LOW,  HIGH, LOW },
    {LOW,  HIGH, HIGH},
    {HIGH, LOW,  LOW },
    {HIGH, LOW,  HIGH},
    {HIGH, HIGH, LOW }
  };

  const int total = sizeof(estadosIniciais) / sizeof(estadosIniciais[0]);
  int indice = millis() % total;

  for (int i = 0; i < 3; i++) {
    estadoLED[i] = estadosIniciais[indice][i];
  }

  acenderLedsEstadoAtual();
}

bool verificarTodosAcesos() {
  bool todosAcesos = true;
  for (int i=0; i<3; i++){
    if (!estadoLED[i]) {
      todosAcesos = false;
    }
  }
  return todosAcesos;
}

void inverterEstadoLedByIndice(int indiceLED) {
  Serial.print("inverte | indice ");
  Serial.print(indiceLED);
  Serial.print(" | estado ");
  Serial.println(estadoLED[indiceLED]);
  if (estadoLED[indiceLED]) {
    estadoLED[indiceLED] = LOW;
  } else { 
    estadoLED[indiceLED] = HIGH;
  }
  Serial.print(" -> ");
  Serial.println(estadoLED[indiceLED]);
  flagEstadoAlterado = true;
}

void iniciarFase3() {
  concluida = false;
  perdida = false;

  tentativasRestantes = 5;

  apagarLeds();
  sincronizarBotoes();

  sortearLEDs();


  Serial.println();
  Serial.println("========================");
  Serial.println("FASE 3 - Restaurar a energia");
  Serial.println("========================");

  Serial.println(" B1 -> Altera o estado atual dos LEDs VERDE e AMARELO");
  Serial.println(" B2 -> Altera o estado atual dos LEDs AMARELO e VERMELHO");
  Serial.println(" B3 -> Altera o estado atual do LED AMARELO");
  Serial.println("========================");
}

void executarFase3(bool apertou[]) {
  flagEstadoAlterado = false;
  apagarLeds();
  acenderLedsEstadoAtual();

  if (apertou[0]){
    inverterEstadoLedByIndice(INDICE_VERDE);
    inverterEstadoLedByIndice(INDICE_AMARELO);
    Serial.println("Apertou [0] B1 - Verde e Amarelo");
    acenderLedsEstadoAtual();
  }
  
  if (apertou[1]) {
    inverterEstadoLedByIndice(INDICE_VERMELHO);
    inverterEstadoLedByIndice(INDICE_AMARELO);
    Serial.println("Apertou [1] B2 - Vermelho e Amarelo");
    acenderLedsEstadoAtual();
  }
  
  if (apertou[2]) {
    inverterEstadoLedByIndice(INDICE_AMARELO);
    Serial.println("Apertou [2] B3 - Amarelo");
    acenderLedsEstadoAtual();
  }
  
  if (apertou[3]) {
    Serial.println("Botão START não possui funcionalidade nesta fase!");
    acenderLedsEstadoAtual();
  }

  if (verificarTodosAcesos()) {
    somSucesso();
    sincronizarBotoes();
    concluida = true;
    
    return;

  } else if (flagEstadoAlterado) {
    tentativasRestantes--;
    Serial.print("Movimentos restantes: ");
    Serial.println(tentativasRestantes);
  }

  if (tentativasRestantes < 1) {
    Serial.println("Você está sem tentativas restantes!!");
    perdida = true;

    return;
  }

  if (flagEstadoAlterado) {
    somTentativas(tentativasRestantes);
  }
}

bool fase3Concluida() {
  return concluida;
}

bool fase3Perdida() {
  return perdida;
}