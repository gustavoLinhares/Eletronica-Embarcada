## *1. Projete o hardware necessário para o MSP430 controlar um motor DC de 12V e 4A. Utilize transistores bipolares de junção (TBJ) com Vbe = 0,7 V, beta = 100 e Vce(saturação) = 0,2 V. Além disso, considere que Vcc = 3 V para o MSP430, e que este não pode fornecer mais do que 10 mA por porta digital.*

Primeiramente Transistor Bipolar ou Par Darlington 

```
Ic = B*Ib
4 = 100*Ib
Ib = 40 mA
```

Transistor Darlington devido Ib>10mA

```
Ic = Ib*B^2
4 = Ib*100^2
Ib = 0.4 mA
Rb = (Vcc -2*Vbe)/Ib
Rb = (3 -2*0.7)/0.4*10^-3
Rb = 4 kOhm
```
## *2. Projete o hardware necessário para o MSP430 controlar um motor DC de 10V e 1A. Utilize transistores bipolares de junção (TBJ) com Vbe = 0,7 V e beta = 120. Além disso, considere que Vcc = 3,5 V para o MSP430, e que este não pode fornecer mais do que 10 mA por porta digital.*

Primeiramente Transistor Bipolar ou Par Darlington 
```
Ic = B*Ib
1 = 120*Ib
Ib = 8.33 mA
```

Transistor normal devido Ib < 10mA
```
Rb = (Vcc -Vbe)/Ib
Rb = (3.5 -0.7)/8.33*10**-3
Rb = 336 Ohm
```

## *3. Projete o hardware utilizado para controlar 6 LEDs utilizando charlieplexing. Apresente os pinos utilizados no MSP430 e os LEDs, nomeados L1-L6.*


## *4. Defina a função `void main(void){}` para controlar 6 LEDs de uma árvore de natal usando o hardware da questão anterior. Acenda os LEDs de forma que um ser humano veja todos acesos ao mesmo tempo.*

## *5. Defina a função `void main(void){}` para controlar 6 LEDs de uma árvore de natal usando o hardware da questão 3. Acenda os LEDs de forma que um ser humano veja os LEDs L1 e L2 acesos juntos por um tempo, depois os LEDs L3 e L4 juntos, e depois os LEDs L5 e L6 juntos.*

## *6. Defina a função `void EscreveDigito(volatile char dig);` que escreve um dos dígitos 0x0-0xF em um único display de 7 segmentos via porta P1, baseado na figura abaixo. Considere que em outra parte do código os pinos P1.0-P1.6 já foram configurados para corresponderem aos LEDs A-G, e que estes LEDs possuem resistores externos para limitar a corrente.*

```
        ---  ==> A
       |   |
 F <== |   | ==> B
       |   |
        ---  ==> G
       |   |
 E <== |   | ==> C
       |   |
        ---  ==> D
```

```C

#define SA BIT0
#define SB BIT1
#define SC BIT2
#define SD BIT3
#define SE BIT4
#define SF BIT5
#define SG BIT6

void EscreveDigito(volatile char dig)
{
	P1OUT = 0;
	switch(dig)
	{
		case 'A':
			P1OUT |= (SA + SB + SC + SE + SF);
			break;
		case 'B':
			P1OUT |= (SF + SE + SC + SD + SG);
			break;
		case 'C':
			P1OUT |= (SA + SF + SE + SD);
			break;
		case 'D':
			P1OUT |= ~(SA + SF);
			break;
		case 'E':
			P1OUT |= ~(SB + SC);
			break;
		case 'F':
			P1OUT |= ~(SB + SC + SD);
			break;
		case '9':
			P1OUT |= ~(SD + SE);
			break;
		case '8':
			P1OUT ˆ= (P1OUT);
			break;
		case '7':
			P1OUT |= (SA + SB + SC);
			break;
		case '6':
			P1OUT |= ~(SB);
			break;
		case '5':
			P1OUT |= ~(SB + SE);
			break;
		case '4':
			P1OUT |= ~(SA + SE + SD);
			break;
		case '3':
			P1OUT |= ~(SF + SE);
			break;
		case '2':
			P1OUT |= ~(SF + SC);
			break;
		case '1':
			P1OUT |= (SB + SC);
			break;
		case '0':
			P1OUT |= ~(SG);
			break;	
		default:
			break;
	}
}
```

7. Multiplexe 2 displays de 7 segmentos para apresentar a seguinte sequência em loop:
	00 - 11 - 22 - 33 - 44 - 55 - 66 - 77 - 88 - 99 - AA - BB - CC - DD - EE - FF

```C
#define DISPLAYOUT P2OUT
#define DISPLAY1 BIT0
#define DISPLAY2 BIT1

void Atraso_ms(volatile unsigned int ms); 

void SeqTeste()
{
	for(int i = 0; i <= 9; i++)
	{
		DISPLAYOUT |= DISPLAY1 + ~DISPLAY2;
		EscreveDigito((char) '0'+i);
		Atraso_ms(10);
		DISPLAYOUT |= DISPLAY2 + ~DISPLAY1;
		EscreveDigito((char) '0'+i);
		Atraso_ms(10);
	}
		for(int i = 0; i <= 6; i++)
	{
		DISPLAYOUT |= DISPLAY1 + ~DISPLAY2;
		EscreveDigito((char) 'A'+i);
		Atraso_ms(10);
		DISPLAYOUT |= DISPLAY2 + ~DISPLAY1;
		EscreveDigito((char) 'A'+i);
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
