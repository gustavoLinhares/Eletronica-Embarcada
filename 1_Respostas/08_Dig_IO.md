## Para todas as questões, utilize os LEDs e/ou os botões da placa Launchpad do MSP430.

## 1. Escreva um código em C que pisca os LEDs ininterruptamente.

```C
//Pisca Leds
#include <msp430g2553.h>

#define LED1 BIT0
#define LED2 BIT6

void main (void)
{
    volatile int i;
    WDTCTL = WDTPW|WDTHOLD;
    P1DIR  = LED1 + LED2;
    P1OUT = LED1;

    for(;;)
    {
        // Liga e desliga o Led
        P1OUT ^= (LED1 + LED2);
        // Atraso
        for (i = 0; i < 0x6000; i++);
    }
}
```

## 2. Escreva um código em C que pisca os LEDs ininterruptamente. No ciclo que pisca os LEDs, o tempo que os LEDs ficam ligados deve ser duas vezes maior do que o tempo que eles ficam desligados.

## 3. Escreva um código em C que acende os LEDs quando o botão é pressionado.

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

## 4. Escreva um código em C que pisca os LEDs ininterruptamente somente se o botão for pressionado.

## 5. Escreva um código em C que acende os LEDs quando o botão é pressionado. Deixe o MSP430 em modo de baixo consumo, e habilite a interrupção do botão.
```C
#include <msp430g2553.h>
#include "legacymsp430.h" // Interrupcoes

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

    P2OUT |= BTN;
    P2IES |= BTN;

    P2IE |= BTN;
    _BIS_SR(GIE);

    while(1)
    {
        P1OUT ^= LEDS;
        for(i = 0; i < 0x6000; i++)
    }
    return 0;
}
```
