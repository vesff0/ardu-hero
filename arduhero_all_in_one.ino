/*ATENÇÃO PROGRAMADORES: 
Este código NÃO foi feito por um programdor e pode ser doloroso para a vista de um programador de verdade.
Este código possui sequências INEFICIENTES e talvez precise com urgência de otimização.
Por favor, prossigam com isso em mente.
*/

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
volatile int modoJogo = 1;
//modo 1 = modo clássico
//modo 2 = modo níveis
//modo 3 = modo random
//modo 4 = modo multiplayer
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
int pontoMax = 0;
int valorDelay;

//Interrupt que será chamado sempre que o botão for pressionado;
void buttonInterrupt() {
  buttonState = digitalRead(buttonPin);
  buttonState2 = digitalRead(buttonPin2);
  ledState = digitalRead(highestPin);
  ledState2 = digitalRead(lowestPin);
  verificador = false;
  //Muda o estado de gameBegin para iniciar o jogo
  if (!gameBegin && verificador == false) {
    if (buttonState == LOW) {
      gameBegin = true;
    }
  }
  verificador = false;
  //O comportamento do interrupt fazia com que essa parte do código rodasse várias vezes
  //a cada pressionar do botão, por isso criei a variável verificador.
  //verificador é uma variável que serve para confirmar o acionamento do botão.
  //se o botão já estiver pressionado, ele não vai rodar essa parte de novo.
  //Isso evita o código de rodar a mesma parte várias vezes.
  if (verificador == false) {
    //Verifica qual botão é o player atual. 
    //Se o modo selecionador for qualquer um que não seja multiplayer, 
    //o player sempre vai ser "p1," e "p2" sempre será falso.
    //Isso também impede outro jogador de pressionar botões quando não for
    //o turno dele.
    if (!p2) {
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
        pointFlag = true;
      }
      //Caso o usuário erre.
      else if (ledState2 == LOW && buttonState2 == LOW) {
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
    digitalWrite(r, LOW);
    digitalWrite(g, LOW);
     
  }
}


void setup() {
  //define todos os pins como OUTPUT
  for (int thisPin = lowestPin; thisPin <= highestPin; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }
  
  //define os compartamentos de todos os outros pins
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
  //enquanto o jogo não é iniciado... roda o "menu"
  while (!gameBegin) {
    if (modoJogo == 1) {
      Serial.println("Selecione o modo de jogo:");
      Serial.println("-> 1.Classico");
      Serial.println("2.Niveis");
      Serial.println("3.Velocidade aleatoria");
      Serial.println("4.Multiplayer");
      Serial.println("");
      while (digitalRead(buttonPin2) == HIGH && digitalRead(buttonPin) == HIGH) {
        //enquanto nenhum botão é pressionado, não faz nada
      }
      if (digitalRead(buttonPin2) == LOW) {
        //se o botão de trocar de modo for pressionado, troca de modo
        modoJogo = 2;
        delay(500);
      }
    }
    else if (modoJogo == 2) {
      Serial.println("Selecione o modo de jogo:");
      Serial.println("1.Classico");
      Serial.println("->2.Niveis");
      Serial.println("3.Velocidade aleatoria");
      Serial.println("4.Multiplayer");
      Serial.println("");
      while (digitalRead(buttonPin2) == HIGH && digitalRead(buttonPin) == HIGH) {
     
      }
      if (digitalRead(buttonPin2) == LOW) {
        modoJogo = 3;
        delay(500);
      }
    }
    else if (modoJogo == 3) {
      Serial.println("Selecione o modo de jogo:");
      Serial.println("1.Classico");
      Serial.println("2.Niveis");
      Serial.println("->3.Velocidade aleatoria");
      Serial.println("4.Multiplayer");
      Serial.println("");
      while (digitalRead(buttonPin2) == HIGH && digitalRead(buttonPin) == HIGH) {
     
      }
      if (digitalRead(buttonPin2) == LOW) {
        modoJogo = 4;
        delay(500);
      }
    }
    else if (modoJogo == 4) {
      Serial.println("Selecione o modo de jogo:");
      Serial.println("1.Classico");
      Serial.println("2.Niveis");
      Serial.println("3.Velocidade aleatoria");
      Serial.println("->4.Multiplayer");
      Serial.println("");
      while (digitalRead(buttonPin2) == HIGH && digitalRead(buttonPin) == HIGH) {
     
      }
      if (digitalRead(buttonPin2) == LOW) {
        modoJogo = 1;
        delay(500);
      }
    }
  }

  if (gameBegin) {
    acerto = true;
    p1 = true;
    //por algum motivo essa parte roda duas vezes... eu desisti de tentar entender o porquê.
    Serial.println("Iniciando jogo no modo selecionado... Olhe para os LEDs!");
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
      //Começa o jogo no modo selecionado. A partir daqui os comentários estão desatualizados.
      switch (modoJogo) {
        case 1:
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
              if (pontoMax == 0) {
                pontoMax = pontos;
              }
              else if (pontos > pontoMax) {
                pontoMax = pontos;
              }
              Serial.println("");
              Serial.print("Sua pontuacao eh de ");
              Serial.print(pontos);
              Serial.println(" pontos.");
              Serial.println("");
              delay(100000);
              pontos = 0;
              gameBegin = false;
              if (valorDelay < 100) {
                Serial.println("Vc... eh o verdadeiro Ardu Hero...");
              }
              return;
            }
          }
        break;  
        case 2:
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
            if (pontos == 5) {
              Serial.println("Parabens! Voce passou para o nivel 2, a velocidade ira aumentar");
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(1000);
              valorDelay = 400;
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
            }
            else if (pontos == 15) {
              Serial.println("Parabens! Voce passou para o nivel 3, a velocidade ira aumentar");
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(1000);
              valorDelay = 350;
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
            }
            else if (pontos == 25) {
              Serial.println("Parabens! Voce passou para o nivel 4, a velocidade ira aumentar");
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(1000);
              valorDelay = 300;
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
            }
            else if (pontos == 35) {
              Serial.println("Parabens! Voce passou para o nivel 5, a velocidade ira aumentar");
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(1000);
              valorDelay = 250;
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
            }
            else if (pontos == 45) {
              Serial.println("Parabens! Voce passou para o nivel 6, a velocidade ira aumentar");
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(1000);
              valorDelay = 200;
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
            }
            else if (pontos == 55) {
              Serial.println("Parabens! Voce passou para o nivel 7, a velocidade ira aumentar");
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(1000);
              valorDelay = 170;
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
            }
            else if (pontos == 65) {
              Serial.println("Parabens! Voce passou para o nivel 8, a velocidade ira aumentar");
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(1000);
              valorDelay = 150;
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
            }
            else if (pontos == 75) {
              Serial.println("Parabens! Voce passou para o nivel 9, a velocidade ira aumentar");
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(1000);
              valorDelay = 130;
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
            }
            else if (pontos == 75) {
              Serial.println("Parabens! Voce passou para o nivel 10, a velocidade ira aumentar");
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(1000);
              valorDelay = 100;
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
              delay(valorDelay);
              digitalWrite(lowestPin, HIGH);
              delay(valorDelay);
              digitalWrite(lowestPin, LOW);
            }
            else if (pontos == 100) {
              Serial.println("Parabens! Voce eh o verdadeiro Ardu Hero!");
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
              delay(500);
              digitalWrite(g, HIGH);
              delay(500);
              digitalWrite(g, LOW);
             
             
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
            delay(100000);
            pontos = 0;
            gameBegin = false;
            if (valorDelay < 100) {
              Serial.println("Vc... eh o verdadeiro Ardu Hero...");
            }
            return;
          }
        }
        break;
        case 3:
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
            if (pontos <= 10 && pontos > 0) {
              valorDelay = random(300, 500);  
            }
            else if (pontos >= 11 && pontos <= 20) {
              valorDelay = random(200, 300);
            }
            else if (pontos >= 21) {
              valorDelay = random(100, 200);
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
            delay(100000);
            pontos = 0;
            gameBegin = false;
            if (valorDelay < 100) {
              Serial.println("Vc... eh o verdadeiro Ardu Hero...");
            }
            return;
          }
        }
        break;
        case 4:
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
                delay(100000);
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
                delay(100000);
                pontos = 0;
                gameBegin = false;
                if (valorDelay < 100) {
                  Serial.println("Vc... eh o verdadeiro Ardu Hero...");
                }
                return;
              }
            }
          }    
        break;
      }
    }
  }
}
