#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct no
{
	int nchaves;
	int folha;
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
	char *nome;
	//NPU = numero de periodos na universidade
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
		{
			libera(a->filho[i]);
			//free(a->filho[i]);
		}
		for(i = 0; i < a->nchaves;i++)
		{
			free(a->info[i]->nome);
			free(a->info[i]);
		}
		free(a);
	}
}
//está dando erro ao inserir primeiro elemento
TAB *cria_no(int t)
{
	TAB *no = (TAB*)malloc(sizeof(TAB));
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

TInfo *cria_aluno(int mat, int cr, int cur,char *nome)
{
	TInfo *aluno = (TInfo*) malloc(sizeof(TInfo));
	aluno->mat = mat;
	aluno->cr = cr;
	aluno->ntranc = 0;
	aluno->CHCS = 0;
	aluno->NPU = 0;
	aluno->cur = cur;
	aluno->nome = malloc(sizeof(char) * 50);
	strcpy(aluno->nome,nome);
	return aluno;
}

void imprime(TAB *a, int andar){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      	imprime(a->filho[i],andar+1);
      	for(j=0; j<=andar; j++) printf("   ");
      		printf("%d:%d\n",a->nchaves,a->info[i]->mat);
   	 	}
    	imprime(a->filho[i],andar+1);
	}
}

TAB *busca(TAB *raiz, int x)
{
	if(!raiz)return raiz;
	int i = 0;
	while(i < raiz->nchaves && x > raiz->info[i]->mat) i++;
	if(raiz->folha && i < raiz->nchaves && x == raiz->info[i]->mat) return raiz;
	return busca(raiz->filho[i],x);
}

TInfo *copiaInfo(TInfo *a)
{
	TInfo *resp = cria_aluno(a->mat,a->cr,a->cur,a->nome);
	return resp;
}

TInfo *buscaAluno(TAB *a,int mat)
{
	TAB *arv = busca(a,mat);
	if(!arv)
	{
		printf("Informacao nao encontrada na arvore\n");
		return NULL;
	}
	int i;
	for(i = 0; i < arv->nchaves;i++)
	{
		if(arv->info[i]->mat == mat) return arv->info[i];
	}
	return NULL;
}

void imprimeAluno(TAB *a, int mat, TCur *curs)
{
	TInfo *aluno = buscaAluno(a,mat);
	if(!aluno)
	{
		printf("Aluno nao encontrado");
		return;	
	}
	printf("Aluno:%s\n",aluno->nome);
	printf("Matricula: %d\n",aluno->mat);
	printf("CR: %f\n",aluno->cr);
	printf("Trancamentos: %d\n",aluno->ntranc);
	printf("CHCS:%d\n",aluno->CHCS);
	printf("NPU:%d\n",aluno->NPU);
	printf("Curriculo:%d\n",curs[aluno->cur].cht);
	printf("Folha:%d\n",busca(a,mat)->folha);
}

TAB *divide(TAB *b,int i, TAB *a, int t)
{
	//filho da direita de a
	TAB *c = cria_no(t);
	c->nchaves = t + (a->folha - 1);
	c->folha = a->folha;
	if(a->folha) c->info[0] = a->info[t-1];
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
 	b->info[i-1] = copiaInfo(a->info[t-1]);
  	b->nchaves++;
	a->prox = c;
	return b;
}

TAB *insere_incompleto(TAB *a, int mat, float cr, int cur, char *nome, int t)
{
	int i = a->nchaves-1;
	if(a->folha){
		while((i>=0) && (mat<a->info[i]->mat)){
	  		a->info[i+1] = a->info[i];
	  		i--;
		}
		a->info[i+1] = cria_aluno(mat,cr,cur,nome);
		a->folha = 1;
		a->nchaves++;
		return a;
	}
	while((i>=0) && (mat<a->info[i]->mat)) i--;
  	i++;
  	if(a->filho[i]->nchaves == ((2*t)-1)){
   		a = divide(a, (i+1), a->filho[i], t);
   	 	if(mat>a->info[i]->mat) i++;
  	}
  	a->filho[i] = insere_incompleto(a->filho[i], mat,cr,cur, nome, t);
	imprime(a,0);
	return a;
}

TAB *insere(TAB *raiz, int mat, float cr, int cur, char *nome, int t)
{
	if(busca(raiz,mat))return raiz;
	if(!raiz) 
	{
		raiz = cria_no(t);
		raiz->folha = 1;
		raiz->info[0] = cria_aluno(mat,cr,cur,nome);
		raiz->nchaves = 1;
		imprime(raiz,0);
		return raiz;
	}
	if((raiz->nchaves == 2 * t - 1))
	{
		TAB *b = cria_no(t);
		b->nchaves=0;
		b->folha = 0;
		b->filho[0] = raiz;
		b = divide(b,1,raiz,t);
		b = insere_incompleto(b,mat,cr,cur, nome,t);	
		return b;
	}
	raiz = insere_incompleto(raiz,mat,cr,cur, nome,t);
	imprime(raiz,0);
	return raiz;
}

TAB *retira(TAB *a,int mat,int t)
{
	if(a==NULL) return a;
	int i;
	for(i = 0; i < a->nchaves && a->info[i]->mat <= mat; i++);
	if(a->folha)
	{
		if(a->nchaves > t -1)//caso 1
		{
			printf("CASO 1");
			int k;
			for(k = 0; k < a->nchaves; k++)
			{
				if(a->info[k]->mat == mat)
				{
					free(a->info[k]->nome);
					free(a->info[k]);
					break;
				}
			}
			if(k >= a->nchaves) return a;
			int j;
			for(j = k; j < a->nchaves;j++)
			{
				a->info[j] = a->info[j+1];
			}
			printf("I:%d\n",k);
			a->nchaves--;
		}
		return a;
	}
	
	TAB *y = a->filho[i], *z = NULL;
 	if (y->nchaves == t-1)
 	{ 	//CASOS 3A e 3B
		if((i < a->nchaves) && (a->filho[i+1]->nchaves >=t))
		{ //CASO 3A
		  printf("\nCASO 3A: i menor que nchaves\n");
			z = a->filho[i+1];
			if (y->folha) y->info[t-1] = z->info[0];		  
			else y->info[t-1] = a->info[i];
			y->nchaves++;
			imprime(y,0);
			printf("Primeira chave de z:%d\n",z->info[0]->mat);
			int j;
		  	for(j=0; j < z->nchaves-1; j++)  //ajustar chaves de z
				z->info[j] = z->info[j+1];
			a->info[i] = z->info[0];     //dar a a uma chave de z
	  		if(!y->folha) y->filho[y->nchaves] = z->filho[0]; //enviar ponteiro menor de z para o novo elemento em y
		  	for(j=0; j < z->nchaves; j++)       //ajustar filhos de z
				z->filho[j] = z->filho[j+1];
			y->folha = z->folha;
		  	z->nchaves--;
		  	a->filho[i] = retira(a->filho[i], mat, t);
		  	return a;
		}
		  
	  
		if((i > 0) && (!z) && (a->filho[i-1]->nchaves >=t))
		{ //CASO 3A
		  	printf("\nCASO 3A: i igual a nchaves\n");
		  	z = a->filho[i-1];
		 	int j;
		  	for(j = y->nchaves; j>0; j--)               //encaixar lugar da nova chave
				y->info[j] = y->info[j-1];
		  	for(j = y->nchaves+1; j>0; j--)             //encaixar lugar dos filhos da nova chave
				y->filho[j] = y->filho[j-1];
			if(y->folha) y->info[0] = z->info[z->nchaves-1];              //dar a y a chave i da arv
			else y->info[0] = a->info[i-1];
		  	y->nchaves++;
		  	a->info[i-1] = y->info[0];//z->chave[z->nchaves-1];   //dar a arv uma chave de z
		  	y->filho[0] = z->filho[z->nchaves];         //enviar ponteiro de z para o novo elemento em y
		 	z->nchaves--;
			z->folha = y->folha;
		  	a->filho[i] = retira(y, mat, t);
		  	return a;
		}
		if(!z){ //CASO 3B
			printf("Entrei no caso 3B");
      		if(i < a->nchaves && a->filho[i+1]->nchaves == t-1){
				z = a->filho[i+1];
				if(!y->folha)y->info[t-1] = a->info[i];     //pegar chave [i] e coloca ao final de filho[i]
				else y->info[t-1] = z->info[0]; 
				y->nchaves++;
				int j;
				for(j=z->folha; j < t-1; j++){
				  y->info[t+(j-y->folha)] = z->info[j];     //passar filho[i+1] para filho[i]
				  y->nchaves++;
				}
				if(!y->folha){
				  for(j=0; j<t; j++){
					y->filho[t+j] = z->filho[j];
				  }
				}
				for(j=i; j < a->nchaves-1; j++){ //limpar referÃªncias de i
				  a->info[j] = a->info[j+1];
				  a->filho[j+1] = a->filho[j+2];
				}
				a->nchaves--;
				y->folha = z->folha;
				if(a->nchaves == 0) a = a->filho[0];
				a = retira(a, mat, t);
				return a;
      		}
			if((i > 0) && (a->filho[i-1]->nchaves == t-1)){ 
				z = a->filho[i-1];
				if(i == a->nchaves)
				{
					if(!z->folha)z->info[t-1] = a->info[i-1]; //pegar chave[i] e poe ao final de filho[i-1]
					else z->info[t-1] = y->info[0];
				}
				else{
					if(!z->folha) z->info[t-1] = a->info[i];   //pegar chave [i] e poe ao final de filho[i-1]
					else z->info[t-1] = y->info[0];
				}
				z->nchaves++;
				int j;
				for(j=z->folha; j < t-1; j++){
				  z->info[t+(j-z->folha)] = y->info[j];     //passar filho[i+1] para filho[i]
				  z->nchaves++;
				}
				if(!z->folha){
				  for(j=0; j<t; j++){
					z->filho[t+j] = y->filho[j];
				  }
				}
				z->folha = y->folha;
				a->nchaves--;
				a->filho[i-1] = z;
				if(a->nchaves == 0) a = a->filho[0];
				a = retira(a, mat, t);
				return a;
			}
		}
	}
	printf("chamando retira pro filho: %d\n",i);
	a->filho[i] = retira(a->filho[i],mat,t);
	return a;
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

int selecionaCurriculo(int cht)
{
	return cht-1;
}

TAB *preenche(TAB *a,char *arq,int t,TCur *curs)
{
	FILE *fp = fopen(arq,"rt");
	if(!fp)
	{
		printf("arquivo não aberto com sucesso");
		exit(1);
	}
	int mat, tranc,chc,nper, cur;
	float cr;
	char linha[200];
	while(fgets(linha,sizeof(linha),fp))
	{
		char *tok = strtok(linha," ");
		mat = atoi(tok);
		tok = strtok(NULL," ");
		cr = atof(tok);
		tok = strtok(NULL," ");
		tranc = atoi(tok);
		tok = strtok(NULL," ");
		chc = atoi(tok);
		tok = strtok(NULL," ");
		nper = atoi(tok);
		tok = strtok(NULL," ");
		int cht = atoi(tok);
		cur = selecionaCurriculo(cht);
		
		//concatena os nomes separados por espaço
		char nome[100];
		strcpy(nome,"");
		tok = strtok(NULL," ");
		while(tok)
		{
			strcat(nome,tok);
			strcat(nome," ");
			tok = strtok(NULL," ");
		}
		//tira quebra de linha no final do nome
		char *quebra = strchr(nome,'\n');
		if(quebra) *quebra = 0;
		
		a = insere(a,mat,cr,cur,nome,t);
		TInfo *aluno = buscaAluno(a,mat);
		aluno->ntranc = tranc;
		aluno->CHCS  = chc;
		aluno->NPU = nper;
	}
	return a;
}

void alteraCHCS(TAB *a, int mat, int novo)
{
	TInfo *aluno = buscaAluno(a,mat);
	if(!aluno)return;
	aluno->CHCS =novo;
}

void alteraCR(TAB *a, int mat, float novo)
{
	TInfo *aluno = buscaAluno(a,mat);
	if(!aluno)return;
	aluno->cr =novo;
}

void alteraNPU(TAB *a, int mat, int novo)
{
	TInfo *aluno = buscaAluno(a,mat);
	if(!aluno)return;
	aluno->NPU =novo;
}

void alteraTRANC(TAB *a, int mat, int novo)
{
	TInfo *aluno = buscaAluno(a,mat);
	if(!aluno)return;
	aluno->ntranc =novo;
}

TAB* retira_raiz(TAB*a,int mat,int t){
	if(!a) return a;
	printf("folha:%d\n",a->folha);
	if(a->folha)
	{
		printf("nchaves: %d\n",a->nchaves);
		int i;
		printf("antes do for:%d\n",a->nchaves);
		for(i = 0; i < a->nchaves; i++)
		{
			printf("Achei o cara %d com mat: %d\n",i,a->info[i]->mat);
			if(a->info[i]->mat == mat)
			{
				free(a->info[i]->nome);
				free(a->info[i]);
				a->info[i] = NULL;
				break;
			}
		}
		int j;
		for(j = i; j < a->nchaves;j++)
		{
			a->info[j] = a->info[j+1];
		}
		a->nchaves--;
		if(a->nchaves == 0)
		{
			libera(a);
			a = NULL;
		}
		return a;
	}
	printf("nao estou removendo da raiz\n");
	return retira(a,mat,t);
}

int main (void)
{
	TCur *curriculos = cria_curriculos();
	
	int t;
	printf("insira o valor de T:\n");
	scanf("%d",&t);
	TAB *raiz = inicializa();
	raiz = preenche(raiz,"arq.txt",t,curriculos);
	int i;
	scanf("%d",&i);
	//loop para operar o programa
	while(i != 0)
	{
		imprime(raiz,0);
		raiz = retira_raiz(raiz,i,t);
		/*//imprimeAluno(raiz,i,curriculos);
		//printf("raiz->nchaves:%d\n",raiz->nchaves);
		if(raiz && raiz->folha==1){
			raiz=retira_raiz(raiz,i,t);
			//printf("é raiz");
		}
		else{
			raiz = retira(raiz,i,t);
			if(raiz && raiz->filho[0]==NULL) raiz->folha=1;
			//printf("Filho%d\n",raiz->filho[0]->info[0]->mat);
		}*/
		imprime(raiz,0);
		scanf("%d",&i);
	}
	libera(raiz);
	free(curriculos);
	return 0;
}
