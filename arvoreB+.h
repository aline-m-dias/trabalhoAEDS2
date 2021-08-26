#ifndef ARVOREB+H_INCLUDED
#define ARVOREB+H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "funcionarios.h"

#define TRUE "TRUE"
#define FALSE "FALSE"

#define SEM_FILHO -1

typedef struct no
{
    struct no* pai;
    struct no* esq;
    struct no* dir;
    int rrn;
    int bloco;
    char folha[5];
    char separador[15];

}TNo;

typedef struct Tnos
{
    char* separador;
    struct Tnos* proximo;

}TNos;

//Assinaturas das funções

int ehfolha(TNo* no);
void liberar(TNo *no);
void inserirArvore(TNo* t, char separador[], int rrn);
void encontrarSeparador(char* chave1, char* chave2, char* separador01);
void ordemAlfabetica(FILE* arq, int nFunc, int* ordem[]);
void criarB(char* arquivo, int n, int nFunc);
void imprimirB(TNo* no);
void criar_indice(FILE* arq, TNo** nos, int tam);
void imprimir_indice(char* nomeArq, int tam);
TNo* criar(char* separador);
Lista* BlocosEncadeados(FILE* arq, int nFunc);

#endif // ARVOREB+_H_INCLUDED
