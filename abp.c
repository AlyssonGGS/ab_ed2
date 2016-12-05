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
	aluno->folha = 0;
	aluno->prox = aluno->ant = aluno->pai = aluno->filhos = NULL;
	return aluno;
}

TCur *cria_curriculos()
{
	TCur *curs = (TCur*)malloc(sizeof(TCur)*3);
	//curriculo 1
	curs[0].cht = 2955;
	curs[0].ntotper = 16;
	curs[0].tnc = 8;
	
	//curriculo 2
	curs[1].cht = 3524;
	curs[1].ntotper = 12;
	curs[1].tnc = 8;
	
	//curriculo 3
	curs[2].cht = 3200;
	curs[2].ntotper = 12;
	curs[2].tnc = 8;
	return curs;
}


int main (void)
{
	TCur *curriculos = cria_curriculos();
	
	int t;
	printf("insira o valor de T:\n");

	scanf("%d",&t);
	TNO *raiz = inicializa_no();

	free(curriculos);
	return 0;
}
