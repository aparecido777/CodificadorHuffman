 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>  
void gotoxy(int x, int y) {
    COORD c; c.X = x - 1; c.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
enum {
    PRETO=0, AZUL_ESC=1, VERDE=2, CIANO=3, VERMELHO=4, MAGENTA=5, MARROM=6, CINZA_CLARO=7,
    CINZA_ESC=8, AZUL_CLARO=9, VERDE_CLARO=10, CIANO_CLARO=11, VERMELHO_CLARO=12,
    ROSA=13, AMARELO=14, BRANCO=15
};


void cor(int texto, int fundo) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (fundo << 4) | texto);
}

void esconderCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100; info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
void linhaDivisoria(int largura) {
    for(int i=0; i<largura; i++) printf("%c", 205);
    printf("\n");
}

void centralizar2(const char* texto, int corTexto) {
    int len = strlen(texto);
    int padding = (80 - len) / 2;
    for(int i=0; i<padding; i++) printf(" ");
    cor(corTexto, PRETO);
    printf("%s\n", texto);
}
 
void moldura(int x, int y, int largura, int altura) {
    int i, j;
    cor(11, 0);  
 
    gotoxy(x, y); printf("%c", 218); 
    gotoxy(x + largura, y); printf("%c", 191);  
    gotoxy(x, y + altura); printf("%c", 192); 
    gotoxy(x + largura, y + altura); printf("%c", 217);   
 
    for(i = x + 1; i < x + largura; i++) {
        gotoxy(i, y); printf("%c", 196);
        gotoxy(i, y + altura); printf("%c", 196);
    }
 
    for(j = y + 1; j < y + altura; j++) {
        gotoxy(x, j); printf("%c", 179);
        gotoxy(x + largura, j); printf("%c", 179);
    }
}
void centralizar(int y, const char* texto, int corTexto, int corFundo) {
    int len = strlen(texto);
    int x = (80 - len) / 2;  
    gotoxy(x, y);
    cor(corTexto, corFundo);
    printf("%s", texto);
}
 
