## 1. Dada uma variável a do tipo char (um byte), escreva os trechos de código em C para:

(a) Somente setar o bit menos significativo de a.
```C
A |= BIT0;  
A |= 0x01;
```

(b) Somente setar dois bits de a: o menos significativo e o segundo menos significativo.
```C
A |= BIT0 + BIT1;
A |= 0x03;
```


(c) Somente zerar o terceiro bit menos significativo de a.
```C
A &= ~BIT2;      
A &= ~0x04;
```

(d) Somente zerar o terceiro e o quarto bits menos significativo de a.
```C
A &= ~(BIT2 + BIT1);
A &= ~0x0E;
```

(e) Somente inverter o bit mais significativo de a.
```C
A ^= ~BIT7;
A ^= 0x80;
```

(f) Inverter o nibble mais significativo de a, e setar o nibble menos significativo de a.
```C
A ^= 0xF0;
A |= 0x0F;
```

## 2. Considerando a placa Launchpad do MSP430, escreva o código em C para piscar os dois LEDs ininterruptamente.
```C
#include <msp430g2553.h>

#define LED1 BIT0
#define LED2 BIT6

void main (void)
{
    volatile int i;
    WDTCTL = WDTPW|WDTHOLD;
    P1DIR  = LED1 + LED2;
    P1OUT = LED1;

    while(1){
        P1OUT ^= (LED1 + LED2);
        for (i = 0; i < 0x6000; i++);
    }
}
```

## 3. Considerando a placa Launchpad do MSP430, escreva o código em C para piscar duas vezes os dois LEDs sempre que o usuário pressionar o botão.
```C
#include <msp430g2553.h>

#define LED1 BIT0
#define LED2 BIT6
#define BTN BIT3

void main (void)
{
	volatile int i;
	WDTCTL = WDTPW|WDTHOLD;
	P1DIR  = LED1 + LED2;
	P1OUT = 0;
   	while(1){
		if(BTN & P1OUT == 0){
			P1OUT = LED1 + LED2;
			for(i = 0; i < 1000; i++)
			P1OUT = 0;
			for(i = 0; i < 1000; i++)
			P1OUT = LED1 + LED2;
			for(i = 0; i < 1000; i++)
			P1OUT = 0;
		}
	}
}
```
## 4. Considerando a placa Launchpad do MSP430, faça uma função em C que pisca os dois LEDs uma vez.
```C
#include <msp430g2553.h>

int pisca(void)
{
  volatile int i;
  WDTCTL = WDTPW | WDTHOLD;
  P1DIR = BIT0 + BIT6;
  P1OUT = 0x00;
  P1OUT ^= BIT0 + BIT6 ;
  for (i = 0; i < 0x6000; i++);
  P1OUT ^= BIT0 + BIT6 ;
}
```

## 5. Reescreva o código da questão 2 usando a função da questão 4.
```C
#include <msp430g2553.h>

int pisca(void);

int main(void)
{
  volatile int i;
  WDTCTL = WDTPW | WDTHOLD;
  P1DIR = BIT0 + BIT6;
  P1OUT = 0x00;
  while(1){
	pisca();  	
  }
}
```

## 6. Reescreva o código da questão 3 usando a função da questão 4.
```C
void main (void)
{
	volatile int i;
	WDTCTL = WDTPW|WDTHOLD;
	P1DIR  = LED1 + LED2;
	P1OUT = 0;
   	while(1){
		if(BTN & P1OUT == 0){
			pisca();
		}
	}
}
```
