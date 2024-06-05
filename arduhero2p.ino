//Define os pinos dos LEDs, botões e LED RGB.
const
int lowestPin = 4;

const
int highestPin = 9;

int buttonPin = 2;
int buttonPin2 = 3;

int r = 13;
int g = 12;
int b = 11;

//Define variáveis para lógica do jogo.
int modoJogo;
volatile int buttonState;
volatile int buttonState2;
volatile int ledState;
volatile int ledState2;
volatile bool acerto;
volatile bool verificador = false;
volatile bool pointFlag = false;
volatile bool gameBegin = false;
volatile bool p1 = true;
volatile bool p2 = false;
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
    buttonState2 = digitalRead(buttonPin2);
    ledState = digitalRead(highestPin);
    ledState2 = digitalRead(lowestPin);
  

    //Verifica se o botão foi pressionado quando o último led ("highestPin") estava aceso.
    //Caso sim, conta o acionamento como um acerto.
    if (!p2) {
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
  if (verificador == false) {
    if (p2) {
      if (ledState2 == HIGH && buttonState2 == LOW) {
        acerto = true;
        verificador = true;
        digitalWrite(g, HIGH);
        delay(500);
        digitalWrite(r, LOW);
        digitalWrite(g, LOW);

        //pointFlag é uma flag para contabilizar os pontos dentro do loop
        pointFlag = true;
      }
      //Caso o usuário erre.
      else if (ledState2 == LOW && buttonState2 == LOW) {
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
  }

    else {
      digitalWrite(r, LOW);
      digitalWrite(g, LOW);
      }
      
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
  pinMode(buttonPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPin2), buttonInterrupt, CHANGE);

}

void loop() {
  if (gameBegin) {

    acerto = true;
    p1 = true;

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
      if (p1) {
        for (int thisPin = lowestPin+1; thisPin <= highestPin; thisPin++) {
          verificador = false;

          if (thisPin == lowestPin+1) {
            digitalWrite(thisPin, LOW);
          }
          p2 = false;

          digitalWrite(thisPin, HIGH);

          if (thisPin == highestPin) {
            digitalWrite(thisPin-1, LOW);

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
            p1 = false;
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


      else {
        for (int thisPin = highestPin-1; thisPin >= lowestPin; thisPin--) {
          verificador = false;

          if (thisPin == highestPin-1) {
            digitalWrite(thisPin, LOW);
          }
          p2 = true;
          digitalWrite(thisPin, HIGH);

          if (thisPin == lowestPin) {
            digitalWrite(thisPin+1, LOW);

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
              digitalWrite(thisPin+1, LOW);
            }

            delay(valorDelay);
            if (pointFlag) {
            pontos++;
            pointFlag = false;
            p1 = true;
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
  
}
