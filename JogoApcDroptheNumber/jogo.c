#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <math.h>

struct ranks
{
    int rank;
    char nome[10];
    struct ranks* next;
};
CONSOLE_SCREEN_BUFFER_INFO csbi;
int columns, rows;

//Geral:
int certeza();
int DimencionarTela();

// Coisas do Menu:
int menu();
void construirmenu();
void printmenu();

// Coisas do Jogo:
int jogo();
int adjacentester();
int administrarniveis();
int construirjogo();
void perguntarnomejogo();

//Coisa dos ranks:
struct ranks* organizar();
int rank();
void criarank();

int main()
{
    DimencionarTela();
    system("cls");
    int tela = 0;
    while(1)
    {
        tela = menu();
        if (tela == 0)
        {
            return 0;
        }
        else if (tela == 1)
        {
            tela = jogo();
            if (tela)
            {
                return 0;
            }
        }
        else if (tela == 2)
        {
            tela = rank();
        }
    }
}

int menu()
{
    int atualizar = 1;

    int posicao = 3;
    
    int n = 0;

    while (1)
    {
        if (atualizar == 1)
        {
            construirmenu(columns,rows ,posicao);
            atualizar = 0;
        }

        atualizar = DimencionarTela();

        if (kbhit())
        {
            n = getch();
            switch(n)
            {
                case 'w':
                    if (posicao != 3)
                    {
                        posicao+=1;
                        atualizar = 1;
                    }
                break;

                case 's':
                    if (posicao != 1)
                    {
                        posicao-=1;
                        atualizar = 1;
                    }   
                break;

                case 13:
                    if (posicao == 1)
                    {
                        system("cls");
                        return 0;
                    }else if (posicao == 3)
                    {
                        system("cls");
                        return 1;
                    }else // posicao = 2
                    {
                        system("cls");
                        return 2;
                    }
                break;
            }
        }
    }
}
void construirmenu(int colunas, int linhas, int selected)
{
    system("cls");
    for (int i = 0; i < (linhas-8)/2; ++i)
    {
        printf("\n");
    }
    if (selected == 1)
    {
        printmenu(colunas, "MENU\n");
        printmenu(colunas, " ");
        printmenu(colunas, "Play\n");
        printmenu(colunas, "Rank\n");
        printmenu(colunas, "[Sair]\n");
    }
    else if (selected == 2)
    {
        printmenu(colunas, "MENU\n");
        printmenu(colunas, " ");
        printmenu(colunas, "Play\n");
        printmenu(colunas, "[Rank]\n");
        printmenu(colunas, "Sair\n");
    }
    else if (selected == 3)
    {
        printmenu(colunas, "MENU\n");
        printmenu(colunas, " ");
        printmenu(colunas, "[Play]\n");
        printmenu(colunas, "Rank\n");
        printmenu(colunas, "Sair\n");
    }

}

void printmenu(int colunas, char*palavra) // Funcao usada no rank para colocar a palavras no meio da tela
{
    int tamanho = strlen(palavra);
    printf("\n");
    for (int i = 0; i < (colunas - tamanho)/2; ++i)
    {
        printf(" ");
    }
    printf("%s",palavra);
    for (int i = 0; i < (colunas - tamanho)/2; ++i)
    {
        printf(" ");
    }
}

int jogo()
{
    int addnum;
    double randaddnum;
    int win = 0;
    int contador = 1;
    struct ranks* first = 0;
    int temp;
    char ntemp[10];
    char nome[10];
    FILE * testef = fopen("numeros.txt","r");
    if (testef == 0)
    {
        srand((unsigned) time(NULL));
        testef = fopen("numeros.txt","w");
        for (int i = 0; i < 500; ++i)
        {
            randaddnum = pow(2,rand()%4+1);
            addnum = randaddnum;
            fprintf(testef, "%d\n", addnum);
        }
        fclose(testef);
    }
    fclose(testef);
    perguntarnomejogo(nome);
    system("cls");
    int pontos = administrarniveis(&win);
    

    FILE* rank = fopen("ranks.bin", "rb");
    while(fread(ntemp,sizeof(char),10,rank)!= 0)
    {
        fread(&temp,sizeof(int),1,rank);
        first = organizar(first, temp,ntemp);
        contador+=1;
    }
    first = organizar(first, pontos, nome);
    fclose(rank);
    rank = fopen("ranks.bin", "wb");
    for (int i = 0; i < contador; ++i)
    {
        fwrite(first->nome, sizeof(char),10,rank);
        fwrite(&first->rank, sizeof(int),1,rank);
        first = first->next;
    }
    fclose(rank);
    if (win)
    {
        return !certeza("PARABENS!!! VOCE ZEROU O JOGO!Quer continuar? ");
    }
    else
    {
        return !certeza("VOCE PERDEU! Quer continuar jogando???");
    }   
}

int administrarniveis(int * win)
{
    FILE * fnumeros = fopen("numeros.txt","r");
    int martelocont = 512;
    int pontos = 0;
    int tempnextnum;
    int tempnextnextnum;  
    int nextnum = 0;
    int nextnextnum = 0;   
    int martelos = 0;
    int n;
    int atualizar = 1;
    int selecionado = 1;
    int matriz[7][5];
    fscanf(fnumeros,"%d %d",&tempnextnum,&tempnextnextnum);
    while (tempnextnextnum > 0)
    {
        nextnextnum += 1;
        tempnextnextnum = tempnextnextnum/2;
    }
    while (tempnextnum > 0)
    {
        nextnum += 1;
        tempnextnum = tempnextnum/2;
    }

    for (int i = 0; i < 7; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            matriz[i][j] = 0;
        }
    }

    while (1)
    {
        if (pontos>martelocont)
        {
            if (martelos < 3)
            {
                martelos+=1;
            }
            martelocont= martelocont*2;
        }
        if (atualizar == 1)
        {
            construirjogo(matriz, selecionado,nextnum,nextnextnum,martelos,pontos);
            atualizar = 0;
        }

        atualizar = DimencionarTela();

        if (kbhit())
        {
            n = getch();        
            switch(n)
            {
                // <- 75,-> 77 ,^ 72, \/ 80
                case 'd':
                    if (selecionado < 5)
                    {
                        selecionado+=1;
                        atualizar = 1;
                    }
                break;

                case 'a':
                    if (selecionado > 1)
                    {
                        selecionado-=1;
                        atualizar = 1;
                    }   
                break;

                case 's':
                    for (int i = 0; i < 7; ++i)
                    {
                        if (matriz[i][selecionado-1] != 0)
                        {
                            if (i == 0)
                            {
                                fclose(fnumeros);
                                remove("numeros.txt");
                                return pontos;
                            }
                            else
                            {
                                matriz[i-1][selecionado-1] = nextnum;
                                adjacentester(matriz,i-1,selecionado-1,&pontos);
                            }
                            i = 7;
                        }
                        else if (i == 6)
                        {
                            matriz[i][selecionado-1] = nextnum;
                            adjacentester(matriz,i,selecionado-1,&pontos);
                            i = 7;
                        }
                    }
                    nextnum = nextnextnum;
                    if (fscanf(fnumeros,"%d",&tempnextnextnum) == EOF)
                    {
                        *win = 1;
                        fclose(fnumeros);
                        remove("numeros.txt");
                        return pontos;
                    }
                    nextnextnum = 0;
                    while (tempnextnextnum > 0)
                    {
                        nextnextnum += 1;
                        tempnextnextnum = tempnextnextnum/2;
                    }
                    atualizar = 1;
                break;

                case 'q':
                    if (martelos>0)
                    {
                        for (int i = 0; i < 7; ++i)
                        {
                            if (matriz[i][selecionado-1] != 0)
                            {
                                matriz[i][selecionado-1] = 0;
                                i = 7;
                            }
                        }
                        atualizar = 1;
                        martelos-=1;
                    }
                break;

                case 'z':
                    fclose(fnumeros);
                    remove("numeros.txt");
                    return pontos;
                break;
            }
        }
    }
    return 0;
}

int construirjogo(int matriz[][5], int selecionado , int nextnum, int nextnextnum,int martelos,int pontos)
{
    // i muda a linha j muda a coluna
    system("cls");
    int total = 0;
    int resp = 1;
    int numprint;
    int tamnum;

    for (int i = 0; i < (rows - 35) /2 -1; ++i)
    {
        printf("\n");
    }
    for (int j = 0; j < ((columns - 50)-2)/2 + selecionado * 10 - 4; ++j)
    {
        printf(" ");
    }
    while(nextnum > 0)
    {
        resp = resp *2;
        nextnum-=1;
    }
    printf(" %d \n",resp);
    for (int j = 0; j < ((columns - 50)-2)/2 + selecionado * 10 - 5; ++j)
    {
        printf(" ");
    }
    printf("%c%c%c%c%c\n",200,205,203,205,188);
    
    for (int i = 0; i < 35 + 2; ++i)
    {
        if (i < 5)
        {
            for (int j = 0; j < (columns - 82)/2-5; ++j)
            {
                printf(" ");
            }
            if (i == 0)
            {
                printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
            }
            if (i == 1)
            {
                printf("%c    Pontuacao:    %c",186,186);
            }
            if (i == 2)
            {
                printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",204,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,185);
            }
            if (i == 3)
            {
                printf("%c     %5d        %c",186,pontos,186);
                
            }  
            if (i == 4)
            {
                printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
            }
            if (i == 5)
            {
                printf("%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,187);
            }
        }
        else
        {
            for (int j = 0; j < (columns - 82)/2+20-5; ++j)
            {
                printf(" ");
            }
        }
        for (int j = 0; j < 50 + 2; ++j)
        {
            if (j == 0)
            {
                if (i != 0 && i != 36)
                {
                    printf("%c",186);
                }
                else if (i == 0)
                {
                    printf("%c",201);
                }
                else
                {
                    printf("%c",200);
                }
            }
            else if (j == 51)
            {
                if (i != 0 && i != 36)
                {
                    printf("%c",186);
                }
                else if (i!=0)
                {
                    printf("%c",188);
                }
                else
                {
                    printf("%c",187);
                }
            }
            else if(i == 0)
            {
                if ((j-1)/10 == selecionado-1 && (j-1)%10 == 6)
                {
                    printf("%c", 206);
                }
                else
                {
                    printf("%c", 205);
                }
            }
            else if (i == 36)
            {
                printf("%c", 205);
            }
            else if (matriz[(i-1)/5][(j-1)/10] != 0)
            {
                numprint = pow(2,matriz[(i-1)/5][(j-1)/10]);
                tamnum = 0;
                while(numprint>0)
                {
                    tamnum +=1;
                    numprint = numprint/10;
                }
                numprint = pow(2,matriz[(i-1)/5][(j-1)/10]);
                if ((j-1)%10 == 0 || (j-1)%10 == 9)
                {
                    if ((i-1)%5 == 0)
                    {
                        if ((j-1)%10 == 0)
                        {
                            printf("%c",201);
                        }
                        else
                        {
                            printf("%c",187);
                        }
                    }
                    else if((i-1)%5 == 4)
                    {
                        if ((j-1)%10 == 0)
                        {
                            printf("%c",200);
                        }
                        else
                        {
                            printf("%c",188);
                        }
                    }
                    else
                    {
                        printf("%c",186);
                    }
                }
                else if((i-1)%5 == 0 || (i-1)%5 == 4)
                {
                    printf("%c",205);
                }
                else if ((i-1)%5 == 2 && (j-1)%10 == 5)
                {
                    printf("%d",numprint);
                }
                else
                {
                    if (!((i-1)%5 == 2 && (j-1)%10 < tamnum))
                    {
                        printf(" ");
                    }
                }
            }
            else
            {
                printf(" ");
            }
        }
        if (i == 0)
        {
            printf("%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,187);
        }
        if (i == 1)
        {
            printf("%c  Next: %c",186,186);
        }
        if (i == 2)
        {
            printf("%c%c%c%c%c%c%c%c%c%c",204,205,205,205,205,205,205,205,205,185);
            
        }
        if (i == 3)
        {
            printf("%c%5.0lf   %c",186,pow(2.0,nextnextnum),186);
        }  
        if (i == 4)
        {
            printf("%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,188);
        }
        if (i == 5)
        {
            printf("%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,187);
        }
        if (i == 6)
        {
            printf("%cMartelos%c",186,186);
        }
        if (i == 7)
        {
            printf("%c%c%c%c%c%c%c%c%c%c",204,205,205,205,205,205,205,205,205,185);
        }
        if (i == 8)
        {
            if (martelos == 3)
            {
                printf("%c%c%c %c%c %c%c%c",186,178,178,178,178,178,178,186);
            }
            else if (martelos == 2)
            {
                printf("%c%c%c %c%c   %c",186,178,178,178,178,186);
            }
            else if (martelos == 1)
            {
                printf("%c%c%c      %c",186,178,178,186);
            }
            else
            {
                printf("%c        %c",186,186);
            }
        }
        if (i == 9)
        {
            printf("%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,188);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
    printf("\n");
    for (int j = 0; j < (columns - 56)/2; ++j)
    {
        printf(" ");
    }
    printf("S para descer o numero / Z to menu / Q para usar martelo\n");
}
int adjacentester(int matriz[7][5],int y,int x,int*pontos)
{
    int mudou = 0;
    int num = matriz[y][x];
    if (num == 0)
    {
        return 0;
    }
    if (x < 4 && matriz[y][x+1] == num)
    {
        matriz[y][x+1] = 0;
        matriz[y][x] +=1;
        mudou = 1;
    }
    if (y > 0 && matriz[y-1][x] == num)
    {
        matriz[y-1][x] = 0;
        matriz[y][x] +=1;
        mudou = 1;
    }
    if (x > 0 && matriz[y][x-1] == num)
    {
        matriz[y][x-1] = 0;
        matriz[y][x] +=1;
        mudou = 1;
    }
    if (y < 6 && matriz[y+1][x] == num)
    {
        matriz[y+1][x] = 0;
        matriz[y][x] +=1;
        mudou = 1;
    }
    if (mudou == 1)
    {
        *pontos+=pow(2,matriz[y][x]);
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                if (matriz[6 - j][i] == 0 && matriz[6 - j - 1][i] > 0)
                {
                    matriz[6 - j][i] = matriz[6 - j - 1][i];
                    matriz[6 - j - 1][i] = 0;
                }
            }
        }
        for (int j = 0; j < 7; ++j)
        {
            adjacentester(matriz,j,x,pontos);
        }
        if (x>0)
        {
            for (int j = 0; j < 7; ++j)
            {
                adjacentester(matriz,j,x-1,pontos);
            }
        }
        if (x<5)
        {
            for (int j = 0; j < 7; ++j)
            {
                adjacentester(matriz,j,x+1,pontos);
            }
        }
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 7; ++j)
            {
                adjacentester(matriz,j,i,pontos);
            }
        }
    }
    return 0;
}

struct ranks* organizar(struct ranks*first, int rank, char * nome)
{
    struct ranks* temp;
    if (first == 0)
    {
        temp = malloc(sizeof(struct ranks));
        temp->rank = rank;
        strcpy(temp->nome, nome);
        temp->next = 0;
        return temp;
    }
    else
    {
        if (first->rank < rank)
        {
            temp = malloc(sizeof(struct ranks));
            temp->rank = rank;
            strcpy(temp->nome, nome);
            temp->next = first;
            return temp;
        }
        else
        {
            first->next = organizar(first->next, rank,nome);
            return first;
        }
    }
}
void perguntarnomejogo(char * palavra)
{
    char temp[100];
    char c = 0;
    int contador = 0;
    system("cls");
    printf("\n");
    printf("\n");
    while(1)
    {
        
        for (int i = 2; i < rows/4; ++i)
        {
            printf("\n");
        }
        printf("    Digite o seu nome : ");
        while((c = getchar()) != EOF && c != '\n' && contador != 100)
        {
            if (contador >= 10)
            {
                contador = 99;
                system("cls");
                printf("\n    Nome muito grande!");
                printf("\n    Insira novamente");
            }
            else
            {
                palavra[contador] = c;
            }
            contador +=1; 
        }
        if (c!= '\n')
        {
            while((c = getchar()) != EOF && c != '\n');
        }
        if (contador != 100)
        {
            palavra[contador] = '\0';
            return;
        }
        contador=0;
    }
    return;
}

int rank()
{
    int posicao = 1;
    int atualizar = 1;
    int n = 0;

    FILE* rank;

    while (1)
    {

        if (atualizar == 1)
        {
            criarank(posicao);
            atualizar = 0;
        }

        atualizar = DimencionarTela();

        if (kbhit())
        {
            n = getch();
            switch(n)
            {
                case 'd':
                    if (posicao != 2)
                    {
                        posicao = 2; 
                        atualizar = 1;
                    }
                break;

                case 'a':
                    if (posicao != 1)
                    {
                        posicao = 1; 
                        atualizar = 1;
                    }
                break;

                case 13:
                    if (posicao == 1)
                    {
                        return 0;
                    }
                    else
                    {
                        if (certeza(" Tem certeza que quer apagar os ranks?"))
                        {
                            rank = fopen("ranks.bin", "wb");
                            fwrite(NULL,1,1,rank);
                            atualizar = 1;
                            fclose(rank);
                        }
                        atualizar = 1;
                    }
                    
                break;
            }
        }
    }
}
void criarank(int posicao)
{
    int linha = 3;
    int pontos;
    char palavra[20];
    char numero [10];

    system("cls");
    FILE * rank = fopen("ranks.bin", "rb");
    if (rank == NULL)
    {
        printf("Arquivo de ranks nao encontrado\n");
    }
    printmenu(columns, "RANK\n");
    while(fread(palavra,sizeof(char),10,rank)!= 0)
    {
        linha+=1;
        fread(&pontos,sizeof(int),1,rank);
        strcat(palavra," ");
        sprintf(numero,"%d",pontos);
        strcat(palavra,numero);
        printmenu(columns, palavra);
    }
    while(linha < rows -1)
    {
        linha+=1;
        printf("\n");
    }
    if (posicao == 1)
    {
        printf("      [sair]  apagar");
    }
    else
    {
        printf("       sair  [apagar]");
    }
    fclose(rank);
}

int DimencionarTela()
{
    int tcolumns, trows;

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    // Obter o handle (manipulador) da janela do console
    HWND consoleWindow = GetConsoleWindow();
    
    // Remover o estilo de barra de rolagem horizontal e vertical
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style &= ~WS_VSCROLL;  // Remove a barra de rolagem vertical
    style &= ~WS_HSCROLL;  // Remove a barra de rolagem horizontal
    SetWindowLong(consoleWindow, GWL_STYLE, style);
    
    // Atualizar a janela para aplicar as mudanças
    SetWindowPos(consoleWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

    GetConsoleScreenBufferInfo (GetStdHandle (STD_OUTPUT_HANDLE), &csbi);
    tcolumns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    trows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    if (columns != tcolumns || rows != trows)
    {
        columns = tcolumns;
        rows = trows;
        return 1;
    }    
    return 0;
}

int certeza(char *texto)
{
    int tamanho = strlen(texto);
    int posicao = 1;
    int atualizar = 1;
    int n = 0;

    while(1)
    {

        if (atualizar == 1)
        {
            system("cls");
            for (int i = 0; i < (rows-6)/2; ++i)
            {
                printf("\n");
            }
            for (int i = 0; i < (columns - 50)/2; ++i)
            {
                printf(" ");
            }
            printf("%c",201);
            for (int i = 0; i < 50; ++i)
            {
                printf("%c",205);
            }
            printf("%c",187);
            printf("\n");
            for (int j = 0; j < 5; ++j)
            {
                for (int i = 0; i < (columns - 50)/2; ++i)
                {
                    printf(" ");
                }
                printf("%c",186);

                if (j == 0)
                {
                    for (int k = 0; k < (50 - tamanho) / 2; ++k)
                    {
                        printf(" ");
                    }
                    printf("%s", texto);
                    for (int k = 0; k < (50 - tamanho) / 2; ++k)
                    {
                        printf(" ");
                    }
                }
                else if (j == 3)
                {   
                    if (posicao == 1)
                    {
                        for (int k = 0; k < 18; ++k)
                        {
                            printf(" ");
                        }
                        printf("[Nao]      Sim");
                        for (int k = 0; k < 18; ++k)
                        {
                            printf(" ");
                        }
                    }
                    else
                    {
                        for (int k = 0; k < 18; ++k)
                        {
                            printf(" ");
                        }
                        printf("Nao      [Sim]");
                        for (int k = 0; k < 18; ++k)
                        {
                            printf(" ");
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < 50; ++i)
                    {
                        printf(" ");
                    }
                }
                printf("%c\n",186);
            } 
            for (int i = 0; i < (columns - 50)/2; ++i)
            {
                printf(" ");
            }
            printf("%c",200);
            for (int i = 0; i < 50; ++i)
            {
                printf("%c",205);
            }  
            printf("%c",188);
            atualizar = 0;
        }

        atualizar = DimencionarTela();

        if (kbhit())
        {
            n = getch();
            switch(n)
            {
                case 'd':
                    if (posicao != 2)
                    {
                        posicao = 2; 
                        atualizar = 1;
                    }
                break;

                case 'a':
                    if (posicao != 1)
                    {
                        posicao = 1; 
                        atualizar = 1;
                    }
                break;

                case 13:
                    if (posicao == 2)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                    
                break;
            }
        }
    }
}
