## Para todas as questões abaixo, utilize o modo de comparação do Timer A.

## 1. Para os itens abaixo, confira a diferença no brilho do LED.
	
(a)/(b)/(c) Pisque o LED no pino P1.6 numa frequência de 100 Hz e ciclo de trabalho de 25% / 50% / 75%.	

```C
#include <msp430g2553.h>
#include <legacymsp430.h> 

#define LED BIT6
#define CAPT BIT2

PWM_Pisca(volatile unsigned int ciclo ,volatile unsigned int freq);	
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	P1DIR |= LED;
	P1OUT &= ~LED;
	P1DIR &= ~CAPT;
	P1SEL |= CAPT;
	P1SEL2 &= ~CAPT;

	TACCR0 = 5;
	TACCTL1 = CM_2 + CCIS_0 + SCS + CAP + CCIE;
	TACTL = TASSEL_2 + ID_0 + MC_1;
	_BIS_SR(LPM0_bits+GIE);
	return 0;
}

interrupt(TIMER0_A1_VECTOR) TA0_Capt1_ISR(void)
{
	PWM_Pisca(ciclo,100);
	TACCTL1 &= ~CCIFG;
}

void PWM_Pisca(volatile unsigned int ciclo ,volatile unsigned int freq)
{	
    unsigned int T = 1000000/f;
    T=T/2;
    TACCR0 = T-1;
    TACCR1 = (ciclo*T/100)-1;
    TACCTL1 = OUTMOD_7;
    TACTL = TASSEL_2 + ID_1 + MC_1;
}
```

## 2/3/4. Pisque o LED no pino P1.6 numa frequência de 1 Hz e ciclo de trabalho de 25% / 50% / 75%.

```C
#include <msp430g2553.h>
#include <legacymsp430.h> 

#define LED BIT6
#define CAPT BIT2

PWM_Pisca(volatile unsigned int ciclo ,volatile unsigned int freq);	
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	P1DIR |= LED;
	P1OUT &= ~LED;
	P1DIR &= ~CAPT;
	P1SEL |= CAPT;
	P1SEL2 &= ~CAPT;

	TACCR0 = 5;
	TACCTL1 = CM_2 + CCIS_0 + SCS + CAP + CCIE;
	TACTL = TASSEL_2 + ID_0 + MC_1;
	_BIS_SR(LPM0_bits+GIE);
	return 0;
}

interrupt(TIMER0_A1_VECTOR) TA0_Capt1_ISR(void)
{
	PWM_Pisca(ciclo,1);
	TACCTL1 &= ~CCIFG;
}

void PWM_Pisca(volatile unsigned int ciclo ,volatile unsigned int freq)
{	
    unsigned int T = 1000000/f;
    T=T/2;
    TACCR0 = T-1;
    TACCR1 = (ciclo*T/100)-1;
    TACCTL1 = OUTMOD_7;
    TACTL = TASSEL_2 + ID_1 + MC_1;
}
```

