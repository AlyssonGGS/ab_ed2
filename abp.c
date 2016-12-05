#include <stdlib.h>
#include <stdio.h>

typedef struct curriculo
{
	//CHT = carga horaria total, ntotper = maximo de periodos do curriculo, tnc = tempo normal de curso
	int cht, ntotper, tnc;
}TCur;

typedef struct aluno
{
	int mat;
	int folha;
	float cr;
	//NPU = numero de periodos na universidade, CHT = carga horária total
	//CHCS = carga horaria cursada com sucesso, ntranc = numero de trancamentos
	//cur = tipo do curriculo
	int CHCS, NPU, ntranc,cur;
	struct aluno *ant, *prox, *pai, *filhos;
}TNO;


//n sei o que fazer
TNO *inicializa_no()
{
	return NULL;
}


TNO *cria_aluno()
{
	TNO *aluno = (TNO*) malloc(sizeof(TNO));
	aluno->cr = 0;
	aluno->mat = 0;
	aluno->ntranc = 0;
	aluno->CHCS = 0;
	aluno->NPU = 0;
	aluno->cur = -1;
	aluno->prox = aluno->ant = aluno->pai = aluno->filhos = NULL;
	return aluno;
}


int main (void)
{
	int t;
	printf("insira o valor de T:\n");
	scanf("%d",&t);
	TNO *raiz = inicializa_no();
	return 0;
}
