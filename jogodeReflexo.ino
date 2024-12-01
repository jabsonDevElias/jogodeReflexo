#include <Wire.h>  
#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h>  

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

int pontos = 0;
int tempo = 1200;        // TEMPO PADRÃO INICIAL LV.1
int nivelMedio = 1000;   // TEMPO MÉDIO INICIAL LV.2
int nivelDificil = 800;  // TEMPO DIFÍCIL INICIAL LV.3

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
int mensagem = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


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


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao inicializar o OLED"));
    while (1); 
  }

  display.clearDisplay();  // LIMPAR O DISPLAY
  
}

void loop() {

  int arrayLeds[] = {ledVerde, ledAmarelo, ledVermelho, ledAzul, ledBranco};
  int arrayBotoes[] = {botaoVerde, botaoAmarelo, botaoVermelho, botaoAzul, botaoBranco};
  
  int tamanho = sizeof(arrayLeds) / sizeof(arrayLeds[0]); //TAMANHO DO ARRAY

  unsigned long currentMillis = millis();  // TEMPO ATUAL
  botaoIniciarPressionado = digitalRead(botaoDesligar);


  
  if(botaoIniciarPressionado == LOW){
    tempo = 4000; 
    iniciarJogo();
  }

  // NIVEL DE DIFICULDADE DE ACORDO COM A PONTUAÇÃO
  if (pontos > 7) {
    tempo = nivelMedio;
  }

  if (pontos > 15) {
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
      // Serial.println("LED aceso: " + String(arrayLeds[ledAtual]));

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

     display.clearDisplay();
     mostrarNaTela(String(pontos),3,15,6);
  }

  // APAGA LED AUTOMATICAMENTE SE NÃO FOR PRECIONADO
    
  if (ledLigado && (currentMillis - previousMillis >= tempo)) {
    digitalWrite(arrayLeds[ledAtual], LOW);
    ledLigado = false;
    Serial.println("Tempo esgotado! Fim de Jogokkk");
    display.clearDisplay();
    mostrarNaTela("TEMPO",45,15,2);
    mostrarNaTela("ESGOTADO!",19,43,2);
    delay(5000);
    display.clearDisplay();

    pontos = 0;
    jogoIniciado = false;
  }
    
  }else{
     Serial.println("INICIAR!");
     mostrarNaTela("INICIAR",19,15,2);
     mostrarNaTela("JOGO",36,43,2);


    
  }
}

void mostrarNaTela(String texto,int w,int h,int ts) {
  display.setTextSize(ts);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(w,h);
  display.print(texto); 
  display.display(); 
}

void iniciarJogo(){
  jogoIniciado = true;
}
