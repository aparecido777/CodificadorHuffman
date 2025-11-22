#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include"conio2.h"


#define MAX_PALAVRA 50
#define MAX_CODIGO 256 

 
#define COR_AMARELO "\033[33m"
 
struct Registro {
	int simbolo;                    
    char palavra[MAX_PALAVRA];
    int freq;
    char codigo[MAX_CODIGO];
	
};typedef Registro Registro;



struct No {
    char palavra[MAX_PALAVRA];
    struct No *esq;
    struct No *dir;
    int folha; // 1 se for palavra(folha), 0 se for so caminho
};typedef No No;


No* criarNo() {
    No* novo = (No*)malloc(sizeof(No));
    novo->esq = NULL;
    novo->dir = NULL;
    novo->folha = 0;	
    return novo;
}

No* criaArvore(FILE *tabela) {
	
	int total;
	int i;
	char bit;
    No *raiz = criarNo();
    Registro regLido;
    
    fread(&total, sizeof(int), 1, tabela);
    fread(&regLido, sizeof(regLido), 1, tabela);
    while(!feof(tabela)) {
    	 i=0;
    	 No *atual = raiz;
    //	 printf("\nLendo do arquivo: Palavra=%s, Codigo=%s\n", regLido.palavra, regLido.codigo);
    	 while(regLido.codigo[i]!= '\0'	){
	    
	   //    printf("Montando: %s -> %s\n bit atual: %c\n", regLido.palavra, regLido.codigo, regLido.codigo[i]);
	            
	            bit = regLido.codigo[i];
	
	            if (bit == '0') {
	                //eqsq
	                if (atual->esq == NULL) {
	                    atual->esq = criarNo();
	                }
	                atual = atual->esq;
	            }
	            else { 
	                 //dir
	                if (atual->dir == NULL) {
	                    atual->dir = criarNo();
	                }
	                atual = atual->dir;
	            }
	            
	            i++; 
        } 
        strcpy(atual->palavra, regLido.palavra);
        atual->folha = 1; //fim  do caminhs
    	fread(&regLido, sizeof(regLido), 1, tabela);
	}
    return raiz;
}




 
void decodificar(FILE *fCodificado, No *raiz) {
    unsigned char byte;
    No *atual = raiz; 
	
	int cursorX = 4;
    int cursorY = 7; 
    int limiteX = 75; 
	int bit;
	
	fread(&byte, sizeof(unsigned char), 1, fCodificado);
    while (!feof(fCodificado)) {
     
        for (int i = 7; i >= 0; i--) {
        	
        	if(atual!=NULL){
	            bit = (byte >> i) & 1;
	
	            if (bit==0) 
	                atual = atual->esq;
	            else
		            atual = atual->dir;

	            
	            if (atual->folha == 1) {
	               
	            
                    gotoxy(cursorX, cursorY);
                    cor(BRANCO, PRETO); 
                    for(int c=0; c < strlen(atual->palavra); c++) {
                        printf("%c", atual->palavra[c]);
                        Sleep(20);
                    }
                  //  printf(" ");
                    
               
                    cursorX += strlen(atual->palavra) + 1;
                    
                    if(cursorX > limiteX) {
                        cursorX = 4;
                        cursorY++;
                        if(cursorY > 10) cursorY = 7;  
                    }
                    
                    atual = raiz;
	            }
            }
        }
        fread(&byte, sizeof(unsigned char), 1, fCodificado);
	}
}
void exibeArvoreVertical(No *raiz, char prefixo[], int ehUltimo, int bit) {
    if (raiz == NULL) return;

    printf("%s", prefixo);
    

    if (ehUltimo) {
    	
        printf(COR_AMARELO"+-- ");
    } else {
        printf(COR_AMARELO"+-- ");
    }

    
    if (bit == 0) { 
		cor(ROSA, PRETO);       
        printf( "0 " );
    } 
    else if (bit == 1) {
		cor(ROSA, PRETO);     
        printf("1 " );
    } 
    else {
    	cor(CIANO, PRETO);   
        printf( "R " );
    }
    if (raiz->folha == 1) {
    	cor(CIANO_CLARO, PRETO);
        printf( "-> [%s] \n", raiz->palavra);
    } else {
        printf(".\n");
    }
    char novoPrefixo[512];
    strcpy(novoPrefixo, prefixo);
    
    if (ehUltimo)
        strcat(novoPrefixo, "    ");
    else
        strcat(novoPrefixo, COR_AMARELO "|   "  );
        
    int temEsq = (raiz->esq != NULL);
    if (raiz->dir != NULL)
        exibeArvoreVertical(raiz->dir, novoPrefixo, !temEsq, 1);
    
    if (raiz->esq != NULL)
        exibeArvoreVertical(raiz->esq, novoPrefixo, 1, 0); 
}
int main(){
    
    centralizar(2, "HUFFMAN DECODER 2.0 - Estrutura de Dados II", CIANO, PRETO);
    centralizar(3, "Deodificador de Texto para BinarioI", ROSA, PRETO);
    printf("\n");
    linhaDivisoria(80);
printf("\n");
    gotoxy(3, 4); 
	printf("Frase Decodificada");
    moldura(2, 5, 78, 5);
    
    
    FILE *tabela = fopen("registros.dat", "rb");
    if (tabela == NULL) {
        printf("Erro: Nao encontrei 'registros.dat'. Rode o Programa 1 antes!\n");
    }
    FILE *codificado = fopen("codificado.dat", "rb");
    if (codificado == NULL) {
        printf("Erro: Nao encontrei 'codificado.dat'. Rode o Programa 1 antes!\n");
    }

	No *raiz = criaArvore(tabela);
    fclose(tabela);
    
    decodificar(codificado, raiz);
    fclose(codificado);

    gotoxy(1, 15); 
    cor(BRANCO, PRETO);
     cor(BRANCO, PRETO);
    linhaDivisoria(40);
    cor(BRANCO, PRETO);
    printf(" ESTRUTURA VISUAL DA ARVORE:\n");
    linhaDivisoria(40);
    exibeArvoreVertical(raiz, "", 1, -1);
   
  
    
  
    
    printf("\n\n");
    fclose(codificado);

    return 0;
}
