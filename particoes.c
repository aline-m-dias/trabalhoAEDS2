#include "funcionarios.h"
#include <stdlib.h>
#include <limits.h>

#include "particoes.h"

void classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc) {
    rewind(arq); //posiciona cursor no inicio do arquivo


    int reg = 0;

    while (reg != nFunc) {
        //le o arquivo e coloca no vetor
        TFunc *v[M];
        int i = 0;
        while (!feof(arq)) {
            fseek(arq, (reg) * tamanho_registro(), SEEK_SET);
            v[i] = le_funcionario(arq);
       //     imprime_funcionario(v[i]);
            i++;
            reg++;
            if(i>=M) break;
        }

        //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
        if (i != M) {
            M = i;
        }

        //faz ordenacao
        for (int j = 1; j < M; j++) {
            TFunc *f = v[j];
            i = j - 1;
            while ((i >= 0) && (v[i]->cod > f->cod)) {
                v[i + 1] = v[i];
                i = i - 1;
            }
            v[i + 1] = f;
        }

        //cria arquivo de particao e faz gravacao
        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;
        printf("\n%s\n", nome_particao);
        FILE *p;
        if ((p = fopen(nome_particao, "wb+")) == NULL) {
            printf("Erro criar arquivo de saida\n");
        } else {
            for (int i = 0; i < M; i++) {
                fseek(p, (i) * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[i], p);
                imprime_funcionario(v[i]);
            }
            fclose(p);
        }
        for(int jj = 0; jj<M; jj++)
            free(v[jj]);
    }
}

void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int n){
    /*PARTE 1 TRABALHO*/
    rewind(arq); //posiciona cursor no inicio do arquivo

    int cont= 0; //contador de registro funcionario
    int chave= INT_MAX;
    while (cont != nFunc){
        TFunc *vetor[M]; // vetor para armazenar m registro do arquivo (funcionário)
        int i = 0;
        while (!feof(arq)) {
            /*faz a leitura M regitros (funcionario) para a memoria*/
            fseek(arq, (reg) * tamanho_registro(), SEEK_SET);
            vetor[i] = le_funcionario(arq);
            i++;
            cont++;
            if(i>=M){
                break;
            }
        }
        //precisa ajustar tamanho M caso arquivo tenha terminado antes do vetor
        if (i != M) {
            M = i;
        }

        /*Selecionar, no array em memória, o registro (funcionario) r com menor chave*/

        TFunc menor= vetor[0];
        for (int j=0; j<M; j++){
            if (vetor[j]->cod < chave){
                if (vetor[j]->cod <  menor){
                    menor=vetor[j];
            }

            }
        }

    }
}
