int pontos = 0;
int tempo = 5000;        // TEMPO PADRÃO INICIAL LV.1
int nivelMedio = 2500;   // TEMPO MÉDIO INICIAL LV.2
int nivelDificil = 1000;  // TEMPO DIFÍCIL INICIAL LV.3

int botaoPrecionado = 0;
int botaoAtual = -1;     // VARIAVEL PARA ARMAZENAR BOTÃO A SER PRESSIONADO
unsigned long previousMillis = 0;
int ledAtual = -1;       // ARMAZENA LED ATUAL
bool ledLigado = false;
bool jogoIniciado = false;
int botaoIniciarPressionado = 0;

// LEDS
int ledVerde = 13;
int ledAmarelo = 12;
int ledVermelho = 11;
int ledAzul = 10;
int ledBranco = 9;

// BOTÕES
int botaoVerde = 7;
int botaoAmarelo = 6;
int botaoVermelho = 5;
int botaoAzul = 4;
int botaoBranco = 3;
int botaoDesligar = 2;

void setup() {
  Serial.begin(9600);

  // PINOS LEDS
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  pinMode(ledBranco, OUTPUT);

  // PINOS DOS BOTÕES
  pinMode(botaoVerde, INPUT_PULLUP);
  pinMode(botaoAmarelo, INPUT_PULLUP);
  pinMode(botaoVermelho, INPUT_PULLUP);
  pinMode(botaoAzul, INPUT_PULLUP);
  pinMode(botaoBranco, INPUT_PULLUP);
  
  pinMode(botaoDesligar, INPUT_PULLUP); // BOTÃO DESLIGAR
}

void loop() {

  int arrayLeds[] = {ledVerde, ledAmarelo, ledVermelho, ledAzul, ledBranco};
  int arrayBotoes[] = {botaoVerde, botaoAmarelo, botaoVermelho, botaoAzul, botaoBranco};
  
  int tamanho = sizeof(arrayLeds) / sizeof(arrayLeds[0]); //TAMANHO DO ARRAY

  unsigned long currentMillis = millis();  // TEMPO ATUAL
  botaoIniciarPressionado = digitalRead(botaoDesligar);
  
  if(botaoIniciarPressionado == LOW){
    iniciarJogo();
  }

  // NIVEL DE DIFICULDADE DE ACORDO COM A PONTUAÇÃO
  if (pontos > 10) {
    tempo = nivelMedio;
  }

  if (pontos > 25) {
    tempo = nivelDificil;
  }

  if(jogoIniciado){
    
  //ESCOLHER LED ALEATORIO
  if (!ledLigado) {
    if (currentMillis - previousMillis >= tempo) {
      previousMillis = currentMillis;      // ATUALIZAR O TEMPO ANTERIOR

      ledAtual = rand() % tamanho;         // LED ALEATORIO
      digitalWrite(arrayLeds[ledAtual], HIGH);
      ledLigado = true;
      Serial.println("LED aceso: " + String(arrayLeds[ledAtual]));

      botaoAtual = arrayBotoes[ledAtual];  // Define o botão correspondente
    }
  }

  // VERIFICAR SE O BOTÃO CERTO FOI PRECIONADO
  botaoPrecionado = digitalRead(botaoAtual);
  
  if (ledLigado && botaoPrecionado == LOW) { //VERIFICA SE O BOTÃO REFERENTE AO LED FOI PRECIONADO
    digitalWrite(arrayLeds[ledAtual], LOW);
    ledLigado = false;
    pontos++;                                
    Serial.println("Acertou! Pontos: " + String(pontos));
  }

  // APAGA LED AUTOMATICAMENTE SE NÃO FOR PRECIONADO
    
  if (ledLigado && (currentMillis - previousMillis >= tempo)) {
    digitalWrite(arrayLeds[ledAtual], LOW);
    ledLigado = false;
    Serial.println("Tempo esgotado! Fim de Jogokkk");
    pontos = 0;
    jogoIniciado = false;
  }
    
  }else{
     Serial.println("VOCÊ PERDEU!");
  }
}

void iniciarJogo(){
  jogoIniciado = true;
}
