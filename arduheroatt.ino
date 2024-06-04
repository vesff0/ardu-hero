//Define os pinos dos LEDs, botões e LED RGB.
const
int lowestPin = 3;

const
int highestPin = 8;

int buttonPin = 2;

int r = 13;
int g = 12;
int b = 11;

//Define variáveis para lógica do jogo.
volatile int buttonState;
volatile int ledState;
volatile bool acerto;
volatile bool verificador = false;
volatile bool pointFlag = false;
volatile bool gameBegin = false;
int pontos = 0;
int valorDelay;

//Interrupt que será chamado sempre que o botão for pressionado;
void buttonInterrupt() {

  //Muda o estado de gameBegin para iniciar o jogo
  if (!gameBegin) {
    gameBegin = true;
  }

  //O comportamento do interrupt fazia com que essa parte do código rodasse várias vezes
  //a cada pressionar do botão, por isso criei a variável verificador.
  //verificador é uma variável que serve para confirmar o acionamento do botão.
  //se o botão já estiver pressionado, ele não vai rodar essa parte
  //isso evita o código de rodar a mesma parte várias vezes
  if (verificador == false) {
    buttonState = digitalRead(buttonPin);
    ledState = digitalRead(highestPin);

    //Verifica se o botão foi pressionado quando o último led ("highestPin") estava aceso.
    //Caso sim, conta o acionamento como um acerto.
    if (ledState == HIGH && buttonState == LOW) {
      digitalWrite(g, HIGH);
      delay(500);
      digitalWrite(r, LOW);
      acerto = true;
      verificador = true;

      //pointFlag é uma flag para contabilizar os pontos dentro do loop
      pointFlag = true;
    }

    //Caso o usuário erre.
    else if (ledState == LOW && buttonState == LOW) {
      digitalWrite(r, HIGH);
      digitalWrite(g, LOW);
      acerto = false;
      verificador = true;
    }

    else {
      digitalWrite(r, LOW);
      digitalWrite(g, LOW);
      verificador = true;
    }
  }
  else {
    digitalWrite(r, LOW);
    digitalWrite(g, LOW);
  }
}

void setup() {

  //define todos os pins como OUTPUT
  for (int thisPin = lowestPin; thisPin <= highestPin; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }

  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);

  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, CHANGE);

}

void loop() {
  if (gameBegin) {

    acerto = true;

    //valorDelay define a velocidade do jogo
    valorDelay = 500;
    delay(500); 
    digitalWrite(lowestPin, HIGH);
    delay(500);
    digitalWrite(lowestPin, LOW);
    delay(500); 
    digitalWrite(lowestPin, HIGH);
    delay(500);
    digitalWrite(lowestPin, LOW);
    delay(500); 
    digitalWrite(lowestPin, HIGH);
    delay(500);
    digitalWrite(lowestPin, LOW);
    delay(500);

    while (acerto == true) {

      //Esse "for" serve para acionar um led por vez
      //e verificar o estado do jogo a cada led acionado
      for (int thisPin = lowestPin; thisPin <= highestPin; thisPin++) {
        verificador = false;

        if (thisPin == lowestPin) {
          digitalWrite(thisPin, LOW);
        }

        digitalWrite(thisPin, HIGH);

        if (thisPin == highestPin) {
          digitalWrite(thisPin-1, LOW);
          delay(valorDelay);
          digitalWrite(thisPin, LOW);

          //Vai deixando o jogo mais rápido a cada loop
          if (valorDelay >= 500) {
            valorDelay -= 50;
          }
          else if (valorDelay < 500 && valorDelay >= 200) {
            valorDelay -= 10;
          }
          else if (valorDelay < 200 && valorDelay >= 60) {
            valorDelay -= 2;
          }
          else if (valorDelay <= 60) {
            valorDelay = 58;
          }

          //Se a pontuação não mudou quer dizer que o usuário não acertou.
          if (!pointFlag) {
            acerto = false;
          }
        }
        else if (thisPin != 3) {
          digitalWrite(thisPin-1, LOW);
        }

        delay(valorDelay);

        if (pointFlag) {
          pontos++;
          pointFlag = false;
        }

        if (!acerto) {
          //finaliza o jogo em caso de erro
          digitalWrite(r, HIGH);
          digitalWrite(thisPin, HIGH);
          delay(500);
          digitalWrite(r, LOW);
          delay(500);
          digitalWrite(r, HIGH);
          delay(500);
          digitalWrite(r, LOW);
          delay(500);
          digitalWrite(r, HIGH);
          delay(500);
          digitalWrite(r, LOW);
          delay(500);
          digitalWrite(r, HIGH);
          delay(500);
          digitalWrite(r, LOW);
          delay(500);
          digitalWrite(r, HIGH);
          delay(500);
          digitalWrite(r, LOW);
          digitalWrite(thisPin, LOW);
          Serial.print("Pontos: ");
          Serial.println(pontos);
          pontos = 0;
          gameBegin = false;
          if (valorDelay < 100) {
            Serial.println("Vc... eh o verdadeiro Ardu Hero...");
          }
          return;
        }
      }
    }
  }
  
}
