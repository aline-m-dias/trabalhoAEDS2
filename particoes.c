#include "funcionarios.h"
#include <stdlib.h>
#include <limits.h>

#include "particoes.h"

void classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc)
{
    rewind(arq); //posiciona cursor no inicio do arquivo

    int reg = 0;

    while (reg != nFunc)
    {
        //le o arquivo e coloca no vetor
        TFunc *v[M];
        int i = 0;
        while (!feof(arq))
        {
            fseek(arq, (reg) * tamanho_registro(), SEEK_SET);
            v[i] = le_funcionario(arq);
            //     imprime_funcionario(v[i]);
            i++;
            reg++;
            if(i>=M) break;
        }

        //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
        if (i != M)
        {
            M = i;
        }

        //faz ordenacao
        for (int j = 1; j < M; j++)
        {
            TFunc *f = v[j];
            i = j - 1;
            while ((i >= 0) && (v[i]->cod > f->cod))
            {
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
        if ((p = fopen(nome_particao, "wb+")) == NULL)
        {
            printf("Erro criar arquivo de saida\n");
        }
        else
        {
            for (int i = 0; i < M; i++)
            {
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
TFunc* obtemFuncionario(FILE *arq, int* cont)
{
    fseek(arq, *cont * tamanho_registro(), SEEK_SET);
    TFunc* f= le_funcionario(arq); //recebe um funcionario do arquivo
    (*cont)++;
    return f;
}
int menorChave(TFunc* vetor[], int indVetor)
{
    int menor=0;
    for (int j=1; j<indVetor; j++)
    {
        if (vetor[menor]->cod < vetor[j]->cod)
        {
            menor=j;
        }
    }
    return menor;

}
//cria uma nova partição
void criaParticao(Lista* nomes, int* nParticoes)
{
    if(nomes->prox == NULL)
    {
        char* novoNome = malloc( 5 * sizeof(char));
        (*nParticoes)++;
        sprintf(novoNome, "p%d.dat", *nParticoes);
        nomes->prox = cria(novoNome, NULL);
    }
}


void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int n, int *nParticoes)
{

    rewind(arq); //posiciona cursor no inicio do arquivo
    int cont= 0; //contador de registro funcionario
    Lista* nomes = nome_arquivos_saida; //indicar os nomes das partições
    FILE *particaooatual= NULL;
    int contParticao=0;
    //FILE *particao;
    FILE *repositorio = fopen("repositorio.dat", "w+"); //abertura do repositório
    int contRepositorio=0;

    TFunc** vetor= malloc ( M * sizeof( TFunc* )); // vetor para armazenar m registro do arquivo (funcionário)
    int indiceVetor = 0;

    while (cont != nFunc)
    {
        printf("Entrou");
        int i=0;
        while (!feof(arq))
        {
            //faz a leitura M regitros (funcionario) para a memoria

            vetor[i] = obtemFuncionario(arq, &cont); //recebe um funcionario
            //imprime_funcionario(vetor[i]); /*imprime as m registros desordenados*/
            i++;
            if(i>=M)
            {
                break;
            }
        }
<<<<<<< HEAD


        //enquanto o indice do vetor for menor que o numero de elementos M, voltar  no inicio
        if (indiceVetor < M)
        {
=======
         printf("Entrou2");

        //precisa ajustar tamanho M caso arquivo tenha terminado antes do vetor
        if (indiceVetor < M)
        {

>>>>>>> 160da09c3c1823bd35dc407fa934208e4c1413fb
           continue; //o programa retorna no começo e ignorar as outras instruções
        }

        //Selecionar, no array em memória, o registro (funcionario) r com menor chave

        int menor=menorChave(vetor,indiceVetor);

        //Gravar o registro r na partição de saída

        if (particaooatual == NULL)
        {

<<<<<<< HEAD
            char *nome= nomes->nome;
            printf("\n%s\n", nome);
            particaooatual= fopen(nome,"w+");

            criaParticao(nomes, nParticoes); //cria a partição
=======
            char *nome=nomes->nome;
            printf("\n%s\n", nome);
            particaooatual= fopen(nome,"w+");

            /*nomeAtual = nomes;*/
            criaParticao(nomes, nParticoes);
>>>>>>> 160da09c3c1823bd35dc407fa934208e4c1413fb

            nomes= nomes->prox;
        }

        //fecha a partição ser for maior do que o numero de elementos a ser armazenado
        if (contParticao>=M)
        {
            fclose(particaooatual);
            particaooatual=NULL;
            contParticao=0;
            continue;

        }

        fseek(particaooatual, contParticao * tamanho_registro(), SEEK_SET); //leitura e escrita na partição
        salva_funcionario(vetor[menor], particaooatual);
        imprime_funcionario(vetor[menor]);
        fflush(stdout);
<<<<<<< HEAD
=======
        /* nomeAtual->tamanho++;*/
>>>>>>> 160da09c3c1823bd35dc407fa934208e4c1413fb
        contParticao++;


        //Substituir, no array em memória, o registro r pelo próximo registro do arquivo de entrada

        if( !(cont < nFunc))
        {
            for (int i = menor; i<indiceVetor-1; i++)
            {
                vetor[i] = vetor[i+1];//substitui
            }
            indiceVetor--;
            break;
        }

        int cod= vetor[menor]->cod; //salva o codigo do menor
        free(vetor[menor]);
        vetor[menor]= obtemFuncionario(arq, &cont); // adiciona o próximo funcionario

        // testa se chave deste último seja menor do que a chave recém gravada, gravá-lo no reservatório e substituir
        if (vetor[menor]->cod < cod)
        {

            fseek(repositorio, (contRepositorio) * tamanho_registro(), SEEK_SET);
            salva_funcionario(vetor[menor], repositorio);
            imprime_funcionario(vetor[menor]);
            contRepositorio++;
            if( !(cont < nFunc))
            {
                for (int i = menor; i<indiceVetor-1; i++)
                {
                    vetor[i] = vetor[i+1];
                }
                indiceVetor--;
                break;
            }
            vetor[menor]=obtemFuncionario(arq, &cont);
        }

        //Caso ainda exista espaço livre no reservatório, voltar ao passo 2
        if (contRepositorio < n)
        {
            continue;
        }
        fclose(particaooatual); //fechar a partição de saída

        /*cria uma nova partição*/
        char *nome= nomes->nome;
        printf("\n%s\n", nome);
        particaooatual= fopen(nome, "w+");
<<<<<<< HEAD
=======
        /*nomeAtual = nomes;*/
>>>>>>> 160da09c3c1823bd35dc407fa934208e4c1413fb
        criaParticao(nomes, nParticoes);
        nomes=nomes->prox;

        //copiar os registros do reservatório para o array em memória
        int j=0;
        while (indiceVetor>0)
        {
            fseek(particaooatual, j * tamanho_registro(), SEEK_SET );
            int menor= menorChave(vetor, indiceVetor);
            salva_funcionario(vetor[menor], particaooatual);
            imprime_funcionario(vetor[j]);
<<<<<<< HEAD

=======
            // nomeatual->tamanho++;
            //pode tentar fazer uma função
>>>>>>> 160da09c3c1823bd35dc407fa934208e4c1413fb
            for (int j=menor; j< indiceVetor-1; j++)
            {
                vetor[j]= vetor[j+1]; //substitui pelo proximo
            }

            indiceVetor--;
            j++;
        }

        //liberar o vetor (esvazia o resevatorio)
        for (int j = 0; j < indiceVetor; j++)
        {
            free(vetor[j]);
        }

        indiceVetor = 0;

        for (int j=0; j<contRepositorio; j++)
        {
            fseek(repositorio, j * tamanho_registro(), SEEK_SET);
            vetor[j]= le_funcionario(repositorio);
            imprime_funcionario(vetor[j]);
            indiceVetor++;
        }

        contRepositorio=0;
        fclose(particaooatual);
        particaooatual=NULL;
        contParticao=0;

    }

    if (particaooatual)
    {
        fclose(particaooatual);
        particaooatual=NULL;
        contParticao=0;
<<<<<<< HEAD
    }//termina laço while
=======
    }

    //int contDaUltimaPart=0;
>>>>>>> 160da09c3c1823bd35dc407fa934208e4c1413fb

    //verificamos se ainda existe registros no resotório
    if (!(indiceVetor<= 0 && contRepositorio<=0)){
        //colocar no arquivo partição os itens que ainda estão no repositorio
        if(contRepositorio>0){

            char *nome=nomes->nome;
            printf("\n%s\n", nome);
            particaooatual= fopen(nome, "w+");

            contParticao=0;

            for(int j=0; j<contRepositorio; j++){
                //pega do repositorio e adiciona na partição
                fseek(repositorio, j*tamanho_registro(), SEEK_SET);
                TFunc * func = le_funcionario(repositorio);
                fseek(particaooatual, contParticao * tamanho_registro(), SEEK_SET);
                salva_funcionario(func, particaooatual);
                imprime_funcionario(vetor[j]);

                contParticao++;

            }
            fclose(particaooatual);
            contParticao=0;


        }

        //salva no repositorio
        if (indiceVetor>0)
        {
            criaParticao(nomes, nParticoes);
            nomes=nomes->prox;
            char *nome=nomes->nome;
            printf("\n%s\n", nome);
            particaooatual= fopen(nome, "w+");
<<<<<<< HEAD

=======
            //nomeAtual = nomes;
>>>>>>> 160da09c3c1823bd35dc407fa934208e4c1413fb
            contParticao=0;

            for(int j=0; j<indiceVetor; j++)
            {
                fseek(particaooatual, j * tamanho_registro(), SEEK_SET);
                salva_funcionario(vetor[j], particaooatual);
                imprime_funcionario(vetor[j]);

                contParticao++;


            }

            fclose(particaooatual);

        }
    }

    //printf("cheguei no final da seleção natural");

    fclose(repositorio);
    free(vetor);

}
