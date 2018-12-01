## *Para todas as questões, utilize os LEDs e/ou os botões da placa Launchpad do MSP430.*

## *1. Escreva um código em C que pisca os LEDs ininterruptamente.*

```C
#include <msp430g2553.h>

#define LED1 BIT0
#define LED2 BIT6
#define LEDS (BIT0|BIT6)

void Atraso_ms(volatile unsigned int ms);

void main (void)
{
    volatile int i;
    WDTCTL = WDTPW|WDTHOLD;
    P1DIR | = LEDS;
    P1OUT | = LEDS;
    
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    
    while(1)
    {
        P1OUT ^= LEDS;
        for (i = 0; i < 0x6000; i++);
    }
}

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

```

## *2. Escreva um código em C que pisca os LEDs ininterruptamente. No ciclo que pisca os LEDs, o tempo que os LEDs ficam ligados deve ser duas vezes maior do que o tempo que eles ficam desligados.*

```C
#include <msp430g2553.h>

#define LED1 BIT0
#define LED2 BIT6
#define LEDS (BIT0|BIT6)

void Atraso_ms(volatile unsigned int ms);

void main (void)
{
    volatile int i;
    WDTCTL = WDTPW|WDTHOLD;
    P1DIR | = LEDS;
    P1OUT |= LEDS;

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    while(1)
    {
        P1OUT |= LEDS;
        Atraso_ms(20);
        P1OUT &= ~LEDS;
        Atraso_ms(10);
    }
}

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

```



## *3. Escreva um código em C que acende os LEDs quando o botão é pressionado.*

```C
#include <msp430g2553.h>

#define LED1 BIT0
#define LED2 BIT6
#define LEDS (LED1|LED2)
#define BTN  BIT3

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1OUT |= LEDS;
    P1DIR |= LEDS;

    P2DIR &= ~BTN;
    P2REN |= BTN;
    P2OUT |= BTN;

    while(1)
    {
        if(P2IN & BNT == 0)
          P1OUT |= LEDS;
        else
          P1OUT &= ~LEDS;
    }
}
```

## *4. Escreva um código em C que pisca os LEDs ininterruptamente somente se o botão for pressionado.*

```C
#include <msp430g2553.h>

#define LED1 BIT0
#define LED2 BIT6
#define LEDS (LED1|LED2)
#define BTN  BIT3

void Atraso_ms(volatile unsigned int ms);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1OUT |= LEDS;
    P1DIR |= LEDS;
    
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    
    P2DIR &= ~BTN;
    P2REN |= BTN;
    P2OUT |= BTN;

    while(1)
    {
        if(P2IN & BNT == 0){
          P1OUT |= LEDS;
          Atraso_ms(500)
          P1OUT &= ~LEDS;
          }
    }
}


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


```

## *5. Escreva um código em C que acende os LEDs quando o botão é pressionado. Deixe o MSP430 em modo de baixo consumo, e habilite a interrupção do botão.*
```C
#include <msp430g2553.h>
#include "legacymsp430.h" 

#define LED1 BIT0
#define LED2 BIT6
#define LEDS (LED1|LED2)
#define BTN  BIT3


int main(void)
{
    volatile int i;
    WDTCTL = WDTPW | WDTHOLD;
    P1OUT |= LEDS;
    P1DIR |= LEDS;

    P2DIR &= ~BTN;
    P2REN |= BTN;

    P1OUT |= BTN;
    P1IES |= BTN;

    P1IE |= BTN;
    _BIS_SR(GIE+ LPM4_bits);

}

```C
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	P1DIR |= LEDS;
	P1DIR &= ~BTN;
	P1REN |= BTN;
	P1OUT |= BTN;
	P1IES |= BTN;
	P1IE |= BTN;
	_BIS_SR(GIE + LPM4_bits);
	return 0;
}

interrupt(PORT1_VECTOR) Interrupcao_P1(void)
{
	P1OUT |= LEDS;
	while((P1IN&BTN)==0);
	P1OUT &= ~LEDS;
	P1IFG &= ~BTN;
}
```
