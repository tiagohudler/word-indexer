#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define FALSE 0
#define TRUE 1
#define TAMANHO 1000
typedef int Boolean;

// implementações inserção de linhas

typedef struct messi {
    int num;
    char* texto;
    struct messi* prox;
} linha;

void remove_punct_and_make_lower_case(char *p){
    char *src = p, *dst = p;

    while (*src)
    {
       if (ispunct((unsigned char)*src))
       {
          /* Skip this character */
          src++;
       }
       else if (isupper((unsigned char)*src))
       {
          /* Make it lowercase */
          *dst++ = tolower((unsigned char)*src);
          src++;
       }
       else if (src == dst)
       {
          /* Increment both pointers without copying */
          src++;
          dst++;
       }
       else
       {
          /* Copy character */
          *dst++ = *src++;
       }
    }

    *dst = 0;
}

void flush_in(){ 
    int ch;
    while( (ch = fgetc(stdin)) != EOF && ch != '\n' ){} 
}

// Implementações necessárias para árvore

typedef struct elemento {
    linha* linhas;
    linha* ultima_linha;
    char* palavra;
    int aparicoes;
    struct elemento* direita;
    struct elemento* esquerda;
} no_arvore;

typedef struct cr7 {
    no_arvore* raiz;    
} arvore;

arvore criaArvore () {
    arvore tree;
    tree.raiz = NULL;
    return tree;
}

int inicializaNoArvore (no_arvore* no, char* palavra, linha* lin){ //melhor alocar a arvore dentro da função (passamos apenas a palavra e a linha como ref)
    no->palavra = palavra;
    no->aparicoes = 1;
    no->linhas = no->ultima_linha = lin;
    no->direita = no->esquerda = NULL;
    return 1;
}

int inicializaLinha (linha* lin, int linha_num, char* linha_texto) { //melhor alocar a linha dentro da função (passamos apenas o numero e o texto como ref)
    lin->num = linha_num;
    lin->prox = NULL;
    lin->texto = linha_texto;
    return 1;
}

int addPalavraArvore (no_arvore* novo, arvore* arvore) {
    no_arvore* raiz = arvore->raiz;
    //arvore vazia
    if (!(raiz)) {
        arvore->raiz = novo;
        return 1;
    }

    //inserção iterativa usando um ponteiro auxiliar
    no_arvore* aux = raiz;
    no_arvore* folha;
    int comparacao;
    while (aux){
        comparacao = strcmp(novo->palavra, aux->palavra);
        if (comparacao > 0) {
            folha = aux;
            aux = aux->direita;
        }
        if (comparacao < 0) {
            folha = aux;
            aux = aux->esquerda;
        }
        //palavra ja está na árvore
        if (comparacao == 0) { 
            aux->aparicoes++;
            //adicionar a linha no aux
            linha* linha_aux = aux->linhas;
            int linha_atual = novo->linhas->num;
            while (linha_aux) {
                if (linha_aux->num == linha_atual) {
                    free(novo->linhas);
                    return 1;
                    }
                else linha_aux = linha_aux->prox;
            }
            aux->ultima_linha->prox = novo->linhas;
            aux->ultima_linha = aux->ultima_linha->prox;
            free(novo);
            return 1;
        }
    }

    //adiciona como folha
    if (comparacao > 0) {
        folha->direita = novo;
        return 1;
    }
    if (comparacao < 0) {
        folha->esquerda = novo;
        return 1;
    }
    return 0;
}

no_arvore* buscaArvore(char* palavra, arvore arvore) {
    int comparacao;
    no_arvore* aux = arvore.raiz;
    while (aux) {
        comparacao = strcmp(palavra, aux->palavra);
        if (comparacao > 0) aux = aux->direita;
        if (comparacao < 0 ) aux = aux->esquerda;
        if (comparacao == 0) return aux;
    }
    return NULL; 
}

void printaNoArvore(no_arvore* no, char* pal_busca){
    if (!no) {
		printf("Palavra '%s' nao encontrada.\n", pal_busca);
		return;
	}
	linha* linha_aux = no->linhas;
	printf("Existem %i ocorrências da palavra '%s' na(s) seguinte(s) linha(s):\n\n", no->aparicoes, no->palavra);
	while (linha_aux){
		printf("%05i: %s \n", linha_aux->num, linha_aux->texto);
		linha_aux = linha_aux->prox;
	}
	return;	
}

// fim das implementações arborísticas

// implementações necessárias para lista ligada

typedef struct _no_ {
  	char* palavra;
    linha* linhas;
    linha* ultima_linha;
    int aparicoes;
  	struct _no_* proximo;
} no_lista;

typedef struct {

  int tamanho;  
  no_lista * cabeca;

} ListaLigada;

int tamanho(ListaLigada * lista){

  return lista->tamanho;
}

no_lista * inicializaNoLista (char* palavra, linha* lin){
    no_lista* no = (no_lista*) malloc (sizeof(no_lista));
    no->palavra = palavra;
    no->aparicoes = 1;
    no->linhas = lin;
    no->ultima_linha = lin;
    no->proximo = NULL;

    return no;
}

ListaLigada * cria_lista(){

  no_lista * cabeca =  (no_lista *) malloc (sizeof(no_lista));
  cabeca->proximo = cabeca;

  ListaLigada * lista =  (ListaLigada *) malloc (sizeof(ListaLigada));
  lista->cabeca = cabeca;
  lista->tamanho = 0;

  return lista;
}

no_lista* buscaLista(ListaLigada * lista, char* e){

  int i = 0;
  no_lista * p = lista->cabeca->proximo;
  int comparacao = strcmp(e, p->palavra);
  if (p == lista->cabeca) return NULL;
  
  while(p != lista->cabeca && comparacao >= 0){
    comparacao = strcmp(e, p->palavra);
    if (comparacao == 0) return p;
    p = p->proximo;
    i++;
  }
  return NULL;
}

Boolean inserelista(ListaLigada * lista, no_lista* e){

  int comparacao = 0;
  no_lista * p;
  no_lista * anterior;
  linha* aux;

  anterior = lista->cabeca;
  p = lista->cabeca->proximo;

  if (p == lista->cabeca){
    e->proximo = lista->cabeca->proximo;
    lista->cabeca->proximo = e;
    lista->tamanho++;
    return TRUE;
  }

  while(p != lista->cabeca){
    comparacao = strcmp(e->palavra, p->palavra);
        if (comparacao == 0) {
            p->aparicoes++;
            aux = p->linhas;
            while (aux) {
            if (aux->num == e->linhas->num) {
                free(e->linhas);
                return TRUE;
                }
            else aux = aux->prox;
            }
            p->ultima_linha->prox = e->linhas;
            p->ultima_linha = e->linhas;
            free(e);
            return TRUE;
        };

        if(comparacao < 0) break;

    anterior = p;
    p = p->proximo;
  }
  e->proximo = p;   
  anterior->proximo = e;
  lista->tamanho++;

  return TRUE;
}

void printaNoLista(no_lista* no, char * pal_busca){
	if (!no) {
		printf("Palavra '%s' nao encontrada.\n", pal_busca);
		return;
	}
	linha* linha_aux = no->linhas;
	printf("Existem %i ocorrências da palavra '%s' na(s) seguinte(s) linha(s):\n", no->aparicoes, no->palavra);
	while (linha_aux){
		printf("%05i: %s \n", linha_aux->num, linha_aux->texto);
		linha_aux = linha_aux->prox;
	}
	return;
}

// fim das implementações para lista ligada 


int main(int argc, char ** argv){

    FILE * in;
    char * line;
    char * copia_ponteiro_line;
    char * quebra_de_line;
    char * palavra; 
    int contador_line;
    int comparacao = 0;
    char* operacao;
    char* indice;
    char* texto;
    char pal_busca[100];
    clock_t start_t, end_t;
    double total_t;
    int i;

    if(argc == 3) {

        arvore arvore_nova = criaArvore();
        ListaLigada* lista = cria_lista();
        indice = argv[2];
        texto = argv[1];


        in = fopen(argv[1], "r");
    if ((strcmp(argv[2], "lista") != 0) && (strcmp(argv[2], "arvore") != 0)) {
                    printf("Parametro invalido\n");
                    return 0;
                }
        printf(">>>>> Carregando arquivo...\n");
        start_t = clock();
        contador_line = 0;
        line = (char *) malloc((TAMANHO + 1) * sizeof(char));

        while(in && fgets(line, TAMANHO, in)){

            if( (quebra_de_line = strrchr(line, '\n')) ) *quebra_de_line = 0;

            printf("line %03d: '%s'\n", contador_line + 1, line);

            char* linha_eterna = (char*)malloc(sizeof(char)*(TAMANHO+1));
            strcpy(linha_eterna, line);

            copia_ponteiro_line = line;

            while((palavra = strsep(&copia_ponteiro_line, " "))){
                remove_punct_and_make_lower_case(palavra);
                
                //cria struct de linha local
                linha* lin = (linha*)malloc(sizeof(linha));
                inicializaLinha(lin, contador_line + 1, linha_eterna);

                if ((comparacao = strcmp(argv[2], "arvore") == 0)) {
                    //cria ponteiro guardável para cada palavra
                    char* constpalavra = (char*)malloc(sizeof(palavra));
                    strcpy(constpalavra, palavra);

                    no_arvore* no_novo = (no_arvore*)malloc(sizeof(no_arvore));
                    inicializaNoArvore(no_novo, constpalavra, lin);
                    addPalavraArvore(no_novo, &arvore_nova);
                }
                if ((comparacao = strcmp(argv[2], "lista") == 0)) {
                    char* constpalavra = (char*)malloc(sizeof(palavra));
                    strcpy(constpalavra, palavra);

                    no_lista* no_novo = inicializaNoLista(constpalavra, lin);
                    inserelista(lista, no_novo);

                }
                

                printf("\t\t'%s'\n", palavra);
            }

            contador_line++;
        }
        end_t = clock();
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        printf(">>>>> Arquivo carregado!\n");
        printf("Tipo de indice: '%s'\n",indice);
        printf("Arquivo texto '%s'\n", texto);
        printf("Numero de linhas no arquivo: %d\n", contador_line);
        printf("Tempo para carregar o arquivo e construir o indice: %.4f ms\n\n\n", total_t * 1000);
        printf(">");
        
        char* aux;
        char input[100][30];
        int i;

        while (1) {

            //cria buffers para armazenar o input
            char* buffer = (char *) malloc((30) * sizeof(char));
            char* copia_buffer = (char *) malloc((30) * sizeof(char));
            i = 0;

            scanf("%[^\n]%*c", buffer);
            copia_buffer = buffer;

            //separa o input em palavras, formata, e coloca no array input
            while (aux = strsep(&copia_buffer, " ")){
                strcpy(input[i], aux);
                remove_punct_and_make_lower_case(input[i]);
                i++;
            }

            //condicoes de verificação
            if (strcmp(input[0], "fim") == 0) break;

            if (i > 2 || strcmp(input[0], "busca") != 0) {
                printf("Opcao invalida!\n");
                printf(">");
                free(buffer);
                free(copia_buffer);
                continue;
            }
            else {
                if(strcmp(argv[2], "arvore") == 0) {
                    start_t = clock();
                    printaNoArvore(buscaArvore(input[1], arvore_nova), input[1]);
                    end_t = clock();
                    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
                    printf("Tempo de busca: %f ms\n", total_t);
                    free(buffer);
                    free(copia_buffer);
                }
                if(strcmp(argv[2], "lista") == 0) {
                    start_t = clock();
                    printaNoLista(buscaLista(lista, input[1]), input[1]);
                    end_t = clock();
                    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
                    printf("Tempo de busca: %f ms\n",total_t);
                    free(buffer);
                    free(copia_buffer);
                }
                printf("\n=============================\n\n");
                printf(">");
            }
            
        }
        

        return 0;
    }

    return 1;
}



