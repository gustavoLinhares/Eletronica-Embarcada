1. Quais as diferenças entre os barramentos de dados e de endereços?
(a) O Barramento de Dados (Data Bus) transmite dados entre as unidades. Portanto, um microprocessador de 8 bits requer um barramento de dados de 8 linhas para transmitir dados de 8 bits em paralelo. Semelhantemente, um microprocessador de 64 bits necessita de um barramento de dados de 64 linhas para transmitir dados de 64 bits em paralelo. Se o barramento de dados para um microprocessador de 64 bits fosse formado por 8 linhas, seriam necessárias oito transmissões sucessivas, tornando mais lento o sistema. O Barramento de Dados é bi-direcional, isto é, pode transmitir em ambas as direções.
(b) O Barramento de Endereço (Address Bus) é usado para selecionar a origem ou destino de sinais transmitidos num dos outros barramentos ou numa de suas linhas. Ele conduz endereços. Uma função típica do Barramento de Endereço é selecionar um registrador num dos dispositivos do sistema que é usado como a fonte ou o destino do dado. O Barramento de Endereço do nosso computador padrão tem 16 linhas e pode endereçar 216 (64 K) dispositivos.

2. Quais são as diferenças entre as memórias RAM e ROM?
There is one major difference between a read-only memory (ROM) and a random-access memory (RAM) chip: ROM can hold data without power and RAM cannot. Essentially, ROM is meant for permanent storage, and RAM is for temporary storage.
A maior diferença entre a memória ROM (read-only-memory) e a memória RAM (random-acess-memory) é que a ROM consegue guardar os dados sem energia e a RAM não consegue o mesmo. Inicialmente, a ROM é reservada para dados permanentes e a RAM para dados temporários.

3. Considere o código abaixo:

```C
#include <stdio.h>
int main(void)
{
	int i;
	printf("Insira um número inteiro: ");
	scanf("%d", &i);
	if(i%2)
		printf("%d eh impar.\n");
	else
		printf("%d eh par.\n");
	return 0;
}
```

Para este código, responda: (a) A variável `i` é armazenada na memória RAM ou ROM? Por quê? (b) O programa compilado a partir deste código é armazenado na memória RAM ou ROM? Por quê?
(a) - O valor 'i' é armazenado na memória RAM, pois depois de impresso na tela ele não é mais necessario (valor temporário)
(b) - Na memória ROM, assim que o progama for compilado um arquivo (.exe) será guardado de forma permanente pois ele será um dado não volátil.

4. Quais são as diferenças, vantagens e desvantagens das arquiteturas Harvard e Von Neumann?
Arquitetura de von Neumann - Na arquitetura Von-Neumann, os barramentos de dados e endereços são compartilhados entre memórias de programas e memórias de dados na comunicação com a CPU. Nesse tipo de arquitetura, quando a CPU está acessando a memória de programa não pode acessar a memória de dados, porque usa os mesmos barramentos para as duas memórias.
Arquitetura Harvard - É uma arquitetura de computador que se distingue das outras por possuir duas memórias diferentes e independentes em termos de barramento e ligação ao processador. É utilizada nos microcontroladores PIC, tem como principal característica acessar a memória de dados separadamente da memória de programa.

5. Considere a variável inteira `i`, armazenando o valor `0x8051ABCD`. Se `i` é armazenada na memória a partir do endereço `0x0200`, como ficam este byte e os seguintes, considerando que a memória é: (a) Little-endian; (b) Big-endian.
(a) - little Endian:0x200 - CD
					0x202 - AB
					0x204 - 51
					0x206 - 80
						
(b) - Big Endian: 	0x200 - 80
					0x202 - 51
					0x204 - AB
					0x206 - CD

6. Sabendo que o processador do MSP430 tem registradores de 16 bits, como ele soma duas variáveis de 32 bits?
Deve ser feito uma lógica para armazenar o resultado em dois registradores diferentes.