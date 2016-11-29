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
	int chaves, nchaves;
}TNo;

//n sei o que fazer
void inicializa_no(TNo *raiz)
{
	raiz = NULL;
	raiz->filhos = NULL;
	raiz->pai = NULL;
}

TNo *cria_no()
{
	TNo *no = (TNo*) malloc(sizeof(TNo));
	no->filhos = NULL;
	no->pai = NULL;
	//tomar cuidado para atualizar isso na hora de inserir um filho
	no->nchaves = 0;
	
	return no;
}


int main (void)
{
	int t;
	printf("insira o valor de T:\n");
	scanf("%d",&t);
	TNo *raiz = cria_no();
	return 0;
}
