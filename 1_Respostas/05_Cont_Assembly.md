## Para as questões 2 a 5, considere que as variáveis `f`, `g`, `h`, `i` e `j` são do tipo inteiro (16 bits na arquitetura do MSP430), e que o vetor `A[]` é do tipo inteiro. Estas variáveis estão armazenadas nos seguintes registradores:
	f: R4
	g: R5
	h: R6
	i: R7
	j: R8
	A: R9

	Utilize os registradores R11, R12, R13, R14 e R15
	para armazenar valores temporários.

## 1. Escreva os trechos de código assembly do MSP430 para:

(a) Somente setar o bit menos significativo de R5.
```Assembly
//bis realiza a operação OR bit-a-bit
bis #BIT0, R5
```
(b) Somente setar dois bits de R6: o menos significativo e o segundo menos significativo.
```Assembly
bis #BIT0, R6
bis #BIT1, R6
```
(c) Somente zerar o terceiro bit menos significativo de R7.
```Assembly
// bic.w A, B realiza a operação AND bit-a-bit de B com o inverso de A
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
cmp R8, R7			   ;R7 = i, R8 = j
jl ELSE				  ;if(j<i) jump else
add.w R6,R5  			;R5 = f, R6 = g
add.w #000a,R5
jmp EXIT

ELSE:
sub.w R6, R4
sub.w #000a,R5

EXIT:
...
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
.bss i,2 ; 		 		;int i
mov.w #0,&i ;			   ;i=0
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

```Assembly
.bss i,2 ;
mov.w #99,&i ;
fl_loop: call #do_dot ;
call #delay ;
call #do_dash ;
call #delay ;
dec.w &i ;
jnz fl_ck ;
for_done: ;
```
