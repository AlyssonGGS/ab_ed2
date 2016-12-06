#include <stdlib.h>
#include <stdio.h>

typedef struct no
{
	int nchaves;
	int folha;
	struct no *pai;
	struct no **filhos;
	struct info **infos;
}TNO;

typedef struct curriculo
{
	//CHT = carga horaria total, ntotper = maximo de periodos do curriculo, tnc = tempo normal de curso
	int cht, ntotper, tnc;
}TCur;


typedef struct info
{
	int mat;
	float cr;
	//NPU = numero de periodos na universidade, CHT = carga horária total
	//CHCS = carga horaria cursada com sucesso, ntranc = numero de trancamentos
	//cur = tipo do curriculo
	int CHCS, NPU, ntranc,cur,nchaves;
	//variavel que guarda as infos no mesmo "array"
	struct info *ant, *prox;
}TInfo;

TNO *inicializa_no()
{
	return NULL;
}

//está dando erro ao inserir primeiro elemento
TNO *cria_no(int t)
{
	TNO *no = (TNO*)malloc(sizeof(TNO));
	no->pai = NULL;
	no->folha = 0;
	no->filhos = (TNO**)malloc(sizeof(TNO) * (2 * t - 1));
	no->infos= (TInfo**)malloc(sizeof(TInfo) * (2 * t - 1));
	int i;
	for(i = 0; i < 2 * t-1; i++)
	{
		no->filhos[i] = NULL;
		no->infos[i] = NULL;
	}
	return no;
}

TInfo *cria_aluno(int mat, int cr, int cur)
{
	TInfo *aluno = (TInfo*) malloc(sizeof(TInfo));
	aluno->mat = mat;
	aluno->cr = cr;
	aluno->ntranc = 0;
	aluno->CHCS = 0;
	aluno->NPU = 0;
	aluno->cur = cur;
	aluno->prox = aluno->ant = NULL;
	return aluno;
}

TNO *insere(TNO *raiz, int mat, float cr, int cur, int t)
{
	TNO *resp;
	if(raiz->infos[0] == NULL)
	{
		raiz->infos[0] = cria_aluno(mat, cr, cur);
		resp = raiz;
	}
	return resp;
}

TNO *busca(TNO *raiz, int x)
{
	if(!raiz) return raiz;
	int i;
	TNO *resp = NULL;
	while(i < raiz->nchaves && x > raiz->infos[i]->mat) i++;
	if(raiz->folha && i < raiz->nchaves && x == raiz->infos[i]->mat) return raiz;
	return busca(raiz->filhos[i],x);
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
	TNO *raiz = cria_no(t);
	raiz->folha = 1;
	raiz = insere(raiz,3,3.5,1,t);
	printf("raiz %d",raiz->infos[0]->mat);
	free(curriculos);
	return 0;
}
