#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct no
{
	int nchaves;
	int folha;
	struct no *pai;
	struct no **filho;
	struct info **info;
	struct no *prox;
}TAB;

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
	//variavel que guarda as info no mesmo "array"
	struct info *prox;
}TInfo;

TAB *inicializa()
{
	return NULL;
}

void libera(TAB *a)
{
	if(a)
	{
		int i;
		if(!a->folha)
		for(i = 0; i < a->nchaves + 1;i++)
			libera(a->filho[i]);
		 
		for(i = 0; i < a->nchaves;i++)
			free(a->info[i]);
		
		free(a);
	}
}
//está dando erro ao inserir primeiro elemento
TAB *cria_no(int t)
{
	TAB *no = (TAB*)malloc(sizeof(TAB));
	no->pai = NULL;
	no->folha = 0;
	no->filho = (TAB**)malloc(sizeof(TAB) * (2 * t));
	no->info= (TInfo**)malloc(sizeof(TInfo) * (2 * t - 1));
	no->prox = NULL;
	int i;
	for(i = 0; i < 2 * t; i++)
	{
		no->filho[i] = NULL;
	}
	
	for(i = 0; i < 2 * t-1; i++)
	{
		no->info[i] = NULL;
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
	return aluno;
}

void imprime(TAB *a, int andar){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      	imprime(a->filho[i],andar+1);
      	for(j=0; j<=andar; j++) printf("   ");
      		printf("%d\n", a->info[i]->mat);
   	 	}
    	imprime(a->filho[i],andar+1);
	}
}
TAB *busca(TAB *raiz, int x)
{
	//printf("buscando %d\n",x);
	if(!raiz)
		return raiz;
	
	int i = 0;
	while(i < raiz->nchaves && x > raiz->info[i]->mat) i++;
	//printf("valor de mat:%d\n",raiz->info[i]->mat); 
	if(i < raiz->nchaves && x == raiz->info[i]->mat) return raiz;
	return busca(raiz->filho[i],x);
}

TAB *divide(TAB *b,int i, TAB *a, int t)
{
	//filho da direita de a
	TAB *c = cria_no(t);
	c->nchaves = t + (a->folha - 1);
	c->folha = a->folha;
	if(a->folha)
		c->info[0] = a->info[t-1];
	int j;
	//copia as chaves
	for(j=0;j<t-1;j++) c->info[j+a->folha] = a->info[j+t];
	//copia os filhos
	if(!a->folha)
	{
    	for(j=0;j<t;j++)
		{
      		c->filho[j] = a->filho[j+t];
      		a->filho[j+t] = NULL;
    	}
  	}
	//conserta o numero de chaves da esquerda
	a->nchaves = t-1;
  	for(j=b->nchaves; j>=i; j--) b->filho[j+1]=b->filho[j];
  	b->filho[i] = c;
  	for(j=b->nchaves; j>=i; j--) b->info[j] = b->info[j-1];
 	b->info[i-1] = a->info[t-1];
  	b->nchaves++;
	a->prox = c;
	return b;
}

TAB *insere_incompleto(TAB *a, int mat, float cr, int cur, int t)
{
	int i = a->nchaves-1;
	if(a->folha){
		while((i>=0) && (mat<a->info[i]->mat)){
	  		a->info[i+1] = a->info[i];
	  		i--;
		}
		a->info[i+1] = cria_aluno(mat,cr,cur);
		a->nchaves++;
		return a;
	}
	while((i>=0) && (mat<a->info[i]->mat)) i--;
  	i++;
  	if(a->filho[i]->nchaves == ((2*t)-1)){
   		a = divide(a, (i+1), a->filho[i], t);
   	 	if(mat>a->info[i]->mat) i++;
  	}
  	a->filho[i] = insere_incompleto(a->filho[i], mat,cr,cur, t);
	return a;
}
TAB *insere(TAB *raiz, int mat, float cr, int cur, int t)
{
	printf("inserindo:%d\n",mat);
	if(busca(raiz,mat))return raiz;
	if(!raiz) 
	{
		raiz = cria_no(t);
		raiz->folha = 1;
		raiz->info[0] = cria_aluno(mat,cr,cur);
		raiz->nchaves++;
		return raiz;
	}
	if((raiz->nchaves == 2 * t - 1))
	{
		printf("faz divisao\n");
		TAB *b = cria_no(t);
		b->nchaves=0;
		b->folha = 0;
		b->filho[0] = raiz;
		b = divide(b,1,raiz,t);
		b = insere_incompleto(b,mat,cr,cur,t);	
		imprime(b,0);
		return b;
	}
	raiz = insere_incompleto(raiz,mat,cr,cur,t);
	imprime(raiz,0);
	return raiz;
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
	TAB *raiz = inicializa();
	//raiz->folha = 1;
	raiz = insere(raiz,3,3.5,1,t);
	raiz = insere(raiz,5,3.5,1,t);
	raiz = insere(raiz,9,3.5,1,t);
	raiz = insere(raiz,20,3.5,1,t);
	raiz = insere(raiz,4,3.5,1,t);
	raiz = insere(raiz,1,3.5,1,t);
	raiz = insere(raiz,0,3.5,1,t);
	raiz = insere(raiz,21,3.5,1,t);
	libera(raiz);
	free(curriculos);
	return 0;
}
