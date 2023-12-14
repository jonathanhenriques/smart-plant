/*===========================
  Smart Plant
  Autor: Jonathan Henrique
  Data: 3/11/2021

  ============================*/

// chamada a biblioteca do lcd:
#include <LiquidCrystal.h>

// config de portas do LCD
/*
  VSS pin gnd  | thinkercad seria a porta GND do LCD de dois fios
  VDD pin + | thinkercad seria a porta VCC  do LCD de dois fios
  VO pin ligado a porta 7 para controle backlight | thinkerdcad seria a porta VO do LCD de dois fios
  RS pin 8 | thinkercad seria a porta RS no pin 12 do LCD de dois fios
  RW pin gnd | thinkercas seria a porta RW do pin RW ligado no GND do LCD de dois fios
  Enable pin 9 | thinkercad seria a porta E do pin -11 DO LCD de dois fios
  D0 pin xdesligado | thinkercad não ligado a nada no LCD de dois fios
  D1 pin xdesligado | thinkercad não ligado a nada no LCD de dois fios
  D2 pin xdesligado | thinkercad não ligado a nada no LCD de dois fios
  D3 pin xdesligado | thinkercad não ligado a nada no LCD de dois fios
  D4 pin 10 | thinkercas seria a porta D4 no pin -5 do LCD de dois fios
  D5 pin 11 | thinkercas seria a porta D5 no pin 4 do LCD de dois fios
  D6 pin 12 | thinkercas seria a porta D6 no pin -3 do LCD de dois fios
  D7 pin 13 | thinkercas seria a porta D7 no pin 2 do LCD de dois fios
  A resistor 470 ligado no pin + | thinkercas seria a porta LED no pin + um RESISTOR de 220 U do LCD de dois fios
  K pin gnd | thinkercas seria a outra porta LED, a última, no pin -, do LCD de dois fios
*/

// Ligacoes das portas do lcd
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define LigaBacklightLcd 7

// chamada a biblioteca do teclado:
#include <Keypad.h>

//variaveis teclado
// Teclado 4x3 4 linhas e 3 colunas
#define linha_1  18
#define linha_2  6
#define linha_3  5
#define linha_4  4
#define coluna_1  2
#define coluna_2  19
#define coluna_3  3

// Vetores do teclado
byte pinosLinhas[] = {linha_1, linha_2, linha_3, linha_4};
byte pinosColunas[] = {coluna_1, coluna_2, coluna_3};
char teclas[4][3] = {{'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, 4, 3);
char teclaPressionada;

//variaveis dos leds e sensores
#define pinSensorUmidadeA A0
#define ledAviso A2
#define botaoEmocao A3
//int botaoTecnico = 1;
//int ledBotao = 3;

//variaveis para Verificar estado da planta
//String nomePlantaAtual = "";
String nomePlanta[] = {"Todas",
                       "Cecilia",
                       "Catharina",
                       "Azaleia",
                       "Bento",
                       "Gertrudes",
                       "Anastasia",
                       "Carlos",
                       "Oracio",
                       "Vazio"
                      };
            

int estadoSeco = 900, estadoQuaseSeco = 800, estado_quaseUmido = 700, estado_umido = 600;

// Vetor de rega das plantas
String dataDeRegaDasPlantas[10]; // Alterar para tamanho dinâmico
int tamanhoDataDeRegaDasPlantas = sizeof(dataDeRegaDasPlantas) / sizeof(dataDeRegaDasPlantas[0]); // Retorna o tamanho do array


//variaveis de luminosidade
#define sensorDeLuz 15//A1
int luzLida = 0, luzPorcentagem = 0;

//variaveis de umidade
int umidadeLida = 0, umidadePorcentagem = 0;

//controle de tempo
unsigned long tempo = 0;

// Characters
//byte boca[] = {B00000, B00000, B11111, B11111, B11111, B00000, B00000, B00000};
//byte olhos[] = {B00000, B00000, B00000, B00000, B00000, B11111, B11111, B11111};
byte temperaturaTermometro[] = {B01110, B01010, B01010, B01010, B01110, B11111, B11111, B01110};
byte luminosidadeSol[] = {B00100, B10101, B01110, B11111, B11111, B01110, B10101, B00100};
byte umidadeGota[] = {B00000, B00100, B01110, B01110, B11111, B11111, B11111, B01110};
byte feliz[] = {B10001, B11011, B10001, B00000, B00000, B11111, B11111, B01110};
byte ok[] = {B00000, B11011, B11011, B00000, B00000, B01110, B01110, B00000};
byte triste[] = {B00000, B01010, B11011, B01010, B00000, B00000, B01110, B10001};
byte coracao[] = {B00000, B01010, B11111, B11111, B01110, B00100, B00000, B00000};



// Variáveis para atribuir a posicao salva dos characters na memória do lcd
//byte bocaChart = 1;
//byte olhosChart = 2;
byte temperaturaChart = 3;
byte luminosidadeChart = 4;
byte umidadeChart = 5;
byte felizChart = 6;
byte okChart = 7;
byte tristeChart = 8;
byte coracaoChart = 9;



// Funções
void sensores(int numPlanta, String nomeSensor, byte chart, int leitura);
void foiRegadaEm(int numPlanta);
void atualizaDataPlantaRegada(String nomePlantaAtual);
void ligaLedSePlantaSeca();
void plantaSeSentindoEmoji(int numPlanta);
void SaudeDaPlanta();


void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  //lcd.createChar(1, boca);
  //lcd.createChar(2, olhos);
  lcd.createChar(3, temperaturaTermometro);
  lcd.createChar(4, luminosidadeSol);
  lcd.createChar(5, umidadeGota);
  lcd.createChar(6, feliz);
  lcd.createChar(7, ok);
  lcd.createChar(8, triste);
  lcd.createChar(9, coracao );

  pinMode(LigaBacklightLcd, OUTPUT);
  pinMode(ledAviso, OUTPUT);


  pinMode(botaoEmocao, INPUT_PULLUP);          // Botoes com INPUT_PULLUP para usarem o resistor interno do arduino
  //pinMode(botaoTecnico, INPUT_PULLUP);

  digitalWrite(ledAviso, LOW);

}

void loop() {

  umidadeLida = analogRead(pinSensorUmidadeA);
  teclaPressionada = teclado.getKey();
  
  int numPlanta = teclaPressionada - '0';
  lcd.home();
  lcd.clear();
  lcd.display();

  if (teclaPressionada == '0') {
    
    digitalWrite(LigaBacklightLcd, HIGH);
    ligaLedSePlantaSeca();
    SaudeDaPlanta();
    delay(2000);

  } else if (teclaPressionada == '*') {
    
    digitalWrite(LigaBacklightLcd, HIGH);
    lcd.print("Registro de Rega");
    delay(1500);
    lcd.clear();
    lcd.home();
    lcd.print("Insira numero");
    lcd.setCursor(0, 1);
    lcd.print("da Planta: ");
    char tecla;

    while (true) {
      tecla = teclado.getKey();

      if (tecla == '#') {
        lcd.println("Cancelado!");
        delay(500);
        break;
      }else if(tecla == '*'){
        numPlanta = 0;
        break;
      } else if (tecla) {
        int charParaInt = tecla - '0';
        numPlanta = charParaInt ;
        break;
      }
    }
    atualizaDataPlantaRegada(numPlanta);

  } else if (teclaPressionada != '0' && teclaPressionada != '*' && teclaPressionada != '#' && teclaPressionada) {
   
    digitalWrite(LigaBacklightLcd, HIGH);
    executa(numPlanta);
  }

  if (digitalRead(botaoEmocao) == LOW) {
    digitalWrite(LigaBacklightLcd, HIGH);
    ligaLedSePlantaSeca();
    SaudeDaPlanta();
    delay(2000);
  }

  digitalWrite(ledAviso, LOW);
  digitalWrite(LigaBacklightLcd, LOW);
  lcd.noDisplay();

}


// Métodos
void executa(int numPlanta) {

  luzLida = analogRead(sensorDeLuz);
  luzPorcentagem = map(luzLida, 1023, 0, 0, 100); //
  umidadeLida = analogRead(pinSensorUmidadeA);
  umidadePorcentagem = map(umidadeLida, 1023, 0, 0, 100);

  lcd.home();
  lcd.clear();
  lcd.display();
  digitalWrite(LigaBacklightLcd, HIGH);
  lcd.print(nomePlanta[numPlanta]);

  sensores(numPlanta, "Umidade", umidadeChart, umidadePorcentagem);
  sensores(numPlanta, "Calor", temperaturaChart, umidadeLida);
  sensores(numPlanta, "Luz", luminosidadeChart, luzPorcentagem);

  lcd.clear();
  foiRegadaEm(numPlanta);
  lcd.clear();
  plantaSeSentindoEmoji(numPlanta);                   //metodo que retorna um emoji emocional da planta
}



void sensores(int numPlanta, String nomeSensor, byte chart, int leitura) {


  lcd.display();
  lcd.setCursor(0, 0);
  lcd.print("Dados " + nomePlanta[numPlanta] + ":");
  delay(2000);

  lcd.setCursor(0, 1);
  lcd.write(chart);
  lcd.print(" " + nomeSensor + ": ");
  lcd.print(leitura);
  lcd.println("%          ");
  delay(2000);
}


void foiRegadaEm(int numPlanta) {
  lcd.home();
  lcd.print("Ultima Rega: ");
  lcd.setCursor(0, 1);
  if (dataDeRegaDasPlantas[numPlanta] == "") {
    lcd.print("Sem Dados!");
    delay(2000);
  } else {
    lcd.print(dataDeRegaDasPlantas[numPlanta]);
    delay(2000);
  }

}


void atualizaDataPlantaRegada(int numPlanta) {

  lcd.home();
  lcd.clear();
  lcd.print("Planta atual: ");
  lcd.setCursor(0, 1);
  lcd.print(nomePlanta[numPlanta]);
  delay(2000);
  lcd.clear();
  lcd.print("Insira data");
  lcd.setCursor(0, 1);
  lcd.print("dd/mm: ");
  String dataDaRega;



  while (true) {
    char digito = teclado.getKey();
    if (digito) {
      dataDaRega += digito;
      lcd.setCursor(0, 1);
      lcd.print(dataDaRega);
    }

    /*arrumar
    if (dataDaRega.length() == 2 && dataDaRega.length() < 3) {     //Adiciona uma barra para dividir dia e mes no lcd
      lcd.print("/");
    }
    */

    if (dataDaRega.length() >= 4) {
      String dia;
      String mes;
      dia = dataDaRega.substring(0, 2);
      mes = dataDaRega.substring(2);
      dataDaRega = dia + "/" + mes;

      if (nomePlanta[numPlanta] == "Todas") {
        for (byte i = 0; i < tamanhoDataDeRegaDasPlantas; i++) {
          dataDeRegaDasPlantas[i] = dataDaRega;
          Serial.print(nomePlanta[numPlanta]+ i);
        }
        lcd.clear();
        //Serial.print(nomePlanta[numPlanta]);
        lcd.print(nomePlanta[numPlanta] + " as Plantas");
        lcd.setCursor(0, 1);
        lcd.print("Regadas em:" + dataDeRegaDasPlantas[0]); //Retornar qualquer posição desse Array é válido
        delay(4000);
        break;
      } else {

        dataDeRegaDasPlantas[numPlanta] = dataDaRega;
        lcd.clear();
        lcd.print(nomePlanta[numPlanta]);
        lcd.setCursor(0, 1);
        lcd.print("Regada em: " + dataDeRegaDasPlantas[numPlanta]);
        delay(4000);
        break;

      }
      if (digito == '#') {
        lcd.clear();
        lcd.print("Cancelado!");
        delay(500);
        break;
      }

    }
    if (digito == '#') {
      lcd.clear();
      lcd.print("Cancelado!");
      delay(500);
      break;
    }

  }

}



void ligaLedSePlantaSeca() {
  if (analogRead(pinSensorUmidadeA) > estadoSeco) {
    digitalWrite(ledAviso, HIGH);
  }
}


void plantaSeSentindoEmoji(int numPlanta) {
  lcd.home();
  lcd.print(nomePlanta[numPlanta] + " se");
  lcd.setCursor(0, 1);
  lcd.print("sentindo: ");
  if (analogRead(pinSensorUmidadeA) >= estadoSeco) {
    lcd.write(8);
  } else if (analogRead(pinSensorUmidadeA) > estado_quaseUmido && analogRead(pinSensorUmidadeA) < estadoQuaseSeco) {
    lcd.write(7);
  } else {
    lcd.write(6);
  }
  delay(3000);
}


void SaudeDaPlanta() {
  lcd.clear();
  lcd.home();
  lcd.display();
  lcd.print("Saude da");
  lcd.setCursor(0, 1);
  lcd.print("Planta: ");
  delay(1500);

  if (umidadeLida > estadoSeco) {
    lcd.clear();
    lcd.home();
    lcd.print("Planta: ");
    delay(500);
    lcd.write(coracaoChart);
    delay(2000);
    //lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Estado: ");
    delay(500);
    lcd.print("Triste");
    delay(2000);
  } else if (umidadeLida > estado_quaseUmido && umidadeLida < estadoQuaseSeco) {
    lcd.write(coracaoChart);
    lcd.write(coracaoChart);
    delay(1500);
    lcd.clear();
    lcd.home();
    lcd.print("Estado: ");
    lcd.print("Normal");
    delay(2000);
  } else {
    lcd.write(coracaoChart);
    lcd.write(coracaoChart);
    lcd.write(coracaoChart);
    delay(1500);
    lcd.clear();
    lcd.home();
    lcd.print("Estado: ");
    lcd.print("Feliz");
    delay(2000);
  }

}
