#include "intercalacao.h"
#include "funcionarios.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes) {

    int fim = 0; //variavel que controla fim do procedimento
    FILE *out; //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL) {
        printf("Erro ao abrir arquivo de sa?da\n");
    } else {
        //cria vetor de particoes
        TVet v[num_p];

        //abre arquivos das particoes, colocando variavel de arquivo no campo f do vetor
        //e primeiro funcionario do arquivo no campo func do vetor
        for (int i=0; i < num_p; i++) {
            v[i].f = fopen(nome_particoes->nome, "rb");
            v[i].aux_p = 0;
            if (v[i].f != NULL) {
                fseek(v[i].f, v[i].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[i].f);
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posi??o do vetor
                    v[i].func = funcionario(INT_MAX, "","","",0);
                }
                else {
                    //conseguiu ler funcionario, coloca na posi??o atual do vetor
                    v[i].func = f;
                }
            }
            else {
                fim = 1;
            }
            nome_particoes = nome_particoes->prox;
        }

        int aux = 0;
        while (!(fim)) { //conseguiu abrir todos os arquivos
            int menor = INT_MAX;
            int pos_menor;
            //encontra o funcionario com menor chave no vetor
            for(int i = 0; i < num_p; i++){
                if(v[i].func->cod < menor){
                    menor = v[i].func->cod;
                    pos_menor = i;
                }
            }
            if (menor == INT_MAX) {
                fim = 1; //terminou processamento
            }
            else {
                //salva funcionario no arquivo de saída
                fseek(out, aux * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[pos_menor].func, out);
                //atualiza posição pos_menor do vetor com pr?ximo funcionario do arquivo
                v[pos_menor].aux_p++;
                fseek(v[pos_menor].f, v[pos_menor].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[pos_menor].f);
                aux++;
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posiçao do vetor
                    v[pos_menor].func = funcionario(INT_MAX, "", "", "",0.0);
                }
                else {
                    v[pos_menor].func = f;
                }

            }
        }

        //fecha arquivos das partiÇões de entrada
        for(int i = 0; i < num_p; i++){
            fclose(v[i].f);
        //    free(v[i].func);
        }
        //fecha arquivo de saída
        fclose(out);
    }
}

void intercalacao_arvore_de_vencedores(TPilha **pilha, int *vetTop, char *nome_arquivo_saida, int num_p){
    //implementar segunda parte do trabalho
    int qtdEntrada = num_p; //variavel que contém a quantidade dos arquivos de entrada
    FILE *out; //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL) {
        printf("Erro ao abrir arquivo de saída\n");
        return;
    } else {
        printf("Iniciando intercalação com árvore de vencedores\n");
    }
    TNoA **arvore = malloc(qtdEntrada * sizeof (TNoA*));
    TFunc **particoes = malloc(nFunc * sizeof(TFunc*));

    //laço encaixado para a criação da árvore
    for(int i =0; i < qtdEntrada; i++){
        TFunc *funcionario = pop(pilha[i], 0, &vetTop[i]);
        particoes[funcionario->cod] = funcionario;
        TNoA *no = criaNo_arvore_binaria(funcionario ->cod, -1);//Verificar o significado de rrn, que nesse caso é o -1
        no->pilha = i;
        arvore[i] = no;
    }

    TNoA *paiDeTodos;
    TNoA **pai = malloc(qtdEntrada * sizeof (TNoA*));
    TNoA **novoPai = malloc(qtdEntrada * sizeof (TNoA*));
    int numOperacoes = 0;
    int numElementos = 0;
    int numPais = 0;
    int numNovoPai = 0;
    int contNo = 0;

    while(1) {
        TNoA **espacoLivre;
        int tamEspacoLivre;
        int numComparacoes;
        if (numOperacoes == 0) {
            espacoLivre = arvore;
            tamEspacoLivre = qtdEntrada;
            numComparacoes = numElementos;
        } else {
            espacoLivre = pai;
            tamEspacoLivre = numElementos;
            numComparacoes = numNovoPai;
        }
        //criação dos nós da primeira iteração
        for (int i = 0; i < tamEspacoLivre; i += 2) {
            if (i + 1 == tamEspacoLivre) {
                TNoA *no = criaNo_arvore_binaria(espacoLivre[i]->info, -1);
                no->pilha = espacoLivre[i]->pilha;
                no->esq = espacoLivre[i];
                no->dir = NULL;
                no->esq->pai = no;
                if (numOperacoes == 0) {
                    pai[numElementos] = no;
                    numElementos += numElementos;
                } else {
                    novoPai[numNovoPai] = no;
                    numNovoPai += numNovoPai;
                }
                continue;
            }
            if (espacoLivre[i]->info < espacoLivre[i + 1]->info) {
                TNoA *no = criaNo_arvore_binaria(espacoLivre[i]->info, -1);
                no->pilha = espacoLivre[i]->pilha;
                no->esq = espacoLivre[i];
                no->dir = espacoLivre[i + 1];
                no->esq->pai = no;
                no->dir->pai = no;
                if (numOperacoes == 0) {
                    pai[numElementos] = no;
                    numElementos += numElementos;
                } else {
                    novoPai[numNovoPai] = no;
                    numNovoPai += numNovoPai;
                }
            } else {
                TNoA *no = criaNo_arvore_binaria(espacoLivre[i + 1]->info, -1);
                no->pilha = espacoLivre[i + 1]->pilha;
                no->esq = espacoLivre[i];
                no->dir = espacoLivre[i + 1];
                no->esq->pai = no;
                no->dir->pai = no;
                if (numOperacoes == 0) {
                    pai[numElementos] = no;
                    numElementos += numElementos;
                } else {
                    novoPai[numNovoPai] = no;
                    numNovoPai += numNovoPai;
                }
            }
        }
        //atrubui novos valores aos veotres pais
        if (numComparacoes == 1){
            if(numOperacoes == 0 ){
                paiDeTodos = pai[0];
            }else{
                paiDeTodos = paiNovo[0];
            }
            break;
        }
        if (numComparacoes == 0){
            numComparacoes += numComparacoes;
            continue;
        }
        for (int j = 0; j < numNovoPai; j++)
        {
            pai[j] = novoPai[j];
        }
        numElementos = numNovoPai;
        numNovoPai = 0;
        numOperacoes+=numOperacoes;
    }

    TNoA *atual = paiDeTodos;

    while (atual->info != INT_MAX){
        int vencedor = paiDeTodos->info;
        //percorre a árvore até encrontrar a folha do nó vencedor
        while (atual->esq != NULL || atual->dir != NULL){
            if (atual->esq != NULL){
                if (atual->esq->info == vencedor){
                    atual = atual->esq;
                    continue;
                }
            }
            if (atual->dir != NULL){
                if (atual->dir->info == vencedor){
                    atual = atual->dir;
                    continue;
                }
            }
        }
    }

    // salvar funcionario no arquivo
    TFunc *funcionario = particoes[vencedor];
    fseek(out, contNo * tamanho_registro(), SEEK_SET);
    contNo += contNo;
    salva_funcionario(funcionario, out);
    int posicaoPilha = paiDeTodos->pilha;

    funcionario = pop(pilha[posicaoPilha], 0, &vetTop[posicaoPilha]);
    // printf("Retirou da pilha o %d\n", func->cod);
    if (funcionario == NULL){
        atual->info = INT_MAX;
    }else{
        atual->info = funcionario->cod;
        particoes[funcionario->cod] = funcionario;
    }
    
    
    //adiciona os novos valores a árvore criada anteriormente
    while (atual->pai != NULL)
    {
        atual = atual->pai;
        if (atual->esq && atual->dir){
            if (atual->esq->info < atual->dir->info){
                atual->info = atual->esq->info;
                atual->pilha = atual->esq->pilha;
            }
            else{
                atual->info = atual->dir->info;
                atual->pilha = atual->dir->pilha;
            }
        }else{
            if (atual->esq != NULL){
                atual->info = atual->esq->info;
                atual->pilha = atual->esq->pilha;
            }
            else
            {
                atual->info = atual->dir->info;
                atual->pilha = atual->dir->pilha;
            }
        }

    }

    free(arvore);
    free(particoes);
    free(pai);
    free(novoPai);

}

