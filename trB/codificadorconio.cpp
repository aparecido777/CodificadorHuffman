#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h> // Visual
#include"conio2.h"
 
#define COR_RESET   "\033[0m"
#define COR_VERDE   "\033[32m"
#define COR_AMARELO "\033[33m"

#define MAX_PALAVRA 50
#define MAX_CODIGO 256
#define MAX_LISTA 200

typedef struct Registro {
    int simbolo;
    char palavra[MAX_PALAVRA];
    int freq;
    char codigo_huffman[MAX_CODIGO];
} Registro;

typedef struct arvH {
    int freq;
    int simbolo; 
    struct arvH *esq;
    struct arvH *dir;
} ArvH;

typedef struct nolista {
    ArvH *arvore;
    struct nolista *prox;
} NoLista;

typedef union {
    unsigned char byte;
    struct {
        unsigned char b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
    } bits; 
} UByte;
 

int pegaPalavra(char texto[], int i, char palavra[]) {
    int e = 0;
    if (texto[i] == ' ') {
        palavra[0] = ' ';
        palavra[1] = '\0';
        i++;
    } else {
        while (texto[i] != ' ' && texto[i] != '\0') {
            char c = texto[i];
            int eh_letra = 0;
            if (c >= 'a' && c <= 'z') eh_letra = 1;
            if (c >= 'A' && c <= 'Z') eh_letra = 1;
            if ((unsigned char)c > 127) eh_letra = 1; 

            if (eh_letra == 1) {
                palavra[e] = tolower(c);
                e++;
            }
            i++;
        }
        palavra[e] = '\0';
    }
    return i;
}

void contar(char texto[], Registro lista[], int *total, int *prox_id) {
    char temp[MAX_PALAVRA];
    int i = 0;
    int fim_texto = 0;
    
    printf("   > Lendo texto base e calculando frequencias... ");
    
    while (fim_texto == 0) {
        i = pegaPalavra(texto, i, temp);
        if (strlen(temp) > 0) {
            int achou = 0;
            int j = 0;
            while (j < *total && achou == 0) {
                if (strcmp(lista[j].palavra, temp) == 0) {
                    achou = 1;
                } else {
                    j++;
                }
            }
            if (achou == 1) {
                lista[j].freq++;
            } else {
                lista[*total].simbolo = (*prox_id)++;
                strcpy(lista[*total].palavra, temp);
                lista[*total].freq = 1;
                (*total)++;
            }
        }
        if (texto[i] == '\0') fim_texto = 1;
    }
    cor(VERDE, PRETO); printf("[OK]\n");
}

ArvH* criaFolha(int freq, int simbolo) {
    ArvH *n = (ArvH*)malloc(sizeof(ArvH));
    n->freq = freq;
    n->simbolo = simbolo;
    n->esq = NULL;
    n->dir = NULL;
    return n;
}

ArvH* criaNoInterno(ArvH *esq, ArvH *dir) {
    ArvH *n = (ArvH*)malloc(sizeof(ArvH));
    n->freq = esq->freq + dir->freq;
    n->simbolo = -1;
    n->esq = esq;
    n->dir = dir;
    return n;
}

void insereNoLista(NoLista **cabeca, ArvH *arvore) {
    NoLista *novo = (NoLista*)malloc(sizeof(NoLista));
    novo->arvore = arvore;
    novo->prox = NULL;
    if (*cabeca == NULL || arvore->freq < (*cabeca)->arvore->freq) {
        novo->prox = *cabeca;
        *cabeca = novo;
    } else {
        NoLista *aux = *cabeca;
        while (aux->prox != NULL && aux->prox->arvore->freq <= arvore->freq) {
            aux = aux->prox;
        }
        novo->prox = aux->prox;
        aux->prox = novo;
    }
}

ArvH* removePrimeiro(NoLista **cabeca) {
    ArvH *arvore = NULL;
    if (*cabeca != NULL) {
        NoLista *temp = *cabeca;
        arvore = temp->arvore;
        *cabeca = temp->prox;
        free(temp);
    }
    return arvore;
}

ArvH* geraArv(NoLista **cabeca) {
    ArvH *esq, *dir, *novo_no;
    while (*cabeca != NULL && (*cabeca)->prox != NULL) {
        esq = removePrimeiro(cabeca);
        dir = removePrimeiro(cabeca);
        novo_no = criaNoInterno(esq, dir);
        insereNoLista(cabeca, novo_no);
    }
    return removePrimeiro(cabeca);
}

void geraCodigos(ArvH *raiz, char codigo[], int nivel, Registro lista_registros[], int total_registros) {
    if (raiz->esq == NULL && raiz->dir == NULL) {
        codigo[nivel] = '\0';
        int i = 0;
        int achou = 0;
        while (i < total_registros && achou == 0) {
            if (lista_registros[i].simbolo == raiz->simbolo) {
                strcpy(lista_registros[i].codigo_huffman, codigo);
                achou = 1;
            }
            i++;
        }
    } else {
        codigo[nivel] = '0';
        geraCodigos(raiz->esq, codigo, nivel + 1, lista_registros, total_registros);
        codigo[nivel] = '1';
        geraCodigos(raiz->dir, codigo, nivel + 1, lista_registros, total_registros);
    }
}


void exibeTabelaCompleta(Registro lista[], int total) {
    printf("\n");
    cor(CIANO, PRETO);
    printf("  %c", 201); for(int i=0;i<40;i++) printf("%c", 205); printf("%c\n", 187);
    printf("  %c TABELA DE CODIGOS (%02d Palavras)       %c\n", 186, total, 186);
    printf("  %c", 204); for(int i=0;i<40;i++) printf("%c", 205); printf("%c\n", 185);
    
    cor(BRANCO, PRETO);
    printf("  %c ID | PALAVRA      | FREQ | CODIGO      %c\n", 186, 186);
    
    // imprime todas as linhas 
    for (int i = 0; i < total; i++) {
        cor(BRANCO, PRETO);
        printf("  %c ", 186);
        
        cor(CINZA_CLARO, PRETO);
        printf("%02d | ", lista[i].simbolo);
        
        if(strcmp(lista[i].palavra, " ") == 0)
            printf("[Espaco]     ");
        else
            printf("%-12.12s ", lista[i].palavra);
            
        printf("| %02d   | ", lista[i].freq);
        
        cor(AMARELO, PRETO);
        printf("%-11s ", lista[i].codigo_huffman);
        
        cor(BRANCO, PRETO);
        printf("%c\n", 186);
    }
    
    cor(CIANO, PRETO);
    printf("  %c", 200); for(int i=0;i<40;i++) printf("%c", 205); printf("%c\n", 188);
    cor(BRANCO, PRETO);
}
 void exibeArvoreVertical(ArvH *raiz, char prefixo[], int ehUltimo) {
    if (raiz == NULL) return;
    printf("%s", prefixo);
    if (ehUltimo) {
        printf("+-- ");
    } else {
        printf("+-- ");
    }

    if (raiz->simbolo != -1) {
        printf(COR_VERDE "(ID:%d) %d" COR_RESET "\n", raiz->simbolo, raiz->freq);
    } else {
        printf(COR_AMARELO "<%d>" COR_RESET "\n", raiz->freq);
    }

    char novoPrefixo[512];
    strcpy(novoPrefixo, prefixo);
    if (ehUltimo)
        strcat(novoPrefixo, "    ");
    else
        strcat(novoPrefixo, "¦   ");
    int temEsq = raiz->esq != NULL;
    int temDir = raiz->dir != NULL;
    if (raiz->dir != NULL)
        exibeArvoreVertical(raiz->dir, novoPrefixo, !temEsq);
    if (raiz->esq != NULL)
        exibeArvoreVertical(raiz->esq, novoPrefixo, 1);
}

void gravaRegistrosBinario(Registro lista[], int total) {
    FILE *arq = fopen("registros.dat", "wb");
    if (arq) {
        fwrite(&total, sizeof(int), 1, arq);
        fwrite(lista, sizeof(Registro), total, arq);
        fclose(arq);
        printf("   > Gravando registros.dat... ");
        cor(VERDE, PRETO); printf("[OK]\n");
    }
}

void codificaEGrava(const char frase[], Registro lista[], int total) {
    char temp[MAX_PALAVRA];
    int i = 0, fim = 0, bit_count = 0;
    UByte u;
    u.byte = 0;
    FILE *arq = fopen("codificado.dat", "wb");
    
    printf("   > Codificando frase: ");
    cor(AMARELO, PRETO); printf("\"%s\"\n", frase);
    cor(BRANCO, PRETO);
    
    if (arq) {
        while (fim == 0) {
            i = pegaPalavra((char*)frase, i, temp);
            if (strlen(temp) > 0) {
                int j = 0;
                int achou = 0;
                while (j < total && achou == 0) {
                    if (strcmp(lista[j].palavra, temp) == 0) {
                        char *c = lista[j].codigo_huffman;
                        int k = 0;
                        while (c[k] != '\0') {
                            if (c[k] == '1') {
                                u.byte |= (1 << (7 - bit_count));
                            }
                            bit_count++;
                            if (bit_count == 8) {
                                fwrite(&u.byte, sizeof(unsigned char), 1, arq);
                                u.byte = 0;
                                bit_count = 0;
                            }
                            k++;
                        }
                        achou = 1;
                    }
                    j++;
                }
            }
            if (frase[i] == '\0') fim = 1;
        }
        if (bit_count > 0) fwrite(&u.byte, sizeof(unsigned char), 1, arq);
        fclose(arq);
        printf("   > Gravando codificado.dat... ");
        cor(VERDE, PRETO); printf("[OK]\n");
    }
}
 
int main() {
    SetConsoleOutputCP(437);
    esconderCursor();
    system("cls");
    
 
    printf("\n");
    centralizar2("HUFFMAN ENCODER 1.0 - ESTRUTURA DE DADOS II", CIANO);
    centralizar2("Codificador de Texto para Binario", ROSA);
    printf("\n");
    linhaDivisoria(80);

    Registro lista[MAX_LISTA];
    int total = 0;
    int prox_id = 0;
    
    char texto[] = 
        "O tempo passa e o tempo voa quando a gente sonha e quando a gente vive "
        "Sonhar e viver e aprender a cada dia e cada dia e uma nova chance "
        "Uma nova chance de fazer a escolha certa a escolha de viver bem e sonhar alto "
        "O tempo nao espera por ninguem e ninguem pode parar o tempo "
        "Viver e curtir cada momento como se fosse o ultimo momento de um sonho "
        "A vida e feita de escolhas e cada escolha tem uma consequencia e um motivo "
        "O tempo cura tudo mas o tempo tambem faz esquecer os problemas "
        "Esquecer os problemas e lembrar dos sonhos e lembrar de viver "
        "Sonhar com um futuro melhor e viver o presente com alegria e paz "
        "A alegria de estar vivo e de poder sonhar todos os dias "
        "O tempo e o mestre de todas as coisas e a vida e a escola do tempo "
        "Aprender a viver e a sonhar e o segredo da felicidade eterna "
        "Felicidade e ver o tempo passar sem medo de nao viver e medo de nao sonhar "
        "Quem sonha vive e quem vive sonha e quem ama vive dobrado "
        "O tempo corre e a gente corre com o tempo para alcancar a vida "
        "Correr para alcancar os sonhos e correr para viver a vida intensamente";

    
    contar(texto, lista, &total, &prox_id);

    NoLista *cabeca = NULL;
    int i = 0;
    while (i < total) {
        insereNoLista(&cabeca, criaFolha(lista[i].freq, lista[i].simbolo));
        i++;
    }

    ArvH *raiz = geraArv(&cabeca);
    char buf[MAX_CODIGO];
    geraCodigos(raiz, buf, 0, lista, total);

   
    exibeTabelaCompleta(lista, total);
    printf("\n");

   
    gravaRegistrosBinario(lista, total);

   
    codificaEGrava("viver o tempo e sonhar o futuro", lista, total);

  
    printf("\n");
    cor(BRANCO, PRETO);
    linhaDivisoria(40);
    cor(BRANCO, PRETO);
    printf(" ESTRUTURA VISUAL DA ARVORE:\n");
    linhaDivisoria(40);
    exibeArvoreVertical(raiz, "", 1);

    printf("\n\n");
    system("pause");
    return 0;
}
