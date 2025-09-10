#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

//estrutura inicial
typedef struct dir{ 
    char nome[100];
    int tam;
    struct dir *pFilho;
    struct dir *prox;
} Dir;


//cria diretorio na estrutura E no sistema de arquivo 
Dir* criarDir(const char *nome, int tam, int inicial){
    Dir* novoDir= (Dir*)malloc(sizeof(Dir));
    if(novoDir == NULL){
        printf("Erro de alocacao de memoria.\n");
        exit(1);
    }
    int i;
    for(i = 0; nome[i] != '\0' && i < 99; i++){
        novoDir->nome[i]= nome[i];
    }
    novoDir->nome[i]= '\0'; // Finaliza a string

    //definindo tamanho e ponteiros do diretorio
    novoDir->tam= tam;
    novoDir->pFilho= NULL;
    novoDir->prox= NULL;

    //tenta criar o diretorio no sistema de arquivos, incluindo o RAIZ
    if(_mkdir(nome) == 0){
        if(inicial == 0){
            printf("Diretorio '%s' criado com sucesso no sistema de arquivos.\n", nome);
        }
    }else{
        if (inicial == 0){
            printf("Erro ao criar o diretorio '%s' no sistema de arquivos.\n", nome);
        }
    }
    return novoDir; //retorna novo diretorio criado na estrutura em memoria
}

//cria um arquivo no diretorio atual
void criarArquivo(const char *nome, int tam){
    FILE *arquivo = fopen(nome, "w");
    
    if(arquivo){
        printf("Arquivo '%s' criado com sucesso no sistema de arquivos.\n", nome);
        fprintf(arquivo, "%*s", tam, ""); //cria o arquivo com o tamanho especificado
        fclose(arquivo);
    }else{
        printf("Falha ao criar o arquivo '%s'.\n", nome);
    }
}

//adiciona um diretorio filho
void adicionarFilho(Dir *pai, Dir *filho){
	
	//verifica se o pai ja tem Filho caso nao, o novo filho   atribuido a pFilho 
	if (pai->pFilho == NULL){ 
        pai->pFilho = filho;
	}else{
    //se ja tem pelo menos um filho, percorre ate o  ltimo e adiciona o novo filho
        Dir *temp = pai->pFilho;
        while (temp->prox != NULL){
            temp = temp->prox; //vai para o proximo irmao
        }
        temp->prox = filho; //adiciona o novo filho no final
    }
}

//busca por um diretorio especifico
//const char pra evitar erro
Dir* buscarDir(Dir *raiz, const char *nome){
    if(raiz == NULL) return NULL; //s  p verificar: se o diret rio for NULL, retorna NULL
    if(strcmp(raiz->nome, nome) == 0) return raiz; //se o nome do diret rio corresponder ao nome buscado, retorna o diret rio
	//strcmp compara as strings e retorna se for correto

    //faz a busca recursiva entre os filhos do diret rio
    Dir *temp= raiz->pFilho; //*temp vai percorrer a todos os filhos
    while (temp != NULL){
        //loop enquanto temp nao for NULL 
        Dir *res = buscarDir(temp, nome); 
        if (res != NULL) return res;  //caso encontrado, retorna o diret rio
        temp = temp->prox;  //nao encontrado, vai para o pr ximo irm o
    }
    return NULL; //se o diret rio nao for encontrado, retorna NULL
}

//procura o diret rio pai de um diret rio x
Dir* encontrarPai(Dir *raiz, Dir *filho){
    if(raiz == NULL || raiz->pFilho == NULL) return NULL; //se raiz for null (vazia) nao tem pai nem filho
    Dir *temp= raiz->pFilho;  //ponteiro temp que aponta para o primeiro filho
	if(temp == filho) return raiz; //verifica o 1o filho da raiz e retorna se for o procurado

    while (temp != NULL){ //percorre todo os irmaos 
        if (temp == filho) return raiz;
        
    //se o dir atual nao for o procurado, a funcao   chamada e recebe temp- que   o proximo diretorio- como a nova raiz
    Dir *res = encontrarPai(temp, filho);
        if (res != NULL) return res;
        temp = temp->prox; //temp   atualizado pro proximo irmao
    }
    return NULL;
}

//remove um diretorio inteiro
void removerDir(Dir *pai, const char *nome){
    if(pai == NULL || pai->pFilho == NULL){
        //printf("Erro ao remover diretorio. \n");
        return; } //se o diret rio pai ou o 1o filho forem NULL, n o tem o que remover

    Dir *temp = pai->pFilho; //ponteiro temp vai percorrer os filhos do diretorio pai
    Dir *anterior = NULL;

    //procura o diretorio a ser removido
    while (temp != NULL && strcmp(temp->nome, nome) != 0){
        anterior = temp; //temp eh atualizado quando toda vez que o nome nao corresponde
        temp = temp->prox; }

    //diretorio nao foi encontrado
    if(temp == NULL){
        printf("Erro. Diretorio '%s' nao encontrado.\n", nome);
        return; }

    //remove (recursivamente) os filhos do diretorio
    while (temp != NULL){
        Dir *filhoAtual = temp->pFilho;
        
        while(filhoAtual != NULL){
            Dir *proximoFilho = filhoAtual->prox; 
            removerDir(temp, filhoAtual->nome); //remove os filhos (recursividade
            filhoAtual = proximoFilho;
			}

        Dir *proximo= temp->prox; //salva o proximo antes de remover o atual

		//remove diretorio do sistema de arquivos
        if(_rmdir(temp->nome) == 0){
            printf("Diretorio '%s' excluido do sistema de arquivos!\n", temp->nome);
        }else{
            printf("Erro ao excluir o diretorio '%s' do sistema de arquivos.\n", temp->nome);
        }

        //libera o diretorio
        free(temp);
        temp = proximo; //proximo irmao
    }

	//atualiza o pai pra remover o diretorio	
    if(anterior == NULL){
        pai->pFilho = NULL;
    }else{
        anterior->prox = NULL;
    } 
}


//exclui um arquivo no sistema
void excluirArquivo(const char *nome){
    if(remove(nome) == 0){ //se remover retorna 0
        printf("Arquivo '%s' excluido do sistema. \n", nome);
    }else{
        printf("Erro ao excluir o arquivo '%s'. \n", nome);
    }
}

//imprime a estrutura de diret rios (em arvore
void imprimirEstrutura(Dir *raiz, int nivel){
    if(raiz == NULL) return;
    for(int i = 0; i < nivel; i++) printf("  "); //imprime espa os p preencher
    printf("%s (tamanho: %d) \n", raiz->nome, raiz->tam); //imprime novo do dir e seu tamanho

    imprimirEstrutura(raiz->pFilho, nivel + 1); //chama a funcao pro pFilho do diretorio atual
    //aumenta o nivel +1 (hierarquia
    imprimirEstrutura(raiz->prox, nivel); //chama o proximo filho e faz o mesmo, mantendo o nivel
}

//tamanho do diret rio
int obterTamanho(Dir *dir){
    if(dir == NULL) return 0; //se dir for NULL, tamanho   0
    int total= dir->tam; //total recebe o tamanho do diretorio atual

    Dir *temp= dir->pFilho; //ponteiro temp vai apontar pro 1o arquivo do diretorio
    while (temp != NULL){
        total+= obterTamanho(temp); //a funcao calcula o tamanho total (temp) do diretorio apontado e   adicionada a TOTAL
        temp= temp->prox; //atualiza temp e vai pro proximo arquivo
    }
    return total;
}
