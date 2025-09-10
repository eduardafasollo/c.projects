#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArvoreNL.h" 

//MENU funcao
int main(){
	
    int inicial=1;  //criacao inicial
	
    Dir*raiz= criarDir("raiz", 0, inicial);
    Dir*atual = raiz;
    
    inicial= 0;
    //inicial como 0 depois de criar o diretorio raiz

    int opcao;

    do{
        printf("\n -- MENU --\n");
        printf("1. Nome do Diretorio Atual \n");
        printf("2. Lista Diretorio \n");
        printf("3. Abrir Diretorio \n");
        printf("4. Voltar Diretorio \n");
        printf("5. Criar um Diretorio \n");
        printf("6. Excluir um Diretorio \n");
        printf("7. Tamanho do Diretorio \n");
        printf("8. Criar um Arquivo \n");
        printf("9. Excluir um Arquivo \n");
        printf("10. Sair \n");
        
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao); 

        switch(opcao){
            case 1:
                printf("Diretorio atual: %s \n", atual->nome);
                break;
                
            case 2:
                printf("Conteudo do diretorio %s: \n", atual->nome);
                imprimirEstrutura(atual,1);
                break;
                
            case 3: {
                char nome[100];
                printf("Digite o nome do diretorio para abrir: ");
                scanf("%s", nome);
                
                Dir *novoAtual= buscarDir(atual, nome);
                if(novoAtual!=NULL) {
                    atual= novoAtual;
                    printf("Diretorio %s aberto. \n", atual->nome);
                }else{
                    printf("Diretorio nao encontrado!\n");
                }
                break; }
                
            case 4: {
                if(atual != raiz) {
                    Dir *pai= encontrarPai(raiz, atual);
                    if(pai != NULL){
                        atual= pai;
                        printf("Voltando para o diretorio: %s\n", atual->nome);
                    }
                }else{
                    printf("Diretorio raiz ja esta aberto! \n");
                }
                break; }
				
			case 5: {
                char nome[100];
                printf("Digite o nome do novo diretorio: ");
                scanf("%s", nome);
                Dir *novoDir = criarDir(nome, 0, inicial);
                adicionarFilho(atual, novoDir);
                break; }
                
            case 6: { 
                char nome[100];
                printf("Digite o nome do diretorio para excluir: ");
                scanf("%s", nome);
                
                removerDir(atual, nome);
                break; }
            
            case 7: 
                printf("Tamanho do diretorio '%s': %d\n", atual->nome, obterTamanho(atual));
                break;
                
            case 8: {
                char nome[100];
                int tam;
                printf("Digite o nome do arquivo: ");
                scanf("%s", nome);
                
                printf("Digite o tamanho do arquivo: ");
                scanf("%d", &tam);
                
                criarArquivo(nome, tam);
                break; }
                
            case 9: {
                char nome[100];
                printf("Digite o nome do arquivo para excluir: ");
                scanf("%s", nome);
                excluirArquivo(nome);
                break; }
                
            case 10:
                printf("\nSaindo do programa...\n");
                break;
                
            default:
                printf("Opcao invalida!\n");
                break;
            }
            
    } while(opcao != 10);

    //libera memoria alocada para a estrutura de diret rios
    removerDir(raiz, raiz->nome);
    free(raiz);

    return 0;
	}
    


