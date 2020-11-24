//Victor Antonio Silva de Queiroga
//RA: 22117054-1

//CC7261 SISTEMAS DISTRIBUIDOS - Trabalho de Curso: Dígitos verificadores CPF e CNPJ


#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <chrono> //biblioteca que lida com tempo: Durações, Pontos de tempo e Relógios. Neste algoritimo será usado para durações.
#include <thread> //biblioteca para executar instruções simultâneamente. 



//definição namespace
using namespace std;
using namespace chrono; 

//variaveis
char arquivo[20]; //variável char para fazer leitura do arquivo

//vetores
int cpf[900000][20]; //vetor na forma de matriz que armazena os cpfs 
int cnpj[900000][20]; //vetor na forma de matriz que armazena os cnpjs 

//contadores
int qtdCpf = 0; //contador dos cpfs lidos 
int qtdCnpj = 0; //contador dos cnpjs lidos

//funções
void validaCpf(); //essa função vai calcular o digito verificador para o cpf
void validaCnpj(); //essa função vai calcular o digito verificador para o cnpj
void orquestrador(); //essa função serve para apresentar os resultados de forma organizada



//essa função vai calcular o digito verificador para o cpf
void validaCpf() {
	
	for (int i = 0; i < qtdCpf; i++) {

		int v1 = 0; // variavel para armazenar o primeiro digito
		int v2 = 0; // variavel para armazenar o segundo digito

		//calculo primeiro digito
		v1 = (cpf[i][0]*10)+(cpf[i][1]*9)+(cpf[i][2]*8)+(cpf[i][3]*7)+(cpf[i][4]*6)+(cpf[i][5]*5)+(cpf[i][6]*4)+(cpf[i][7]*3)+(cpf[i][8]*2);
        v1 = v1%11;
        v1 = 11 - v1;
        if(v1 >=10){
            v1 = 0;
        }

		//calculo segundo digito
        v2 = (cpf[i][0]*11)+(cpf[i][1]*10)+(cpf[i][2]*9)+(cpf[i][3]*8)+(cpf[i][4]*7)+(cpf[i][5]*6)+(cpf[i][6]*5)+(cpf[i][7]*4)+(cpf[i][8]*3)+(v1*2);
        v2 = v2%11;
        v2 = 11 - v2;
        if(v2 >=10){
            v2=0;
        }
		
		//acrescentando os digitos verificadores no final do cpf
		cpf[i][9] = v1;
		cpf[i][10] = v2;
	}
}


//essa função vai calcular o digito verificador para o cnpj
void validaCnpj() {
	for (int i = 0; i < qtdCnpj; i++) {
		
		int v1 = 0; // variavel para armazenar o primeiro digito
		int v2 = 0; // variavel para armazenar o segundo digito

		//calculo primeiro digito
		v1 = (6 * cnpj[i][0]) + (7 * cnpj[i][1]) + (8 * cnpj[i][2]) + (9 * cnpj[i][3]);
		v1 = v1 + ((2 * cnpj[i][4]) + (3 * cnpj[i][5]) + (4 * cnpj[i][6]) + (5 * cnpj[i][7]));
		v1 = v1 + ((6 * cnpj[i][8]) + (7 * cnpj[i][9]) + (8 * cnpj[i][10]) + (9 * cnpj[i][11]));
		v1 = v1 % 11;
		
		//calculo segundo digito
		v2 = (5 * cnpj[i][0]) + (6 * cnpj[i][1]) + (7 * cnpj[i][2]) + (8 * cnpj[i][3]);
		v2 = v2 + ((9 * cnpj[i][4]) + (2 * cnpj[i][5]) + (3 * cnpj[i][6]) + (4 * cnpj[i][7]));
		v2 = v2 + ((5 * cnpj[i][8]) + (6 * cnpj[i][9]) + (7 * cnpj[i][10]) + (8 * cnpj[i][11] + (9 * v1)));
		v2 = v2 % 11;
		
		
		//tratamento para caso o digito seja maior ou igual a 10		
		if(v1 >= 10){
			v1 = 0;
		}
		
		if(v2 >= 10){
			v2 = 0;
		}

		//acrescentando os digitos verificadores no final do cnpj
		cnpj[i][12] = v1;
		cnpj[i][13] = v2;
	}
}

void orquestrador() {

	//Abertura e tratamento da base
	FILE* arq = fopen("base900K.txt", "r");	//Ponteiro que aponta para o Arquivo

	//For que controla quantas linhas serão lidas
	for (int i = 0; i < 900000; i++) {

		fscanf(arq, "%s", arquivo);		

		int tamanho = 0;	//variavel que guarda o tamanho da linha
		

		//While que faz a leitura
		while (arquivo[tamanho] != '\0') {
			tamanho++;			
					
		}

		

		//Se o tamanho for maior que 11 então é CNPJ		
		if (tamanho > 11) {
			for (int j = 0; j <= 12; j++) {
				cnpj[qtdCnpj][j] = arquivo[j] - '0'; //transforma string em int
			}
			qtdCnpj++;
		}

		//caso contrário é CPF
		else {
			for (int j = 0; j < 9; j++) {
				cpf[qtdCpf][j] = arquivo[j] - '0'; //transforma string em int
			}
			qtdCpf++;
		}
	}
	fclose(arq); // fecha o arquivo
	


	//threads

	auto inicio1 = std::chrono::steady_clock::now();
	std::thread cpfcalc(validaCpf); // thread que calcula o cpf
	cpfcalc.join();
	auto fim1 = steady_clock::now();
	milliseconds t1 = duration_cast<milliseconds>(fim1 - inicio1); //tempo de execução em ms

	auto inicio2 = std::chrono::steady_clock::now();
	std::thread cnpjcalc(validaCnpj); // thread que calcula o cnpj
	cnpjcalc.join();
	auto fim2 = steady_clock::now();
	milliseconds t2 = duration_cast<milliseconds>(fim2 - inicio2); //tempo de execução em ms

	/*
	//Print dos cpfs e cnpjs comentados para não atrapalhar a execução, para imprimir os cpfs e cnpjs com seus 
	//respectivos digitos verificadores, basta descomentar essa parte do código
	
	//imprime os vetores
	for(int i = 0; i<qtdCpf;i++){
		cout<<"cpf ";
	  	for(int j = 0; j <=10;j++){
			cout << cpf[i][j];
		}
		cout<<endl;
	}

	for (int i = 0; i < qtdCnpj; i++) {
	  	cout << "cnpj ";
	  	for (int j = 0; j <= 13; j++) {
			cout << cnpj[i][j];
	  	}
	  	cout << endl;
	}
	*/
   
	cout << "Tempo de calculo dos cpf:" << t1.count() << "ms" << endl; //imprime o tempo de execução da thread que calculou o cpf
	cout << "Tempo de calculo dos cnpj:" << t2.count() << "ms" << endl; //imprime o tempo de execução da thread que calculou o cnpj

	cout << "Quantidade de cpf's verificados:" << qtdCpf << endl; //imprime a quantidade de cpfs verificados
	cout << "Quantidade de cnpj's verificados:" << qtdCnpj << endl; //imprime a quantidade de cnpjs verificados

}



int main() {

	orquestrador();

	return 0;
}

