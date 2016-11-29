#include <stdlib.h>
#include <stdio.h>

typedef struct aluno
{
	float cr;
	int mat, ntranc;
	int CHCS, CHT, NTOTPER, NPU, TNC;
	struct aluno *ant, *prox;
}Folha;

typedef struct no
{
	Folha *filhos;
	struct no *pai;
	int chave, nchaves;
}TNo;

//n sei o que fazer
TNo *inicializa_no()
{
	return NULL;
}

TNo *cria_no()
{
	TNo *no = (TNo*) malloc(sizeof(TNo));
	no->filhos = NULL;
	no->pai = NULL;
	//tomar cuidado para atualizar isso na hora de inserir um filho
	no->nchaves = -1;
	return no;
}

Folha *cria_aluno()
{
	Folha *aluno = (Folha*) malloc(sizeof(Folha));
	aluno->cr = 0;
	aluno->mat = 0;
	aluno->ntranc = 0;
	aluno->CHCS = 0;
	aluno->CHT = 0;
	aluno->NTOTPER = 0;
	aluno->NPU = 0;
	aluno->TNC = 0;
	aluno->prox = aluno->ant = NULL;
	return aluno;
}


int main (void)
{
	int t;
	printf("insira o valor de T:\n");
	scanf("%d",&t);
	TNo *raiz = inicializa_no();
	return 0;
}
