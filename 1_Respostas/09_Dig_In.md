## *1. Escreva uma função em C que faz o debounce de botões ligados à porta P1.*

```C
int Debounce()
{
    if((P1IN & BTN)== 0)
    {
    	Atraso_ms(5);
    	if((P1IN & BTN) == 0)
    		return 1;
    }
    return 0;
}
void Atraso_ms(volatile unsigned int ms)
{	 
    
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

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

## *2. Escreva um código em C que lê 9 botões multiplexados por 6 pinos, e pisca os LEDs da placa Launchpad de acordo com os botões. Por exemplo, se o primeiro botão é pressionado, os LEDs piscam uma vez; se o segundo botão é pressionado, os LEDs piscam duas vezes; e assim por diante. Se mais de um botão é pressionado, os LEDs não piscam.*

```C
#include <msp430.h>

#define ROW1 BIT2
#define ROW2 BIT3
#define ROW3 BIT4
#define ROWS (BIT2|BIT3|BIT4)

#define COL1 BIT5
#define COL2 BIT6
#define COL3 BIT7
#define COLS (BIT5|BIT6|BIT7)


#define LEDS (LED1|LED2)

void pisca(); 
void Atraso_ms(volatile unsigned int ms); 
int numPos(long unsigned int i, long unsigned int j);

void main(void)
{
    int coord[3][3]; // [ROW][COL]
    long unsigned int i, j;
    int q_press, num;

    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= LEDS;
    P1DIR &= ~ROWS; 
    P1DIR |= COLS; 

    P1REN |= ROWS; 
    P1OUT |= ROWS; 

    while(1)
    {
        q_press = 0;
        coord = [[0,0,0],[0,0,0],[0,0,0]];

        P1DIR |= LEDS;
        P1DIR &= ~ROWS;
        P1DIR |= COLS;

        P1REN |= ROWS;
        P1OUT |= ROWS;	
	P1OUT &= ~COLS;
	
        for (i = 0; i <= 2; i++) 
            if ((P1IN & i) == 0 ) 
                for (j = 0; j < 3; j++)
                    coord[i][j] = 1; 
   
        P1DIR |= ROWS;
        P1DIR &= ~COLS;
	
        P1REN |= COLS;
	P1OUT |= COLS;
        P1OUT &= ~ROWS;
        
        for (i = 0; i <= 2; i++)
            if ((P1IN & i) == 0 ))
            	for (j = 0; j < 3; j++)
              		coord[j][i] = 1; 

        for (i = 0; i < 3; i++) 
        {
            for (j = 0; j < 3; j++) 
            {
                if(coord[i][j] == 2) 
                    q_press++;			  
		    num = cord[i][j] == 2 ? numPos(i,j) : num;
            }
        }
        if (q_press == 1)
            for(i=0; i < num; i++)
                pisca();
    }

}

void pisca()
{
	P1OUT |= LEDS;
	Atraso_ms(1000);
	P1OUT &= ~LEDS;	
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

int numPos(long unsigned int i, long unsigned int j)
{
	return (int) 3*j+i+1;
}

```
