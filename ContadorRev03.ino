// -- Variaveis Globais --

const short int Potent = A0; // pino com o poteciometro.
const short int Button = 2; // pino com ligado ao botão que gerara interrupção

const short int Segment[8] = {3,4,5,6,7,8,9,10}; // Array com os pinos correspondente do display de segmentos.
const short int PinMult[2] = {13,12}; // pinos ligados no catodo comum dos displays.

unsigned short int PreMill[3] = {0,0,0}; // Array que será utlizado para controle do delay.
unsigned short int CurrMill = 0; // Variavel que será utitilizado para controle do delay.

short int Count = 0; // Variavel que será utilizado como contador.

bool Pause = true; // Pausa a contagem.

// -- Prototipos --

void Prio(); // função que sera executada na interrupção. Acendera cada segmento no tempo de 250 milessegundos.
void Increment(); // Função responsavel por incrementar o contador e tratar quando voltar o valor para zero.
void EnabSeg(short int Num); // sera responsavel por habilitar ou desabilitar os segmentos de acordo com numero informado.
void EnabDev(short int St); // Esta função Habilitara ou Desabilitara ambos os displays com base no parametro St.
void EnabDev(short int Pin, short int St); // função sera responsavel em ligar ou desligar o display com base no paramentro Num (Define qual display).
void ClearAll();   // Desabilita os segmentos que estiverem habilitados.
// -- --

void setup() {

  for ( short int i = 0; i <= 1; i++ ) { // Sera utilizado para definir os pinos que ligam o display
    pinMode(PinMult[i],OUTPUT); // Define os pinos do array como OUTPUT.
    digitalWrite(PinMult[i],!LOW); // Inializa com os dipslays desligados.
  }

  for ( short int i = 0; i <= 7; i++ ) // será utilizado para definir os pinos como saida para o display de segmento.
    pinMode(Segment[i], OUTPUT);  //inicializando os pinos como saida.

  pinMode(Button,INPUT_PULLUP); // Habilitando Pino 2 como entrada com resistor PULLUP para botão.
  pinMode(Potent,INPUT); // Habilitando o pino A0 como entrada para poteciomentro

  attachInterrupt(digitalPinToInterrupt(Button), Prio, RISING); // Definindo que o botão será utilizado para interrupção quando o botão sair estado baixo para alto.
}

void loop() {
  CurrMill = millis(); // carregando a variavel com valor da função millis. Retorna quanto tempo o arduino esta ligado em milessegundos. 

  if ( Pause ) // se verdadeiro faz com que a contagem pare
    PreMill[0] = CurrMill; // armazena o valor atual da varivel mills. Faz com que a diferença entre CurrMill e Premill[0] seja zero, assim quando retorna a contagem, o contador não sera incrementado imediatamente.

  else if ( CurrMill - PreMill[0] >= 1000 ) { // se tiver passado 1 segundo executa procedimentos.
    Increment(); // incremeta contador.
    PreMill[0] = CurrMill; // armazena o valor atual da varivel mills. Isso fara que creça a diferença entre a variavel CurrMill e Premill[0], permitindo a comparação acima.
  }

  if ( (CurrMill - PreMill[1] >= 300) && (CurrMill - PreMill[2] <= 3000) ) { // a cada 300 milessegundos, isso é caso o botão não tenha sido pressionado por 3000 milessegundos ele acende ou apaga o segmento DP
    digitalWrite(Segment[7], !digitalRead(Segment[7])); // define ligado ou desligado para segmento DP.
    PreMill[1] = CurrMill; // armazena o valor atual da varivel mills. Isso fara que haja diferença no intervalo na hora comparar as variaveis.
  }

  else if ( (digitalRead(Segment[7]) == LOW) && (CurrMill - PreMill[2] >= 3000) )
    digitalWrite(Segment[7], !digitalRead(Segment[7])); // liga o segmento DP caso esteja desligado. Isso permitira identificar visualmente quando deverar solta o botão para zerar o contador.

  if ( digitalRead(Button) != 0 ) // se botão for precionado o vetor PreMill[2], deixara de ser atualizado, fazendo com que creça a diferença entre PreMill[2] e CurrMill. Permitindo identificar quanto tempo o botão foi pressionado.
    PreMill[2] = CurrMill; // armazena o valor atual da varivel mills. Permite faz com que a diferença seja zero, permitindo a veriguar se é somente pausar ou pausar e zerar a contagem.

  EnabSeg(Count); // exibe o numero no display.
}

void Prio() {
  ClearAll(); // Desativa os segmentos que estão habilitados.
  EnabDev(HIGH); // Habilita ambos os displays

  for ( short int i = 0; i < 7; i++ ) {
    digitalWrite(Segment[i], HIGH); // habilita o segmento.

    delay(10000); // gera um delay.

    digitalWrite(Segment[i], LOW); // desabilita o segmento.
  }

  EnabDev(LOW); // Desabilita ambos os displays

  if ( CurrMill - PreMill[2] >= 3000 ) { // Verifica se o botão foi segurado por 3 segundos.
    if ( !Pause )
      Pause = !Pause; // Altera o valor da variavel para que contador seja pausado

    Count = 0; // zera contador.
  }

  else
    Pause = !Pause; 
}

void Increment() {
  Count++; // incrementa o contador

  if ( Count > 59 ) // zera o contador caso ultrapasse o valor 59.
    Count = 0; // incrementa contador[0]
}

void EnabSeg(short int Num) {
  // matriz que contem os esquemas de exibição dos numeros no display. 
  // As linhas corresponde o "numero", enquanto as colunas contem status 
  // dos pinos ({3,4,5,6,7,8,9}) que formarão o numero no display. 

  short int State[10][7] = {{1,1,1,1,1,1,0},  // Numero 0
                            {0,1,1,0,0,0,0},  // Numero 1
                            {1,1,0,1,1,0,1},  // Numero 2
                            {1,1,1,1,0,0,1},  // Numero 3
                            {0,1,1,0,0,1,1},  // Numero 4
                            {1,0,1,1,0,1,1},  // Numero 5
                            {1,0,1,1,1,1,1},  // Numero 6
                            {1,1,1,0,0,1,0},  // Numero 7
                            {1,1,1,1,1,1,1},  // Numero 8
                            {1,1,1,1,0,1,1}}; // Numero 9

  short int Disp[2] = {Num/10,Num%10}; // separa a dezena e a unidade nas respectivas posições do vetor.

  for ( short int i = 0; i<= 1; i++ ) { // sera responsavel por indicar qual display habilitar e desabilitar.
    for ( short int j = 0; j < 7; j++ ) { // responsavel por acender os segmentos de acordo com valor passado para função
      digitalWrite(Segment[j], State[Disp[i]][j]); // exibe o numero de acordo com esquema salvo na matriz.
    }

    EnabDev(i,HIGH); EnabDev(i,LOW); // habilita o display[i] 1 milessegundo e depois o desabilita.
  }
}

void EnabDev(short int St) {
  for ( short int x = 0; x <= 1; x++ )
    digitalWrite(PinMult[x],!St);
}

void EnabDev(short int Pin, short int St) {  
  short int Delay = analogRead(Potent) + 1; // Poteciômetro sera utilizado para controlar a taxa de atualização.

  digitalWrite(PinMult[Pin],!St); // escreve o status do pino

  if ( !St == LOW )
    delay(Delay); // Aplica delay, conforme o valor do poteciometro.
}

void ClearAll() {
  for ( short int i = 0; i <= 7; i++ ) // percorre cada segmento.
    if ( digitalRead(Segment[i]) == HIGH ) // se o segmento estiver habilitado, devera desabilitar.
      digitalWrite(Segment[i],LOW); // desabilita o segmento.
}
