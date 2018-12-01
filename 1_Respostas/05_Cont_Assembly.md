## Para as questões 2 a 5, considere que as variáveis `f`, `g`, `h`, `i` e `j` são do tipo inteiro (16 bits na arquitetura do MSP430), e que o vetor `A[]` é do tipo inteiro. Estas variáveis estão armazenadas nos seguintes registradores:
	f: R4
	g: R5
	h: R6
	i: R7
	j: R8
	A: R9

	Utilize os registradores R11, R12, R13, R14 e R15
	para armazenar valores temporários.

## *1. Escreva os trechos de código assembly do MSP430 para:*

(a) Somente setar o bit menos significativo de R5.
```Assembly
bis #BIT0, R5
```
(b) Somente setar dois bits de R6: o menos significativo e o segundo menos significativo.
```Assembly
bis #BIT0, R6
bis #BIT1, R6
```
(c) Somente zerar o terceiro bit menos significativo de R7.
```Assembly
bic.w #BIT0, R7
```
 (d) Somente zerar o terceiro e o quarto bits menos significativo de R8.
```Assembly
bic.w #BIT2, R8
bic.w #BIT3, R8
```
(e) Somente inverter o bit mais significativo de R9.
```Assembly
xor.w #BIT7, R9
```
 (f) Inverter o nibble mais significativo de R10, e setar o nibble menos significativo de R10.
```Assembly
xor.w #0xF000, R10
bis #BIT0, R10
```
## *2. "Traduza" o seguinte trecho de código em C para o assembly do MSP430:*

```C
if(i>j) f = g+h+10;
else f = g-h-10;
```

```Assembly
cmp R8, R7			  
jl ELSE				  
add.w R6,R5  			
add.w #000a,R5
jmp EXIT

ELSE:
sub.w R6, R4
sub.w #000a,R5

EXIT:
```

## *3. "Traduza" o seguinte trecho de código em C para o assembly do MSP430:*

```C
while(save[i]!=k) i++;
```

```assembly
	WHILE:
	mov R7,R12
	rla R12
	add R10,R12
	cmp 0(R12),R9
	jeq EXIT
	inc R7
	jump WHILE
	EXIT:
```


## *4. "Traduza" o seguinte trecho de código em C para o assembly do MSP430:*

```C
for(i=0; i<100; i++) A[i] = i*2;
```

```Assembly
.bss i,2 ; 		 		
mov.w #0,&i ;			 
mov.w i,R11
for_loop: cmp.w #100,&i	 ;R10 = A, if(i<100)
jge for_done
mov.w i,R12
rla R12
mov.w 0(R11),R12
add.w #1,&i
jmp for_loop

for_done:
```

## *5. "Traduza" o seguinte trecho de código em C para o assembly do MSP430:*

```C
for(i=99; i>=0; i--) A[i] = i*2;
```

```assembly
mov #99,R7
mov #0,R11
FOR:
rla R7
mov R7,0(R9)
inc R7
add R7,R9
cmp R11,R7
jge FOR
```
