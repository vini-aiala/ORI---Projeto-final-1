#pragma warning(disable : 4996)
#pragma warning(disable : 6031)
#pragma warning(disable : 6305)
#pragma warning(disable : 6053)
#pragma warning(disable : 6054)
/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Organização de Recuperação da Informação
 * Prof. Tiago A. de Almeida
 *
 * Trabalho 01
 *
 * RA: 759544
 * Aluno: Vinícius Pereira Aiala
 * ========================================================================== */

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Tamanho dos campos dos registros */
#define TAM_PRIMARY_KEY 11
#define TAM_NOME 		31
#define TAM_GENERO 		2
#define TAM_NASCIMENTO 	11
#define TAM_CELULAR 	16
#define TAM_VEICULO 	31
#define TAM_PLACA 		9
#define TAM_DATA 		9
#define TAM_HORA 		6
#define TAM_TRAJETO 	121
#define TAM_VALOR 		7
#define TAM_VAGAS 		2
#define TAM_REGISTRO 	256
#define MAX_REGISTROS 	1000
#define MAX_TRAJETOS 	30
#define TAM_ARQUIVO 	(MAX_REGISTROS * TAM_REGISTRO + 1)

/* Saídas do usuário */
#define OPCAO_INVALIDA 				"Opcao invalida!\n"
#define MEMORIA_INSUFICIENTE 		"Memoria insuficiente!"
#define REGISTRO_N_ENCONTRADO 		"Registro(s) nao encontrado!\n"
#define CAMPO_INVALIDO 				"Campo invalido! Informe novamente.\n"
#define ERRO_PK_REPETIDA 			"ERRO: Ja existe um registro com a chave primaria: %s.\n"
#define ARQUIVO_VAZIO 				"Arquivo vazio!\n"
#define INICIO_BUSCA 				"**********************BUSCAR**********************\n"
#define INICIO_LISTAGEM 			"**********************LISTAR**********************\n"
#define INICIO_ALTERACAO 			"**********************ALTERAR*********************\n"
#define INICIO_EXCLUSAO 			"**********************EXCLUIR*********************\n"
#define INICIO_ARQUIVO 				"**********************ARQUIVO*********************\n"
#define INICIO_ARQUIVO_SECUNDARIO 	"*****************ARQUIVO SECUNDARIO****************\n"
#define SUCESSO 					"OPERACAO REALIZADA COM SUCESSO!\n"
#define FALHA 						"FALHA AO REALIZAR OPERACAO!\n"

/* Registro da Carona */
typedef struct
{
	char pk[TAM_PRIMARY_KEY];
	char nome[TAM_NOME];
	char genero[TAM_GENERO];
	char nascimento[TAM_NASCIMENTO]; 	/* DD/MM/AAAA */
	char celular[TAM_CELULAR]; 			/*(99) 99999-9999 */
	char veiculo[TAM_VEICULO];
	char placa[TAM_PLACA];				/* AAA-9999 */
	char trajeto[TAM_TRAJETO];
	char data[TAM_DATA];   				/* DD/MM/AA, ex: 24/09/19 */
	char hora[TAM_HORA];   				/* HH:MM, ex: 07:30 */
	char valor[TAM_VALOR]; 				/* 999.99, ex: 004.95 */
	char vagas[TAM_VAGAS];
} Carona;

/*----- Registros dos Índices -----*/

/* Struct para índice Primário */
typedef struct primary_index
{
	char pk[TAM_PRIMARY_KEY];
	int rrn;
} Ip;

/* Struct para índice secundário de nome */
typedef struct secundary_index
{
	char pk[TAM_PRIMARY_KEY];
	char nome[TAM_NOME];
} Is;

/* Struct para índice secundário de data */
typedef struct secundary_index_of_date
{
	char pk[TAM_PRIMARY_KEY];
	char data[TAM_DATA];
} Isd;

/* Struct para índice secundário de hora */
typedef struct secundary_index_of_time
{
	char pk[TAM_PRIMARY_KEY];
	char hora[TAM_HORA];
} Ist;

/* Lista ligada usada no índice com lista invertida */
typedef struct linked_list
{
	char pk[TAM_PRIMARY_KEY];
	struct linked_list *prox;
} ll;

/* Struct para lista invertida de trajetos*/
typedef struct reverse_index
{
	char trajeto[TAM_TRAJETO];
	ll *lista;
} Ir;

/* GLOBAL: ARQUIVO DE DADOS */
char ARQUIVO[TAM_ARQUIVO];

/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

/* Recebe do usuário uma string simulando o arquivo completo e retorna o número
 * de registros. */
int carregar_arquivo();

/* Exibe a Carona */
int exibir_registro(int rrn);

/* Recupera do arquivo o registro com o rrn informado
 *  e retorna os dados na struct Carona */
Carona recuperar_registro(int rrn);

/* (Re)faz o índice respectivo */
int criar_indices(int nregistros, int ntraj, Ip* iprimary, Is* idriver, Ir* iroute, Isd* idate, Ist* itime);

/* Realiza os scanfs na struct Carona*/
void ler_entrada(char *registro, Carona *novo);

/* Gera a chave para determinada struct de carona */ 
void gerarChave(Carona *novo);

/* Rotina para impressao de indice secundario */
void imprimirSecundario(Is *idriver, Ir *iroute, Isd *idate, Ist *itime, int nregistros, int ntraj);

//Insere Uma nova carona no arquivo, retorna rnn
int inserir_arquivo(Carona* novo, int nregistros);

//Cadastra uma nova carona
void cadastrar(int* nregistros, int* ntrajetos, Ip* chaves, Is* idriver, Ir* iroute, Isd* idate, Ist* itime);

//Lista todos as caronas em uma lista
void listar_trajetos(ll* lista, Ip* chaves, int nregistros);

void imprimirCarona(Carona car);

void atualiza_arquivo(Carona registro, int rrn);

int alterar(Ip* chaves, int nregistros);

int remover(Ip* chaves, Is* idriver, Ir* iroute, Isd* idate, Ist* itime, int* nregistros);

void buscar(int nregistros, int ntrajetos, Ip* chaves, Is* idriver, Ir* iroute, Isd* idate, Ist* itime);

void listar(int nregistros, int ntrajetos, Ip* chaves, Is* idriver, Ir* iroute, Isd* idate, Ist* itime);

void liberar(int* nregistros);

int comparaNome(Is* primeiro, Is* segundo);

int comparaData(Isd* primeiro, Isd* segundo);

int comparaHora(Ist* primeiro, Ist* segundo);

int comparaTraj(Ir* primeiro, Ir* segundo);

int ordenaDH(Carona* primeiro, Carona* segundo);

int comparastrData(char* primeiro, Isd* segundo);

int comparastrTraj(char* primeiro, Ir* segundo);

void finaliza(Ip* iprimary, Ir* iroute, Is* idriver, Isd* idate, Ist* itime, int nregistros, int ntraj);
/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
int main()
{
	/* Verifica se há arquivo de dados */
	int carregarArquivo = 0, nregistros = 0, ntraj = 0;
	scanf("%d%*c", &carregarArquivo); /* 1 (sim) | 0 (nao) */
	if (carregarArquivo)
		nregistros = carregar_arquivo();


	/* === ALOCAÇÃO E CRIAÇÃO DOS ÍNDICES === */
	
	/* Índice primário */
	Ip *iprimary = (Ip *)malloc(MAX_REGISTROS * sizeof(Ip));
	if (!iprimary)
	{
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	
	Is *idriver = (Is *)malloc(MAX_REGISTROS * sizeof(Ip));
	if (!idriver)
	{
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}

	Ir *iroute = (Ir *)malloc(MAX_TRAJETOS * sizeof(Ir));
	if (!iroute)
	{
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}

	Isd *idate = (Isd *)malloc(MAX_REGISTROS * sizeof(Isd));
	if (!idate)
	{
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}

	Ist *itime = (Ist *)malloc(MAX_REGISTROS * sizeof(Ist));
	if (!itime)
	{
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	ntraj = criar_indices(nregistros, ntraj, iprimary, idriver, iroute, idate, itime);

	/* === ROTINA DE EXECUÇÃO DO PROGRAMA === */
	int opcao = 0;
	while (1)
	{
		scanf("%d%*c", &opcao);
		switch (opcao)
		{
		case 1:
			/* cadastrar */
			cadastrar(&nregistros, &ntraj, iprimary, idriver, iroute, idate, itime);
			break;

		case 2:
			/* alterar */ 
			printf(INICIO_ALTERACAO);

			if(alterar(iprimary, nregistros))
				printf(SUCESSO);
			else
				printf(FALHA);

			break;

		case 3:
			/* excluir */
			printf(INICIO_EXCLUSAO);

			if(remover(iprimary, idriver, iroute, idate, itime, &nregistros))
				printf(SUCESSO);
			else
				printf(FALHA);

			break;

		case 4:
			/* buscar */
			printf(INICIO_BUSCA);

			buscar(nregistros, ntraj, iprimary, idriver, iroute, idate, itime);

			break;

		case 5:
			/* listar */
			printf(INICIO_LISTAGEM);

			listar(nregistros, ntraj, iprimary, idriver, iroute, idate, itime);

			break;

		case 6:
			/*	libera espaço */
			liberar(&nregistros);
			ntraj = criar_indices(nregistros, ntraj, iprimary, idriver, iroute, idate, itime);

			break;

		case 7:
			/* imprime arquivo de dados */
			printf(INICIO_ARQUIVO);

			if (nregistros == 0)
				printf(ARQUIVO_VAZIO);
			else
				printf("%s\n", ARQUIVO);

			break;

		case 8:
			/*	imprime os índices secundários */
			imprimirSecundario(idriver, iroute, idate, itime, nregistros, ntraj);
			break;

		case 9:
			/*	liberar memória e finalizar o programa */
			finaliza(iprimary, iroute, idriver, idate, itime, nregistros, ntraj);
			return (0);

			break;

		default:
			printf(OPCAO_INVALIDA);
			break;
		}
	}
	return 0;
}

/* ==========================================================================
 * =========================== CÓDIGO DAS FUNÇÕES ===========================
 * ========================================================================== */

/* Exibe a Carona */
int exibir_registro(int rrn)
{
	if (rrn < 0)
		return 0;

	Carona j = recuperar_registro(rrn);
	char *traj, trajeto[TAM_TRAJETO];

	printf("%s\n", j.pk);
	printf("%s\n", j.nome);
	printf("%s\n", j.genero);
	printf("%s\n", j.nascimento);
	printf("%s\n", j.celular);
	printf("%s\n", j.veiculo);
	printf("%s\n", j.placa);
	printf("%s\n", j.data);
	printf("%s\n", j.hora);
	printf("%s\n", j.valor);
	printf("%s\n", j.vagas);

	strcpy(trajeto, j.trajeto);

	traj = strtok(trajeto, "|");

	while (traj != NULL)
	{
		printf("%s", traj);
		traj = strtok(NULL, "|");
		if (traj != NULL)
		{
			printf(", ");
		}
	}

	printf("\n");

	return 1;
}

/* carrega dados do arquivo e retorna
 * a quantidade de registros lidos */
int carregar_arquivo()
{
	scanf("%[^\n]%*c", ARQUIVO);
	return strlen(ARQUIVO) / TAM_REGISTRO;
}


/* Recupera do arquivo o registro com o rrn
 * informado e retorna os dados na struct Carona */
Carona recuperar_registro(int rrn)
{
	char temp[257], *p;
	strncpy(temp, (ARQUIVO + rrn), TAM_REGISTRO);
	temp[256] = '\0';
	Carona j;

	p = strtok(temp, "@");
	strcpy(j.nome, p);
	p = strtok(NULL, "@");
	strcpy(j.genero, p);
	p = strtok(NULL, "@");
	strcpy(j.nascimento, p);
	p = strtok(NULL, "@");
	strcpy(j.celular, p);
	p = strtok(NULL, "@");
	strcpy(j.veiculo, p);
	p = strtok(NULL, "@");
	strcpy(j.placa, p);
	p = strtok(NULL, "@");
	strcpy(j.trajeto, p);
	p = strtok(NULL, "@");
	strcpy(j.data, p);
	p = strtok(NULL, "@");
	strcpy(j.hora, p);
	p = strtok(NULL, "@");
	strcpy(j.valor, p);
	p = strtok(NULL, "@");
	strcpy(j.vagas, p);

	gerarChave(&j);

	return j;
}

/* ==========================================================================
 * ============================= CRIAR ÍNDICES ==============================
 * ========================================================================== */

/* Imprimir indices secundarios */
void imprimirSecundario(Is *idriver, Ir *iroute, Isd *idate, Ist *itime, int nregistros, int ntraj)
{
	int opPrint = 0;
	ll *aux;
	printf(INICIO_ARQUIVO_SECUNDARIO);
	scanf("%d", &opPrint);
	if (!nregistros)
		printf(ARQUIVO_VAZIO);
	switch (opPrint)
	{
	case 1:
		for (int i = 0; i < nregistros; i++)
		{
			printf("%s %s\n", idriver[i].pk, idriver[i].nome);
		}
		break;
	case 2:
		for (int i = 0; i < nregistros; i++)
		{
			printf("%s %s\n", idate[i].pk, idate[i].data);
		}
		break;
	case 3:
		for (int i = 0; i < nregistros; i++)
		{
			printf("%s %s\n", itime[i].pk, itime[i].hora);
		}
		break;
	case 4:
		for (int i = 0; i < ntraj; i++)
		{
			if (iroute[i].lista != NULL)
			{
				printf("%s", iroute[i].trajeto);
				aux = iroute[i].lista;
				while (aux != NULL)
				{
					printf(" %s", aux->pk);
					aux = aux->prox;
				}
				printf("\n");
			}
		}
		break;
	default:
		printf(OPCAO_INVALIDA);
		break;
	}
}

/* (Re)faz o índice respectivo */
int criar_indices(int nregistros, int ntraj, Ip* iprimary, Is* idriver, Ir* iroute, Isd* idate, Ist* itime)
{
	//Verifica se nn tá vazio
	if (nregistros == 0)
		return 0;
	int ntrajetos = 0;

	ll* no;
	ll* aux;
	for (int i = 0; i < ntraj; i++)
	{
		no = iroute[i].lista;
		while (no)
		{
			//printf("%s\n", no->pk);
			aux = no;
			no = no->prox;
			free(aux);
		}
		iroute[i].lista = NULL;
	}


	for (int i = 0; i < nregistros; i++)
	{
		//Lê do arquivo
		int rrn = i * TAM_REGISTRO;
		Carona novo = recuperar_registro(rrn);

		//Inserção nos indices
		strcpy(iprimary[i].pk, novo.pk);
		iprimary[i].rrn = rrn;
		strcpy(idriver[i].nome, novo.nome);
		strcpy(idriver[i].pk, novo.pk);
		strcpy(idate[i].data, novo.data);
		strcpy(idate[i].pk, novo.pk);
		strcpy(itime[i].hora, novo.hora);
		strcpy(itime[i].pk, novo.pk);

		//inserir iroute em listas invertidas
		char* p = strtok(novo.trajeto, "|");
		while (p != NULL)
		{
			Ir* existe = bsearch(p, iroute, ntrajetos, sizeof(Ir), (int(*)(const void*, const void*)) comparastrTraj);
			if (!existe)
			{
				//Caso a chave nao exista, insere o novo trajeto na lista
				sprintf(iroute[ntrajetos].trajeto, "%s", p);
				ntrajetos++;
				qsort(iroute, ntrajetos, sizeof(Ir), (int(*)(const void*, const void*)) comparastrTraj);
			}

			Ir* indice = bsearch(p, iroute, ntrajetos, sizeof(Ir), (int(*)(const void*, const void*)) comparastrTraj);
			//Insere na list
			ll* no = indice->lista;
			ll* ant = NULL;
			ll* node = (ll*)malloc(sizeof(ll));

			if (node == NULL)
			{
				perror(MEMORIA_INSUFICIENTE);
				exit(1);
			}

			strcpy(node->pk, novo.pk);

			if (no) {
				while (strcmp(no->pk, novo.pk) < 0 && no)
				{
					ant = no;
					no = no->prox;
					if (!no)
						break;
				}
			}

			//Se deve ser insrido antes de um no, deve definir que este é o proximo
			if (no)
			{
				node->prox = no;
			}
			else {
				node->prox = NULL;
			}

			//Se não for inserir na primeira posição, deve definir a nova disposição
			if (ant)
			{
				ant->prox = node;
			}
			else {
				indice->lista = node;
			}

			//Divide para o próximo
			p = strtok(NULL, "|");
		}
		//Ordenar indices
		qsort(iprimary, i + 1, sizeof(Ip), (int(*)(const void*, const void*)) strcmp);
		qsort(idriver, i + 1, sizeof(Is), (int(*)(const void*, const void*)) comparaNome);
		qsort(idate, i + 1, sizeof(Isd), (int(*)(const void*, const void*)) comparaData);
		qsort(itime, i + 1, sizeof(Ist), (int(*)(const void*, const void*)) comparaHora);
	}

	//retorna numero de trajetos
	return ntrajetos;
}

/* Realiza os scanfs na struct Carona*/
void ler_entrada(char *registro, Carona *novo)
{
	char buffer[121];

	fgets(buffer, 121, stdin);
	sscanf(buffer, "%30[^\n]s", novo->nome);

	fgets(buffer, 121, stdin);
	sscanf(buffer, "%1[^\n]s", novo->genero);

	fgets(buffer, 121, stdin);
	sscanf(buffer, "%10[^\n]s", novo->nascimento);

	fgets(buffer, 121, stdin);
	sscanf(buffer, "%15[^\n]s", novo->celular);

	fgets(buffer, 121, stdin);
	sscanf(buffer, "%30[^\n]s", novo->veiculo);

	fgets(buffer, 121, stdin);
	sscanf(buffer, "%8[^\n]s", novo->placa);

	fgets(buffer, 121, stdin);
	sscanf(buffer, "%120[^\n]s", novo->trajeto);

	fgets(buffer, 121, stdin);
	sscanf(buffer, "%8[^\n]s", novo->data);

	fgets(buffer, 121, stdin);
	sscanf(buffer, "%5[^\n]s", novo->hora);

	fgets(buffer, 121, stdin);
	sscanf(buffer, "%7[^\n]s", novo->valor);

	fgets(buffer, 121, stdin);
	sscanf(buffer, "%1[^\n]s", novo->vagas);
}

/* Gera a chave para determinada struct de carona */ 
void gerarChave(Carona *novo)
{
	novo->pk[0] = novo->nome[0];
	novo->pk[1] = novo->placa[0];
	novo->pk[2] = novo->placa[1];
	novo->pk[3] = novo->placa[2];
	novo->pk[4] = novo->data[0];
	novo->pk[5] = novo->data[1];
	novo->pk[6] = novo->data[3];
	novo->pk[7] = novo->data[4];
	novo->pk[8] = novo->hora[0];
	novo->pk[9] = novo->hora[1];
	novo->pk[10] = '\0';
}

void cadastrar(int* nregistros, int* ntrajetos, Ip* chaves, Is *idriver, Ir *iroute, Isd *idate, Ist *itime)
{
	//Aloca nova carona
	Carona* novo = (Carona*) malloc(sizeof(Carona));
	if (!novo)
	{
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}

	//Lê a entrada e insere nos campos corretos
	ler_entrada('\0', novo);

	//Gera a chave e insere no campo pk
	gerarChave(novo);

	//Busca se nn existe chave repetida
	Ip* existe = bsearch(novo->pk, chaves, *nregistros, sizeof(Ip), (int(*)(const void*, const void*)) strcmp);
	if(existe)
	{
		if (existe->rrn != -1)
		{
			printf(ERRO_PK_REPETIDA, novo->pk);
			free(novo);
			return;
		}
	}

	//Escreve no arquivo
	int rrn = inserir_arquivo(novo, *nregistros);

	//Inserção nos indices
	strcpy(chaves[*nregistros].pk, novo->pk);
	chaves[*nregistros].rrn = rrn;
	strcpy(idriver[*nregistros].nome, novo->nome);
	strcpy(idriver[*nregistros].pk, novo->pk);
	strcpy(idate[*nregistros].data, novo->data);
	strcpy(idate[*nregistros].pk, novo->pk);
	strcpy(itime[*nregistros].hora, novo->hora);
	strcpy(itime[*nregistros].pk, novo->pk);
	//inserir iroute em listas invertidas
	char* p = strtok(novo->trajeto, "|");
	while (p)
	{
		Ir* existe = bsearch(p, iroute, *ntrajetos, sizeof(Ir), (int(*)(const void*, const void*)) comparastrTraj);
		if (!existe)
		{
			//Caso a chave nao exista, insere o novo trajeto na lista
			sprintf(iroute[*ntrajetos].trajeto, "%s", p);
			(*ntrajetos)++;
			qsort(iroute, *ntrajetos, sizeof(Ir), (int(*)(const void*, const void*)) comparastrTraj);
		}

		Ir* indice = bsearch(p, iroute, *ntrajetos, sizeof(Ir), (int(*)(const void*, const void*)) comparastrTraj);
		//Insere na list
		ll* no = indice->lista;
		ll* ant = NULL;
		ll* node = (ll*)malloc(sizeof(ll));

		if (node == NULL)
		{
			perror(MEMORIA_INSUFICIENTE);
			exit(1);
		}

		strcpy(node->pk, novo->pk);

		if (no) {
			while (strcmp(no->pk, novo->pk) < 0 && no)
			{
				ant = no;
				no = no->prox;
				if (!no)
					break;
			}
		}

		//Se deve ser insrido antes de um no, deve definir que este é o proximo
		if (no)
		{
			node->prox = no;
		}
		else {
			node->prox = NULL;
		}

		//Se não for inserir na primeira posição, deve definir a nova disposição
		if (ant)
		{
			ant->prox = node;
		}
		else {
			indice->lista = node;
		}

		//Divide para o próximo
		p = strtok(NULL, "|");
	}
	
	//Acrescenta o número de registros e desaloca a struct auxiliar
	(*nregistros)++;

	//Ordenar indices
	qsort(chaves, *nregistros, sizeof(Ip), (int(*)(const void*, const void*)) strcmp);
	qsort(idriver, *nregistros, sizeof(Is), (int(*)(const void*, const void*)) comparaNome);
	qsort(idate, *nregistros, sizeof(Isd), (int(*)(const void*, const void*)) comparaData);
	qsort(itime, *nregistros, sizeof(Ist), (int(*)(const void*, const void*)) comparaHora);
	
	

	free(novo);
}

int inserir_arquivo(Carona* novo, int nregistros)
{
	//Ordem: nome genero nascimento celular veiculo placa trajeto data hora valor vagas
	//Nome, modelo e trajeto variável
	char* inicio = ARQUIVO + (nregistros * 256);
	int n_escritos;
	n_escritos = sprintf(inicio, "%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@", novo->nome, novo->genero, novo->nascimento, novo->celular, novo->veiculo, novo->placa, novo->trajeto, novo->data, novo->hora, novo->valor, novo->vagas);
	for(int i = n_escritos; i < 256; i++)
		inicio[i] = '#';
	return nregistros * 256;
}

int alterar(Ip* chaves, int nregistros)
{
	char buffer[TAM_TRAJETO];
	char corte[3];
	char chave[TAM_PRIMARY_KEY];
	
	fgets(buffer, 121, stdin);
	sscanf(buffer, "%10[^\n]s", chave);

	Ip* pk_encontrado = bsearch(chave, chaves, nregistros, sizeof(Ip), (int(*)(const void*, const void*)) strcmp);

	if (pk_encontrado == NULL) {
		printf(REGISTRO_N_ENCONTRADO);
		return 0;
	}

	if (pk_encontrado->rrn == -1) {
		printf(REGISTRO_N_ENCONTRADO);
		return 0;
	}

	do {
		fgets(buffer, 121, stdin);
		sscanf(buffer, "%2[^\n]s", corte);
		//printf(CAMPO_INVALIDO);
	} while (strlen(corte) != 1 || corte[0] < '0' || corte[0] > '9');

	Carona atualizado = recuperar_registro(pk_encontrado->rrn);
	strcpy(atualizado.vagas, corte);
	atualiza_arquivo(atualizado, pk_encontrado->rrn);

	return 1;
}

int remover(Ip* chaves, Is* idriver, Ir* iroute, Isd* idate, Ist* itime, int* nregistros)
{
	char buffer[TAM_TRAJETO];
	char codigo[TAM_PRIMARY_KEY];

	fgets(buffer, 121, stdin);
	sscanf(buffer, "%10[^\n]s", codigo);

	Ip* pk_encontrado = bsearch(codigo, chaves, *nregistros, sizeof(Ip), (int(*)(const void*, const void*)) strcmp);

	if (pk_encontrado == NULL) {
		printf(REGISTRO_N_ENCONTRADO);
		return 0;
	}

	ARQUIVO[pk_encontrado->rrn] = '*';
	ARQUIVO[pk_encontrado->rrn + 1] = '|';

	pk_encontrado->rrn = -1;
	return 1;
}

void buscar(int nregistros, int ntrajetos, Ip* chaves, Is* idriver, Ir* iroute, Isd* idate, Ist* itime)
{
	char opcaostr[2];
	char buffer[TAM_TRAJETO];
	char codigo[TAM_PRIMARY_KEY];
	char data[TAM_DATA];
	char local[TAM_TRAJETO];
	int opcao;
	int imprimiu = 0;

	
	fgets(buffer, 121, stdin);
	sscanf(buffer, "%1[^\n]s", opcaostr);
	opcao = atoi(opcaostr);
	//printf("OPCAO BUSCA = %d\n", opcao);

	switch (opcao)
	{
	case 1:
	{
		//Inserção do elemento
		fgets(buffer, 121, stdin);
		sscanf(buffer, "%10[^\n]s", codigo);

		Ip* pk_encontrado = bsearch(codigo, chaves, nregistros, sizeof(Ip), (int(*)(const void*, const void*)) strcmp);
		if (pk_encontrado)
		{
			if(pk_encontrado->rrn != -1)
			{
				Carona car = recuperar_registro(pk_encontrado->rrn);
				imprimirCarona(car);
				imprimiu = 1;
			}
		}
		break;
	}
	case 2:
	{
		//Inserção do elemento
		fgets(buffer, 121, stdin);
		sscanf(buffer, "%8[^\n]s", data);

		Isd* data_encontrada = bsearch(data, idate, nregistros, sizeof(Isd), (int(*)(const void*, const void*)) comparastrData);
		if (data_encontrada)
		{
			while ((data_encontrada - sizeof(Isd))->data == data)
			{
				data_encontrada -= sizeof(Isd);
			}
			do {
				Ip* pk_encontrado = bsearch(data_encontrada->pk, chaves, nregistros, sizeof(Ip), (int(*)(const void*, const void*)) strcmp);
				if (pk_encontrado)
				{
					if (pk_encontrado->rrn != -1)
					{
						Carona car = recuperar_registro(pk_encontrado->rrn);
						imprimirCarona(car);
						imprimiu = 1;
					}
				}
				data_encontrada += sizeof(Isd);
			} while (data_encontrada->data == data);
		}

		break;
	}
	case 3:
	{
		//Inserção do elemento
		fgets(buffer, 121, stdin);
		sscanf(buffer, "%120[^\n]s", local);

		Ir* local_encontrado = bsearch(local, iroute, ntrajetos, sizeof(Ir), (int(*)(const void*, const void*)) strcmp);
		if (local_encontrado)
		{
			listar_trajetos(local_encontrado->lista, chaves, nregistros);
			imprimiu = 1;
		}
		break;
	}
	case 4:
	{
		fgets(buffer, 121, stdin);
		sscanf(buffer, "%120[^\n]s", local);
		fgets(buffer, 121, stdin);
		sscanf(buffer, "%8[^\n]s", data);

		Ir* local_encontrado = bsearch(local, iroute, ntrajetos, sizeof(Ir), (int(*)(const void*, const void*)) comparastrTraj);
		if(local_encontrado)
		{
			int flag = 1;
			ll* lista = local_encontrado->lista;
			while (lista && flag)
			{
				Ip* pk_encontrado = bsearch(lista->pk, chaves, nregistros, sizeof(Ip), (int(*)(const void*, const void*)) strcmp);
				if (pk_encontrado)
				{
					if (pk_encontrado->rrn != -1)
					{
						Carona car = recuperar_registro(pk_encontrado->rrn);
						if (!strcmp(car.data, data))
						{
							imprimirCarona(car);
							flag = 0;
							imprimiu = 1;
						}
					}
				}
				lista = lista->prox;
			}
		}
		break;
	}
	default:
		break;
	}

	if (!imprimiu)
		printf(REGISTRO_N_ENCONTRADO);
}

int comparaNome(Is* primeiro, Is* segundo)
{
	int result = strcmp(primeiro->nome, segundo->nome);
	if (result == 0)
		result = strcmp(primeiro->pk, segundo->pk);
	return result;
}

int comparaData(Isd* primeiro, Isd* segundo)
{
	char dia1[3], mes1[3], ano1[3], dia2[3], mes2[3], ano2[3];
	strncpy(ano1, primeiro->data + 6, 2);
	strncpy(mes1, primeiro->data + 3, 2);
	strncpy(dia1, primeiro->data, 2);
	strncpy(ano2, segundo->data + 6, 2);
	strncpy(mes2, segundo->data + 3, 2);
	strncpy(dia2, segundo->data, 2);
	int result = strncmp(ano1, ano2, 2);
	if (result == 0)
	{
		result = strncmp(mes1, mes2, 2);
		if (result == 0)
		{
			result = strncmp(dia1, dia2, 2);
			if(result == 0)
				result = strcmp(primeiro->pk, segundo->pk);
		}
	}
	return result;
}

int comparastrData(char* primeiro, Isd* segundo)
{
	char dia1[3], mes1[3], ano1[3], dia2[3], mes2[3], ano2[3];
	strncpy(ano1, primeiro + 6, 2);
	strncpy(mes1, primeiro + 3, 2);
	strncpy(dia1, primeiro, 2);
	strncpy(ano2, segundo->data + 6, 2);
	strncpy(mes2, segundo->data + 3, 2);
	strncpy(dia2, segundo->data, 2);
	int result = strncmp(ano1, ano2, 2);
	if (result == 0)
	{
		result = strncmp(mes1, mes2, 2);
		if (result == 0)
		{
			result = strncmp(dia1, dia2, 2);
		}
	}
	return result;
}

int comparastrTraj(char* primeiro, Ir* segundo)
{
	int result = strcmp(primeiro, segundo->trajeto);
	return result;
}

int comparaHora(Ist* primeiro, Ist* segundo)
{
	int result = strcmp(primeiro->hora, segundo->hora);
	if (result == 0)
		result = strcmp(primeiro->pk, segundo->pk);
	return result;
}

int comparaTraj(Ir* primeiro, Ir* segundo)
{
	int result = strcmp(primeiro->trajeto, segundo->trajeto);
	return result;
}

void listar_trajetos(ll* lista, Ip* chaves, int nregistros)
{
	int imprimiu = 0;
	while (lista)
	{
		Ip* pk_encontrado = bsearch(lista->pk, chaves, nregistros, sizeof(Ip), (int(*)(const void*, const void*)) strcmp);
		if (pk_encontrado)
		{
			if (pk_encontrado->rrn != -1)
			{
				Carona car = recuperar_registro(pk_encontrado->rrn);
				imprimirCarona(car);
				imprimiu = 1;
			}
		}
		lista = lista->prox;
		if (lista != NULL)
			printf("\n");
	}
	if (!imprimiu)
		printf(REGISTRO_N_ENCONTRADO);
}

void imprimirCarona(Carona car)
{
	//Ordem: chave, nome, genero, nasc, cel, mod, placa, data, hora, preço, vagas e trajeto separado por virgulas
	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", car.pk, car.nome, car.genero, car.nascimento, car.celular, car.veiculo, car.placa, car.data, car.hora, car.valor, car.vagas);
	char* p = strtok(car.trajeto, "|");
	while (p)
	{
		printf("%s", p);
		p = strtok(NULL, "|");
		if (p != NULL)
			printf(", ");
	}
	printf("\n");
}

void atualiza_arquivo(Carona registro, int rrn)
{
	char* inicio = ARQUIVO + rrn;
	int n_escritos;
	n_escritos = sprintf(inicio, "%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@", registro.nome, registro.genero, registro.nascimento, registro.celular, registro.veiculo, registro.placa, registro.trajeto, registro.data, registro.hora, registro.valor, registro.vagas);
	for (int i = n_escritos; i < 256; i++)
		inicio[i] = '#';
}

void listar(int nregistros, int ntrajetos, Ip* chaves, Is* idriver, Ir* iroute, Isd* idate, Ist* itime)
{
	char buffer[TAM_TRAJETO];
	char opcaostr[2];
	int opcao;
	fgets(buffer, 121, stdin);
	sscanf(buffer, "%1[^\n]s", opcaostr);
	opcao = atoi(opcaostr);
	//printf("OPCAO LISTAR = %d\n", opcao);

	switch (opcao)
	{
	case 1:
		if(nregistros > 0)
		{
			for (int i = 0; i < nregistros; i++)
			{
				if (chaves[i].rrn != -1)
				{
					Carona car = recuperar_registro(chaves[i].rrn);
					imprimirCarona(car);
					if (i + 1 < nregistros)
						printf("\n");
				}
			}
		}
		else {
			printf(REGISTRO_N_ENCONTRADO);
		}
		break;
	case 2:
		if (ntrajetos > 0)
		{
			for (int i = 0; i < ntrajetos; i++)
			{
				ll* lista = iroute[i].lista;
				listar_trajetos(lista, chaves, nregistros);
				if (i + 1 < ntrajetos)
					printf("\n");
			}
		}
		else {
			printf(REGISTRO_N_ENCONTRADO);
		}
		break;
	case 3:
		if (nregistros > 0)
		{
			for (int i = 0; i < nregistros; i++)
			{
				Ip* pk_encontrado = bsearch(idriver[i].pk, chaves, nregistros, sizeof(Ip), (int(*)(const void*, const void*)) strcmp);
				if (pk_encontrado)
				{
					if (pk_encontrado->rrn != -1)
					{
						if (i != 0)
							printf("\n");
						Carona car = recuperar_registro(pk_encontrado->rrn);
						imprimirCarona(car);
					}
				}
			}
		}
		else {
			printf(REGISTRO_N_ENCONTRADO);
		}
		break;
	case 4:
		if (nregistros > 0)
		{
			Carona* caronas = (Carona*)malloc(sizeof(Carona) * nregistros);
			
			int num_elem = 0;
			for (int i = 0; i < nregistros; i++)
			{
				Ip* pk_encontrado = bsearch(idate[i].pk, chaves, nregistros, sizeof(Ip), (int(*)(const void*, const void*)) strcmp);
				if (pk_encontrado->rrn != -1)
				{
					caronas[num_elem] = recuperar_registro(pk_encontrado->rrn);
					num_elem++;
				}
			}
			qsort(caronas, num_elem, sizeof(Carona), (int(*)(const void*, const void*))ordenaDH);
			for (int i = 0; i < num_elem; i++)
			{
				imprimirCarona(caronas[i]);
				if (i + 1 < num_elem)
					printf("\n");
			}

			free(caronas);
		}
		else {
			printf(REGISTRO_N_ENCONTRADO);
		}
		break;
	case 5:
		if(nregistros > 0)
		{ 
			Carona* caronas = (Carona*)malloc(sizeof(Carona) * nregistros);
			for (int i = 0; i < ntrajetos; i++)
			{
				int num_elem = 0;
				ll* lista = iroute[i].lista;
				while (lista)
				{
					Ip* pk_encontrado = bsearch(lista->pk, chaves, nregistros, sizeof(Ip), (int(*)(const void*, const void*)) strcmp);
					if (pk_encontrado->rrn != -1)
					{
						caronas[num_elem] = recuperar_registro(pk_encontrado->rrn);
						num_elem++;
					}
					lista = lista->prox;
				}
				qsort(caronas, num_elem, sizeof(Carona), (int(*)(const void*, const void*))ordenaDH);
				for (int j = 0; j < num_elem; j++)
				{
					imprimirCarona(caronas[j]);
					if (j + 1 < num_elem)
						printf("\n");
				}
				if (i + 1 < ntrajetos)
					printf("\n");
			}
			free(caronas);
		}
		else {
			printf(REGISTRO_N_ENCONTRADO);
		}
		break;
	default:
		break;
	}
}

void liberar(int* nregistros)
{
	char novoarq[TAM_ARQUIVO];
	novoarq[0] = '\0';
	int nregarq = 0;
	int rrn;

	//Remover no STRNCOPY e refazer indice
	for (int i = 0; i < *nregistros; i++)
	{
		rrn = i * TAM_REGISTRO;
		if (ARQUIVO[rrn] != '*')
		{
			strncat(novoarq, (ARQUIVO + rrn), TAM_REGISTRO);
			nregarq++;
		}
	}


	strcpy(ARQUIVO, novoarq);

	*nregistros = nregarq;
}

int ordenaDH(Carona* primeiro, Carona* segundo)
{
	char dia1[3], mes1[3], ano1[3], dia2[3], mes2[3], ano2[3];
	strncpy(ano1, primeiro->data + 6, 2);
	strncpy(mes1, primeiro->data + 3, 2);
	strncpy(dia1, primeiro->data, 2);
	strncpy(ano2, segundo->data + 6, 2);
	strncpy(mes2, segundo->data + 3, 2);
	strncpy(dia2, segundo->data, 2);
	int result = strncmp(ano1, ano2, 2);
	if (result == 0)
	{
		result = strncmp(mes1, mes2, 2);
		if (result == 0)
		{
			result = strncmp(dia1, dia2, 2);
		}
	}
	return result;

	result = strcmp(primeiro->hora, segundo->hora);
	if (result)
		return result;

	result = strcmp(primeiro->pk, segundo->pk);
	return result;
}

void finaliza(Ip* iprimary, Ir* iroute, Is* idriver, Isd* idate, Ist* itime, int nregistros, int ntraj)
{
	free(iprimary);
	free(idriver);
	free(idate);
	free(itime);
	ll* no;
	ll* aux;
	for (int i = 0; i < ntraj; i++)
	{
		no = iroute[i].lista;
		while (no)
		{
			aux = no;
			no = no->prox;
			free(aux);
		}
	}
	free(iroute);
}