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
			libera(a->filho[i]);
		 
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
      		printf("%d\n", a->info[i]->mat);
   	 	}
    	imprime(a->filho[i],andar+1);
	}
}

TAB *busca(TAB *raiz, int x)
{
	//printf("buscando %d\n",x);
	if(!raiz)return raiz;
	int i = 0;
	while(i < raiz->nchaves && x > raiz->info[i]->mat) i++;
	//printf("valor de mat:%d\n",raiz->info[i]->mat); 
	if(i < raiz->nchaves && x == raiz->info[i]->mat) return raiz;
	return busca(raiz->filho[i],x);
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
}

TAB *divide(TAB *b,int i, TAB *a, int t)
{
	//filho da direita de a
	TAB *c = cria_no(t);
	c->nchaves = t + (a->folha - 1);
	c->folha = a->folha;
	if(a->folha)c->info[0] = a->info[t-1];
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
	b->prox = NULL;
	if(a->folha)a->prox = c;
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
		raiz->nchaves++;
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
	return raiz;
}
void daMerge2(TAB *pai,TAB *a,int t){
	if(a->nchaves<=t-1){
		if(a->prox && a->prox->nchaves>=t-1){
			a->info[a->nchaves]=a->prox->info[0];
			a->filho[a->nchaves+1]=a->prox->filho[0];
			a->nchaves++;
			int j;
			for(j=1;j<a->prox->nchaves;j++){
				a->prox->info[j-1]=a->info[j];
				a->prox->filho[j-1]=a->filho[j];
			}
			a->prox->filho[j-1]=a->filho[j];
			a->prox->nchaves--;
		}
		else{	//tem que jogar no pai,nao juntar com o do lado
			int j=0;
			for(j=0;j<pai->nchaves;j++){
				if(pai->filho[j]==a){
					a->info[a->nchaves-1]=pai->info[j];
					a->filho[a->nchaves]=a->prox->filho[0];
					a->prox->filho[0]=NULL;		
					break;
				}
			}
			for(j=0;j<a->prox->nchaves;j++){
				a->info[j+a->nchaves]=a->prox->info[j];
				a->filho[j+a->nchaves+1]=a->prox->filho[j+1];
				a->prox->filho[j+1]=NULL;
				a->prox->info[j]=NULL;
				
			}
		}
	}

}

/*void daMerge(TAB*pai,TAB*a,int t){
	if(a->prox!=NULL && a->prox->nchaves<=t-1 && (a->nchaves <= t -1) && pai!=NULL){
		int j;
		struct TInfo *aux;
		for(j=0;j<a->prox->nchaves;j++){
			if(j==a->prox->nchaves-1) aux=a->prox->info[a->prox->nchaves-1];
			a->info[a->nchaves+j]=a->prox->info[j];
			a->filho[a->nchaves+1+j]=a->prox->filho[j];
			a->prox->info[j]=NULL;
			a->nchaves++;
		}
		a->filho[a->nchaves+1+j]=a->prox->filho[j];
		a->info[a->nchaves-1]=aux;
		a->prox=a->prox->prox;
		for(j=0;j<pai->nchaves+1;j++){
			if(pai->filho[j]==a){
				int k;
				for(k=j+1;k<pai->nchaves+1;k++){
					pai->filho[k]=pai->filho[k+1];
				}
				pai->filho[pai->nchaves]=NULL;
			
				for(k=j;k<pai->nchaves;k++){
					pai->info[k]=pai->info[k+1];
				}
				pai->info[pai->nchaves-1]=NULL;
				pai->nchaves--;
			}
		}
	}
}*/

TAB *retira(TAB *pai,TAB *a,int mat,int t)
{
	int i,aux;
	for(i = 0; i< a->nchaves && a->info[i]->mat <= mat; i++);
	aux=i;
	if(a->folha)
	{
		printf("é folha %d",t-1);
		if(a->nchaves > t -1)//caso 1
		{
			printf("CASO 1");
			int i,j;
			for(i = 0; i < a->nchaves; i++)
			{

				printf("avalido o %d",a->info[i]->mat);
				if(a->info[i]->mat == mat)
				{
					if(i==0){
						for(j=1;j<pai->nchaves+1;j++){
							if(pai->filho[j]==a){
								pai->info[j-1]=pai->filho[j]->info[1];
								break;
							}
						}
					}
					free(a->info[i]->nome);
					free(a->info[i]);
					a->info[i] = NULL;
					break;
				}
			}
			
			for(j = i; j < a->nchaves;j++)
			{
				a->info[j] = a->info[j+1];
			}
			a->nchaves--;
		}
		
		//pega do irmao
		else if(a->prox->nchaves>t-1){
			a->info[a->nchaves]=a->prox->info[0];
			a->nchaves++;
			int j;
			for(j=1;j<a->prox->nchaves;j++){
				a->prox->info[j-1]=a->prox->info[j];
			}
			a->prox->nchaves--;
			
			
			for(j=0;j<pai->nchaves;j++){
				if(pai->filho[j]==a){
					pai->info[j]=pai->filho[j+1]->info[0];
					break;
				}
			}
			
			a= retira(pai,a,mat,t);
		}
		//combina os dois se nao puder pegar
		else{
			int j;
			struct TAB *aux;
			for(j=0;j<a->prox->nchaves;j++){
				if(j==a->prox->nchaves-1) aux=a->prox->info[a->prox->nchaves-1];
				a->info[a->nchaves+j]=a->prox->info[j];
				a->prox->info[j]=NULL;
				a->nchaves++;
			}
			a->info[a->nchaves-1]=aux;
			a->prox=a->prox->prox;
			for(j=0;j<pai->nchaves+1;j++){
				if(pai->filho[j]==a){
					int k;
					for(k=j+1;k<pai->nchaves+1;k++){
						pai->filho[k]=pai->filho[k+1];
					}
					pai->filho[pai->nchaves]=NULL;
					
					for(k=j;k<pai->nchaves;k++){
						pai->info[k]=pai->info[k+1];
					}
					pai->info[pai->nchaves-1]=NULL;
					pai->nchaves--;
				}
			}
			a= retira(pai,a,mat,t);
		
		}
		return a;
	}
	//tem que aplicar casos 3A e 3B para o filho onde o valor ṕode estar
	//daMerge2(pai,a,t);
	a->filho[i] = retira(a,a->filho[i],mat,t);
	if(!a->folha && i>0) a->info[i-1]=a->filho[i]->info[0];
	
	
	return a;
}

void limpaTNCcom50cht(TAB *raiz,TAB *a,TCur *c,int t){
	if(!a->folha) limpaTNCcom50cht(raiz,a->filho[0],c,t);
	else{
		int i;
		while(a->prox!=NULL){
			for(i=0;i<a->nchaves;i++){
				if(a->info[i]->CHCS>=c[a->info[i]->cur].tnc && a->info[i]->CHCS<c[a->info[i]->cur].cht/2) retira(raiz,raiz,a->info[i]->mat,t);
			}
			a=a->prox;
		}
	}
}

void limpaNTORPEReNao100CHT(TAB *raiz,TAB *a,TCur *c,int t){
	if(!a->folha) limpaTNCcom50cht(raiz,a->filho[0],c,t);
	else{
		int i;
		while(a->prox!=NULL){
			for(i=0;i<a->nchaves;i++){
				if(c[a->info[i]->cur].ntotper<=a->info[i]->NPU && c[a->info[i]->cur].cht>a->info[i]->CHCS) retira(raiz,raiz,a->info[i]->mat,t);
			}
			a=a->prox;
		}
	}
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

int main (void)
{
	TCur *curriculos = cria_curriculos();
	
	int t;
	printf("insira o valor de T:\n");
	scanf("%d",&t);
	TAB *raiz = inicializa();
	int i;
	int mat,cur,aux;
	char *nome;
	float cr;
	printf("Aperte 1 para encher a arvore\n");
	printf("Aperte 2 para inserir um valor\n");
	printf("Aperte 3 para remover um valor\n");
	printf("Aperte 4 para imprimir a arvore\n");
	printf("Aperte 5 para buscar e imprimir um aluno\n");	
	printf("Aperte 6 para limpeza 1(nao atingiu 50 Porcento do cht no final do tempo)\n");
	printf("Aperte 7 para limpeza 2(nao atingiu 100 Porcento do cht no maximo de periodos)\n");
	printf("Aperte 8 para alterar CHCS\n");
	printf("Aperte 9 para alterar CR\n");
	printf("Aperte 10 para alterar NPU\n");
	printf("Aperte 11 para alterar TRANC\n");
	printf("Aperte 12 para limpar a arvore\n");
	scanf("%d",&i);
	//loop para operar o programa
	while(i != 0)
	{
		
		if(i==1){
			raiz = preenche(raiz,"arq.txt",t,curriculos);
		}
		else if(i==2){
			printf("Mat:");
			scanf("%d",&mat);
			printf("\nCr:");
			scanf("%f",&cr);
			printf("\nCurriculo:");
			scanf("%d",&cur);
			printf("\nNome:");
			scanf("%s",nome);
			insere(raiz,mat,cr,cur,nome,t);	
		}
		else if(i==3){
			printf("Mat:");
			scanf("%d",&mat);
			retira(raiz,raiz,mat,t);
		}
		else if(i==4){
			imprime(raiz,0);
		}
		else if(i==5){
			printf("Mat:");
			scanf("%d",&mat);
			imprimeAluno(raiz,mat,curriculos);
		}
		else if(i==6){
		 	limpaTNCcom50cht(raiz,raiz,curriculos,t);
		}
		else if(i==7){
			limpaNTORPEReNao100CHT(raiz,raiz,curriculos,t);
		}
		else if(i==8){
			
			printf("Mat:");
			scanf("%d",&mat);
			printf("Novo CHCS:");
			scanf("%d",&aux);
			alteraCHCS(raiz,mat,aux);
		}
		else if(i==9){
			
			printf("Mat:");
			scanf("%d",&mat);
			
			printf("Novo CR:");
			scanf("%f",&cr);
			alteraCR(raiz,mat,cr);
		
		}
		else if(i==10){
			
			printf("Mat:");
			scanf("%d",&mat);
			
			printf("Novo NPU:");
			scanf("%d",&aux);
			alteraNPU(raiz,mat,aux);
		
		}
		else if(i==11){
			
			printf("Mat:");
			scanf("%d",&mat);
			
			printf("Novo Tranc:");
			scanf("%d",&aux);
			alteraTRANC(raiz,mat,aux);
		
		}
		else if(i==12){
			libera(raiz);
		}
		printf("Aperte 1 para encher a arvore\n");
		printf("Aperte 2 para inserir um valor\n");
		printf("Aperte 3 para remover um valor\n");
		printf("Aperte 4 para imprimir a arvore\n");
		printf("Aperte 5 para buscar e imprimir um aluno\n");	
		printf("Aperte 6 para limpeza 1(nao atingiu 50 Porcento do cht no final do tempo)\n");
		printf("Aperte 7 para limpeza 2(nao atingiu 100 Porcento do cht no maximo de periodos)\n");
		printf("Aperte 8 para alterar CHCS\n");
		printf("Aperte 9 para alterar CR\n");
		printf("Aperte 10 para alterar NPU\n");
		printf("Aperte 11 para alterar TRANC\n");
		printf("Aperte 12 para limpar a arvore\n");
		scanf("%d",&i);
	}
	libera(raiz);
	free(curriculos);
	return 0;
}
