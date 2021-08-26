#include "arvoreb+.h"
#include "string.h"
#include "math.h"

int ehfolha(TNo* no)
{
    if (no->rrn != -1)
        return no->rrn;
    return -1;
}

void liberar(TNo* no)
{
    free(no->pai);
    free(no->esq);
    free(no->dir);
}

void inserirArvore(TNo* t, char separador[], int rrn)
{
    t=(TNo*)malloc(sizeof(TNo));
    t->esq = NULL;
    t->dir = NULL;
    strcpy(t->separador, separador);
    t->rrn = rrn;
}

void encontrarSeparador(char *chave1, char *chave2, char *separador01)
{
    while(1)
    {
        *separador01 = *chave2;
        separador01++;
        if(*chave2 != *chave1)
            break;
        if(*chave2 == 0)
            break;
        chave1++;
        chave2++;
    }
    *separador01 = 0;
}

int inArray(int n, int array[], int tam)
{
    for (int i = 0; i < tam; i++){
        if (n == array[i])
            return 1;
    }
    return 0;
}

void ordemAlfabetica(FILE* arq, int nFunc, int* ordem[])
{

    TFunc* v = malloc(nFunc*sizeof(TFunc*));
    int pos = 0;
    int i = 0;
    int j = 0;
    int registro = 0;
    int menor[nFunc];
    char antes[50];
    char* atual;

    fseek(arq, (registro)*tamanho_registro(), SEEK_SET);
    registro = 1;
    v = le_funcionario(arq);
    strcpy(antes, v->nome);

    while(i < nFunc)
    {
        rewind(arq);
        j = 0;

        while (1)
        {
            if (inArray(registro, menor, i))
            {
                registro++;
                continue;
            }

            fseek(arq, (registro)*tamanho_registro(), SEEK_SET);
            v = le_funcionario(arq);

            if(v == NULL)
                break;

            int tam = 0;

            while (v->nome[tam] != '\0' || antes[tam] != '\0')
            {
                if (v->nome[tam] == antes[tam])
                    tam++;
                else if (v->nome[tam] < antes[tam])
                {
                    strcpy(antes, v->nome);
                    pos = registro;
                    break;
                }
                else
                    break;
            }
            j++;
            registro++;
        }

        menor[i] = pos;
        i++;
        pos = 0;
        registro = 0;
        strcpy(antes, "zzz");
    }

    for (int k = 0; k < nFunc; k++)
        ordem[k] = menor[k];
    free(v);
}

void criarB(char* arquivo, int n, int nFunc)
{
    Lista* blocos;
    int tamanhoBloco = (nFunc/2);
    FILE* arq;
    FILE* indexArq = fopen("indice.dat", "wb+");

    if((arq = fopen(arquivo, "rb"))){
        printf("Árvore B+\n");
    }
    else{
        printf("Nao foi possivel abrir o arquivo\n");
        return;
    }

    blocos = BlocosEncadeados(arq, nFunc);
    TNo** nos = malloc(tamanhoBloco*sizeof(TNo*));
    int j = 1;
    int cont = 0;
    int contB = 1;
    int rrn;
    char *chave1;
    char *chave2;
    char separador01[15];
    int totalDeElementos = 0;
    int rrnE, rrnD;

    while(blocos->prox->prox->tamanho!= -1)
    {

        chave1 = blocos->prox->nome;
        rrnE = blocos->tamanho;

        blocos = blocos->prox->prox;

        chave2 = blocos->nome;
        rrnD = blocos->tamanho;

        encontrarSeparador(chave1, chave2, separador01);
        nos[totalDeElementos] = criar(separador01);
        strcpy(nos[totalDeElementos]->esq->separador, chave1);
        nos[totalDeElementos]->esq->rrn = rrnE;
        strcpy(nos[totalDeElementos]->dir->separador, chave2);
        nos[totalDeElementos]->dir->rrn = rrnD;
        nos[totalDeElementos]->bloco = totalDeElementos;
        strcpy(nos[totalDeElementos]->folha, TRUE);

        if(contB == n)
        {
            nos[totalDeElementos - 1]->dir = nos[totalDeElementos];
            contB = 0;
        }
        else
            contB++;
        totalDeElementos++;
    }

    for(int i = 0; i < totalDeElementos; i++)
    {

        if (cont == n)
        {
            strcpy(nos[i]->folha, FALSE);
            nos[i]->esq = nos[i - 2];
            nos[i]->esq->pai = nos[i];

            if (nos[i + 1] != NULL)
            {
                nos[i]->dir = nos[i + 1];
                nos[i]->dir->pai = nos[i];
            }
            else
            {
                nos[i]->dir->pai = nos[i];
                nos[i]->dir->pai->rrn = nos[i]->dir->rrn;
            }
            i++;
            cont++;
            continue;
        }
        else if(cont == (2*n+1))
        {
            strcpy(nos[i]->folha, FALSE);
            nos[i]->esq = nos[i - 3];
            nos[i]->esq->pai = nos[i];

            if (totalDeElementos < i + 3)
            {
                nos[i]->dir = nos[i + 3];
                nos[i]->dir->pai = nos[i];
            }
            else
            {
                nos[i]->dir = nos[totalDeElementos - 1];
                nos[i]->dir->pai = nos[i];
            }
            cont=0;
            continue;
        }
        cont++;
    }
    printf("\n Criando indice...\n\n");
    criar_indice(indexArq, nos, totalDeElementos);
    printf("\n Imprimindo indice...\n\n");
    imprimir_indice("indice.dat", totalDeElementos);
    libera(blocos);
    fclose(arq);
    fclose(indexArq);
    free(nos);
}

void imprimirB(TNo* no)
{
    printf("\nBloco: %d\n", no->bloco);
    printf("\nrnn: %d\n\n", no->rrn);
    printf("\nPai: %s\n", no->pai->separador);
    printf("\nFolha: %s\n", no->folha);
}

void criar_indice(FILE *arq, TNo **nos, int tam)
{
    for(int k=0; k < tam; k++)
    {
        fwrite(&nos[k]->bloco, sizeof(int), 1, arq);
        fwrite(nos[k]->separador, sizeof(char), sizeof(nos[k]->separador), arq);
        fwrite(nos[k]->pai->separador, sizeof(char), sizeof(nos[k]->pai->separador), arq);
        fwrite(nos[k]->folha, sizeof(char), sizeof(nos[k]->folha), arq);
        fwrite(&nos[k]->esq->rrn, sizeof(int), 1, arq);
        fflush(arq);
    }
}

void imprimir_indice(char* nomeArq, int tam)
{
    FILE* arq = fopen(nomeArq, "rb");
    TNo* no = criar("null");
    rewind(arq);

    while (!feof(arq))
    {
        fread(&no->bloco, sizeof(int), 1, arq);
        fread(no->separador, sizeof(char), sizeof(no->separador), arq);
        fread(no->pai->separador, sizeof(char), sizeof(no->pai->separador), arq);
        fread(no->folha, sizeof(char), sizeof(no->folha), arq);
        fread(&no->rrn, sizeof(int), 1, arq);

        imprimirB(no);
    }
    free(no);
    fclose(arq);
}

TNo* criar(char separador[])
{
    TNo* novo;
    novo = (TNo*)malloc(sizeof(TNo));
    strcpy(novo->separador, separador);
    novo->rrn = -1;
    novo->bloco = NULL;
    novo->pai = (TNo*)malloc(sizeof(TNo*));
    novo->dir = (TNo*)malloc(sizeof(TNo*));
    novo->esq = (TNo*)malloc(sizeof(TNo*));
    strcpy(novo->folha, FALSE);
    return novo;
}

Lista* BlocosEncadeados(FILE *arq, int nFunc)
{
    TFunc* funcao;
    Lista* blocos;
    int* ordem[nFunc];
    int pos;

    ordemAlfabetica(arq, nFunc, ordem);

    rewind(arq);
    blocos = cria("Head", NULL);

    for(int k=nFunc-1; k>=0; k--)
    {
        rewind(arq);
        pos = ordem[k];
        fseek(arq, pos * tamanho_registro(), SEEK_SET);
        funcao = le_funcionario(arq);
        blocos = cria(funcao->nome, blocos);
        blocos->tamanho = pos;
    }
    return blocos;
}
