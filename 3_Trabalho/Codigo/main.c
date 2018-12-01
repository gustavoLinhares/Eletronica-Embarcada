#include <msp430g2553.h>
#include <legacymsp430.h>
#include <math.h>
#include <float.h>

// ------------Defines ----------
#define LEDS (BIT0|BIT6)
#define RX BIT1
#define TX BIT2

#define BAUD_9600   0
#define BAUD_19200  1
#define BAUD_38400  2
#define BAUD_56000  3
#define BAUD_115200 4
#define BAUD_128000 5
#define BAUD_256000 6
#define NUM_BAUDS   7

#define MUL 1 // multiplicação de escala

#define MOTORX 0
#define MOTORY 1
#define MOTORZ 2
#define POSF 3 // posicao F
#define POSP 4 // posicao p
#define POSI 5 // posicao i
#define POSJ 6 // posicao j
#define PERIODO 4
#define false 0
#define true 1

#define StepX BIT1
#define DirecaoX BIT2
#define StepY BIT3
#define DirecaoY BIT4
#define StepZ BIT5
#define DirecaoZ BIT7
#define Enable BIT0

// ------------------Funções----------------
int Read_String(char str[], int strlen);
char Receive_Data(void);
void Send_Data(unsigned char c);
void Send_Int(int n);
void Send_String(char str[]);
void Init_UART(unsigned int baud_rate_choice);
void Atraso_ms(volatile unsigned int ms);
int cmp_str(char str1[], char str2[]);
int cmp_char(char str1, char str2);
void processCommand(char str[]);
float get_position(char str1[], int t);
void stepMotor (int Passos, int direcao, int Motor);
void help();
void where();
float stof(char str1[], int position);
int Acha_char(char str1[], char str2);
void Enable_Motor(int a);
void movea(float x,float y);
void headera(int z);
void headerr(int z);
void mover(float x,float y);
void Circle(float x,float y, float i, float j, int dir);
void reset();
float fastsin(float x);

// ------------Variaveis Universais -----------
float x_pos = 0; //current x position
float y_pos = 0; //current y position
float z_pos = 0; //current z position
int feed = 0;
char str[59]; // Buffer do Gcode
int mode_abs = 1;

int main(void)
{

    WDTCTL = WDTPW + WDTHOLD;

    P1DIR |= StepX;
    P1DIR |= DirecaoX;


    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= LEDS;
    P1OUT |= LEDS;

    Init_UART(BAUD_115200);
    TA1CCR0 = 12500-1;
    TA1CTL = TASSEL_2 + ID_3 + MC_1;

    _BIS_SR(GIE);
    while(1)
    {
        TA1CTL |= TAIE;
        Read_String(str, 59);
        Send_String(str);
        processCommand(str);  // realiza os comandos
    }
    return 0;
}

interrupt(TIMER1_A1_VECTOR) TA1_ISR(void)
{
    P1OUT ^= LEDS;
    TA1CTL &= ~TAIFG;
}

int Read_String(char str[], int strlen)
{
    int i = 0;
    do
    {
        // Salva o caractere recebido
        str[i] = Receive_Data();
        // Ignora o '\r'
        if(str[i]!='\r')
        {
            // Troca '\n' por '\0'
            // e termina a recepção
            if(str[i]=='\n')
            {
                str[i] = '\0';
                break;
            }
            i++;
        }
    } while(i<strlen);

    // Se chegou ao final do vetor,
    // termina-o com '\0'
    if(i==strlen)
    {
        i--;
        str[i] = '\0';
    }
    // Retorna o tamanho da string
    // sem contar o '\0'
    return i;
}

char Receive_Data(void)
{
    while((IFG2&UCA0RXIFG)==0);
    return UCA0RXBUF;
}

void Send_Data(unsigned char c)
{
    while((IFG2&UCA0TXIFG)==0);
    UCA0TXBUF = c;
}

void Send_Int(int n)
{
    int casa, dig;
    if(n==0)
    {
        Send_Data('0');
        return;
    }
    if(n<0)
    {
        Send_Data('-');
        n = -n;
    }
    for(casa = 1; casa<=n; casa *= 10);
    casa /= 10;
    while(casa>0)
    {
        dig = (n/casa);
        Send_Data(dig+'0');
        n -= dig*casa;
        casa /= 10;
    }
}

// Envia uma string para o terminal
void Send_String(char str[])
{
    int i;
    for(i=0; str[i]!= '\0'; i++)
        Send_Data(str[i]);
}

// Inicia a UART com o msp430
void Init_UART(unsigned int baud_rate_choice)
{
    unsigned char BRs[NUM_BAUDS] = {104, 52, 26, 17, 8, 7, 3};
    unsigned char MCTLs[NUM_BAUDS] = {UCBRF_0+UCBRS_1,
                                        UCBRF_0+UCBRS_0,
                                        UCBRF_0+UCBRS_0,
                                        UCBRF_0+UCBRS_7,
                                        UCBRF_0+UCBRS_6,
                                        UCBRF_0+UCBRS_7,
                                        UCBRF_0+UCBRS_7};
    if(baud_rate_choice<NUM_BAUDS)
    {
        // Habilita os pinos para transmissao serial UART
        P1SEL2 = P1SEL = RX+TX;
        // Configura a transmissao serial UART com 8 bits de dados,
        // sem paridade, comecando pelo bit menos significativo,
        // e com um bit de STOP
        UCA0CTL0 = 0;
        // Escolhe o SMCLK como clock para a UART
        UCA0CTL1 = UCSSEL_2;
        // Define a baud rate
        UCA0BR0 = BRs[baud_rate_choice];
        UCA0BR1 = 0;
        UCA0MCTL = MCTLs[baud_rate_choice];
    }
}

// Faz um atraso em milisegundos
void Atraso_ms(volatile unsigned int ms)
{

    TACCR0 = 1000-1;
    TACTL = TACLR;
    TACTL = TASSEL_2 + ID_0 + MC_1;
    while(ms--)
    {
        while((TACTL&TAIFG)==0);
        TACTL &= ~TAIFG;
    }
    TACTL = MC_0;
}

// Compara duas strings e retorna 1 caso o resultado seja positivo
int cmp_str(char str1[], char str2[])
{
    int i;
    for(i=0; (str1[i]!='\0')&&(str2[i]!='\0'); i++)
    {
        if(str1[i]!=str2[i])
            return 0;
    }
    if(str1[i]!=str2[i])
        return 0;
    else
        return 1;
}

// Compara dois caracteres e retorna 1 caso o resultado seja positivo
int cmp_char(char str1, char str2)
{
    if(str1!=str2)
        return 0;
    else
        return 1;
}

//Parametros da Funcao
// Passos numero a ser andado
//direção 1- Horario // 0-Antihorario
//Motor Escolha do motor pelos defines
void stepMotor (int Passos, int direcao, int Motor){

    int x;
    switch(Motor){
    case 0: // Motor X
      Atraso_ms(5);
      if(direcao==0){
          P1OUT &= ~DirecaoX;
      }
      else{
          P1OUT |= DirecaoX;
      }
      for(x = 0; x < Passos; x++) {
          P1OUT |= StepX;
          Atraso_ms(PERIODO/2);
          P1OUT &= ~StepX;
          Atraso_ms(PERIODO/2);
      }
    break;

    case 1 :// Motor Y
        Atraso_ms(5);
        if(direcao==0){
            P1OUT &= ~DirecaoY;
        }
        else{
            P1OUT |= DirecaoY;
        }
        for(x = 0; x < Passos; x++) {
            P1OUT |= StepY;
            Atraso_ms(PERIODO/2);
            P1OUT &= ~StepY;
            Atraso_ms(PERIODO/2);
        }
    break;
    case 2 :// Motor Z
        Atraso_ms(5);
        if(direcao==0){
            P1OUT &= ~DirecaoZ;
        }
        else{
            P1OUT |= DirecaoZ;
        }
        for(x = 0; x < Passos; x++) {
            P1OUT |= StepZ;
            Atraso_ms(PERIODO/2);
            P1OUT &= ~StepZ;
            Atraso_ms(PERIODO/2);
        }
    break;
    default: break;
    }
 }


//---------------------- Realização dos comandos do motor ----------------------------
void processCommand(char str[]){

    unsigned int cmd,i;
    float paralist[7] = {0,0,0,-1,0,0,0};

    paralist[0]=get_position(str,MOTORX);
    paralist[1]=get_position(str,MOTORY);
    paralist[2]=get_position(str,MOTORZ);
    paralist[3]=get_position(str,POSF);
    paralist[4]=get_position(str,POSP);
    paralist[4]=(int)paralist[4];
    paralist[5]=get_position(str,POSI);
    paralist[6]=get_position(str,POSJ);
    cmd=0;
    i=0;

    while (cmp_char(str[i+1],' ')!=1){
          if (str[i+1]=='\0'){
              break;
          }
          cmd = cmd*10 + str[i+1] - '0'; // Convertendo em int a string
          i++;
    }

    if(str[0]=='G'){
        switch(cmd){
            case  0: // move in a line
            case  1: // move in a line
                if(mode_abs){
                    movea(paralist[0],paralist[1]);
                    headera(paralist[2]);
                }else{
                  mover(paralist[0],paralist[1]);
                  headerr(paralist[2]);
                }
            break;
            case  3:
                 Circle(paralist[0],paralist[1],paralist[5],paralist[6],cmd - 2);
                 break;
            case  4:
                 Atraso_ms(paralist[4]);
                 break;  // wait a while
            case 28:
                    reset();
                    movea(0,0);
                    break;
            case 90:
                    mode_abs = 1;  break;  // absolute mode
            case 91:
                    mode_abs = 0;  break;  // relative mode
            case 92:  // set logical position
                    if(paralist[0] != 0){
                        x_pos = paralist[0];
                    }else if(paralist[1] != 0){
                        y_pos = paralist[1];
                    }else if(paralist[2] != 0){
                        z_pos = paralist[2];
                    }else{
                      x_pos = 0;
                      y_pos = 0;
                      z_pos = 0;
                    }
                    break;
                  default:  break;
        }
    }else if(cmp_char(str[0],'M')==1){
        switch(cmd){
            case 17:  // turns on power to steppers (releases the grip)
                Enable_Motor(1);
               break;
             case 18:  // turns off power to steppers (releases the grip)
                 Enable_Motor(0);
               break;
             case 100:  help();  break;
             case 114:
                 if(paralist[0] != 0){
                     x_pos = paralist[0];
                 }else if(paralist[1] != 0){
                     y_pos = paralist[1];
                 }else if(paralist[2] != 0){
                     z_pos = paralist[2];
                 }else{
                   x_pos = 0;
                   y_pos = 0;
                   z_pos = 0;
                 }
                 where();  break;  // prints px, py, fr, and mode.
             default:  break;
           }
    }
  }

void reset(){
  headerr(z_pos);
}

void Circle(float x,float y, float i, float j, int dir){
  if(i == j && j == 0)
    movea(x,y);


  float centx, centy;

  // Centre coordinates are always relative
  centx = i + x_pos/MUL;
  centy = j + y_pos/MUL;
  float angleA, angleB, angle, radius, length, aX, aY, bX, bY;

  aX = (x_pos/MUL - centx);
  aY = (y_pos/MUL - centy);
  bX = (x/MUL - centx);
  bY = (y/MUL - centy);

  if (dir == 0) { // Clockwise
    angleA = atan2(bY, bX);
    angleB = atan2(aY, aX);
  } else { // Counterclockwise
    angleA = atan2(aY, aX);
    angleB = atan2(bY, bX);
  }

  // Make sure angleB is always greater than angleA
  // and if not add 2PI so that it is (this also takes
  // care of the special case of angleA == angleB,
  // ie we want a complete circle)
  if (angleB <= angleA) angleB += 2 * M_PI;
  angle = angleB - angleA;
  if(angle == 0)
    return;

  radius = sqrt(aX * aX + aY * aY);
  length = radius * angle;
  int steps, s, ss;
  steps = (int) ceil(length / 0.1);

  float nx, ny;
  for (s = 1; s <= steps; s++) {
    ss = (dir == 1) ? s : steps - s; // Work backwards for CW
    nx = centx + radius * fastsin(angleA + angle * ((float) ss / steps) + (M_PI/2));
    ny = centy + radius * fastsin(angleA + angle * ((float) ss / steps));
    movea(floor(nx*MUL+0.5), floor(ny*MUL+0.5));

  }
}

float fastsin(float x)
{
  while(x > M_PI)
    x -= 2*M_PI;
  while(x < -M_PI)
    x += 2*M_PI;

  float B = (4 / M_PI);
  float c = -4 / (M_PI * M_PI);

  //return 0;
  return B * x + c * x * ((x < 0) ? -x : x);
}
void movea(float x,float y){
  int dx = (int)(x - x_pos);
  int dy = (int)(y - y_pos);
  mover(dx,dy);
}

void headera(int z){
  int dz = z_pos - z;
  headerr(dz);
}

void headerr(int z){
    stepMotor(z,0,MOTORZ);
    z_pos -= z;
}

void mover(float x,float y){
  int dirx;
  int diry;
  int i;
  x=(int)x;
  y=(int)y;
  dirx= 1;
  diry= 1;
  //stepMotor(x,0,MOTORX);
  if(x == y){
    for(i = 0; i < x; i++){
      stepMotor(dirx,0,MOTORX);
      x_pos += dirx;
      stepMotor(diry,1,MOTORY);
      y_pos += diry;
    }
  }else if(x > y){
    float acc = 0;
    int flag = false;
    for(i = 0; i < x; i++){
      stepMotor(dirx,0,MOTORX);
      x_pos += dirx;
      if(flag){
        stepMotor(diry,1,MOTORY);
        y_pos += diry;
        flag = false;
      }
      acc += y / x;
      if(acc > 0.5){
        flag = true;
        acc--;
      }
    }
  }else{
    float acc = 0;
    int flag = false;
    for(i = 0; i < y; i++){
      stepMotor(diry,1,MOTORY);
      y_pos += diry;
      if(flag){
        stepMotor(dirx,0,MOTORX);
        x_pos += dirx;
        flag = false;
      }
      acc += x / y;
      if(acc > 0.5){
        flag = true;
        acc--;
      }
    }
  }
}

void Enable_Motor(int a){
    if(a==0){
       P2OUT &= ~Enable;
    }
    else{
       P2OUT |= Enable;
    }

}

float get_position(char str1[], int t){
    int position=0;
    float resultado=0;
    switch(t){
        case 0:
            position=Acha_char(str1,'X');
            break;
        case 1:
            position=Acha_char(str1,'Y');
            break;
        case 2:
            position=Acha_char(str1,'Z');
            break;
        case 3:
            position=Acha_char(str1,'F');
            break;
        case 4:
            position=Acha_char(str1,'P');
            break;
        case 5:
            position=Acha_char(str1,'I');
            break;
        case 6:
            position=Acha_char(str1,'J');
            break;
        default:  break;
    }
    resultado = stof(str1,position);
    return resultado;
}

float stof(char str1[], int position){
    float result= 0.0f;
    int dotpos = 0;
    int len = 8;
    int n;
    if (position==0){
        return result;
    }
    for (n = position; n <= position+7 ; n++)
    {
      if (cmp_char(str1[n],'.'))
      {
        dotpos = position+len - n  - 1;
      }
      else
      {
        result = result * 10.0f + (str1[n]-'0');
      }
    }
    while ( dotpos--)
    {
      result /= 10.0f;
    }
    return result;

}

int Acha_char(char str1[], char str2){
    int i, flag;
    flag=0;
    for(i=0; (str1[i]!='\0'); i++)
    {
        if(cmp_char(str1[i],str2)==1){
            flag=i;
            flag++;
            break;
        }
    }
    return flag;

}

void where() {
    Send_String("\n\rCurrent State:\n\r");
    Send_String("X:");
    Send_Int(x_pos);
    Send_String("\n\rY:");
    Send_Int(y_pos);
    Send_String("\n\rZ:");
    Send_Int(z_pos);
    Send_String("\n\rFeed Rate:");
    Send_Int(feed);
}

void help() {
    Send_String("\n\rCommands:\n\r");
    Send_String("G00 [X(steps)] [Y(steps)] [Z(steps)] [F(feedrate)]; - linear move\n\r");
    Send_String("G01 [X(steps)] [Y(steps)] [Z(steps)] [F(feedrate)]; - linear move\n\r");
    Send_String("G04 P[seconds]; - delay\n\r");
    Send_String("G90; - absolute mode\n\r");
    Send_String("G91; - relative mode\n\r");
    Send_String("G92 [X(steps)] [Y(steps)] [Z(steps)]; - change logical position\n\r");
    Send_String("M18; - disable motors\n\r");
    Send_String("M100; - this help message\n\r");
    Send_String("M114; - report position and feedrate\n\r");
}

