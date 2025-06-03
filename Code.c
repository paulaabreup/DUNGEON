#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <stdbool.h>
#include <time.h>

#define lin 25//evite passar de 25 para nao ficar piscando o mapa
#define col 100
#define MAX_espinhos 30
#define portais 2


void gotoxy(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
   
typedef struct {
    int x, y;
} posicao;

bool proximos(posicao a, posicao b) {//comparar a distancia do player e do npc  
    return (abs(a.x - b.x) + abs(a.y - b.y)) == 1;
}

typedef struct{
    int vida;
    int vidamax;
    int dano;
    
}status;


/*EXEMPLO
                         ________________________________
                        |                                |
                        | [ Simbolos de cada objeto ]    |
                        |                                |
                        |Player = &                      |
                        |Npc = p                         |
                        |Parede = *                      |
                        |Chao = .                        |
                        |Chave = @                       |
                        |Botao = O                       |
                        |Porta = D                       |
                        |Teletransporte = >              |
                        |Mostro nivel 1 = X              |
                        |Mostro nivel 2 = V              |
                        |                                |
                        |________________________________|
                        

*/
    
void gerarmapa(char mapa[][col], posicao* p,posicao * npc,posicao* chave, posicao* posporta,status* player,status* npcstatuslv1,posicao* botao, int proxmapa);
void posicaoObjetos(char mapa[][col], posicao* p, posicao* chave,posicao* posporta, posicao* npc,posicao* botao);
void escreverLetraPorLetra(const char* texto, int delayMs);

void menu(char nomeB1[], void botao1(), char nomeB2[], void (*botao2)());
void play(int *opcao);
void creditos(int *opcao);
void vitoria();


    bool npc1;
    bool npc2;
    bool mover;
    bool porta = false;
    bool pegouChave = false;
    bool npcvivo = true;
    bool botaopress = false;
    bool espinhos = false;
    bool continuar = true;
    bool sair = false;
    bool dano = false;
    posicao espinhosxy[MAX_espinhos];
    posicao teleport[portais];
    posicao trilha[50];
    posicao npcInte = {0,0};
    
    int vidaantiga;
    int movimento = 0;
    int mapavez = 0;
    


int main() {


menu("PLAY",play,"CREDITOS",&creditos);


    return 0;
}



void menu(char nomeB1[], void botao1(), char nomeB2[], void (*botao2)()){

HANDLE cor = GetStdHandle(STD_OUTPUT_HANDLE);
   
int menux = 46,menuy = 8; 
int lin1x=0, lin2x = 0;
int op = 0;

char *menu[]= {nomeB1,nomeB2,"SAIR"};

char *tela[] = {
    " _______________________________ ",
    "|                               |",
    "|      Caverna do Dragao        |",
    "|                               |",
    "|                               |",
    "|                  [ DICA ]     |",
    "|                  Cima:  W     |",
    "|                  Baixo: S     |",
    "|                               |",
    "|_______________________________|",
};

int novolinx = lin1x;
gotoxy(menux,menuy);
printf(">-------------------------------<");
Sleep(500);
gotoxy(menux,menuy);
printf("                                 ");

for(int i = 0; i < 10;i++){
gotoxy(menux,novolinx+menuy+1);
printf(">-------------------------------<");

gotoxy(menux,menuy+i);
printf("%s", tela[i]);

novolinx++;
lin1x = novolinx;

Sleep(100);

printf("\n");
}

gotoxy(menux,lin1x+menuy);
printf("                                 ");

while(1){

for(int i = 0; i < 10;i++){

gotoxy(menux,menuy+i);
printf("%s", tela[i]);

printf("\n");
}



for(int i = 0; i < 3;i++){
gotoxy(menux+4,(menuy+5)+i);

if(i == op){
SetConsoleTextAttribute(cor,2);
printf("> %s <\n", menu[i]);
SetConsoleTextAttribute(cor,7);
}else{
printf("%s     \n", menu[i]);
}
}


while(kbhit())getch();
int tecla = getch();

if(tecla == 'w' && op > 0)op--;
if(tecla == 's'&& op < 2)op++;

if(tecla == 13){
switch (op){

case 0:
botao1(&op);
    break;

case 1:
botao2(&op);
    break;
}

if(op == 2){
gotoxy(47,16);
printf("(Tem Certeza?)");
tecla = getch();
if(tecla == 13){
break;
}
}

}

}
gotoxy(0,30);


}

void play(int *opcao){

do{
    system("cls");
    for(int i = 0 ; i < MAX_espinhos; i++){
    espinhosxy[i] = (posicao) {0,0};
    }

    srand(time(NULL));
    status player,npcstatuslv;
    player.vidamax = 100;
    vidaantiga = player.vidamax;
    posicao p = {0,0},chave = {0,0},npclv = {0,0},posporta = {0,0},botao = {0,0};

    char lobby[lin][col] = {

        "                                                                           ",
        "  ..................................&....................................  ",
        "  ..................................~..................*****.............  ",
        "  ..........************............~..................*.@.*.............  ",
        "  ..........*.P........*............~..................*...*.............  ",
        "  ..........*****...****............~..................*...*.............  ",
        "  ................~.................~..................*...*.............  ",
        "  ................~.................~....................................  ",
        "  ................~.................~....................................  ",
        "  ................~.................~....................................  ",
        "  ..******........~.................~....................................  ",
        "  ..*....*........~.................~....................................  ",
        "  ..*.P.....~.~.~.~.~.~.~.~.~.~.~.~.~....................................  ",
        "  ..*....*..........................~....................................  ",
        "  ..******..........................~....................................  ",
        "  ..................................~....................................  ",
        "  .......................P..........~....................................  ",
        "  ..................................~....................................  ",
        "  ..................................~....................................  ",
        "  ..................................~....................................  ",
        "  ..................................~....................................  ",
        "  ..................................~....................................  ",
        "  ..................................D....................................  ",
        "                                                                           ",
        
    };
    
    char mapaNivel1[lin][col] = {

        "                                                                           ",
        "               . . . . . . . . . . . . . . . . . . . . . . . . . .         ",
        "               .                                                 .         ",
        "               /* * * * * * * * * * * * *        * * * * * * *   .         ",
        "                &.......................*        *......@....*   .         ",
        "               /* * * ...* * * * *......*        *...........*   .         ",
        "               .     *...*       *......*        *...........*   .         ",
        "               .     *...*       *......*        *...........*   .         ",
        "               .     *...*       *.......* * * * * * * *...* * * .         ",
        "               .     *...*       *..............................*.         ",
        "               .     *...*       * * * * * * * * * * * * * * * * .         ",
        "               .     *...*                                       .         ",
        "               .     *...*                                       .         ",
        "               .     *...*                                       .         ",
        "               .     *...*                                       .         ",
        "               .     *...*                                       .         ",
        "               .     *...*                                       .         ",
        "               .     *...* * * * * * * * * * * * * * *           .         ",
        "               .     *...............................*           .         ",
        "               .     * * * * * * * * * * * * * * *...*           .         ",
        "               . . . . . . . . . . . . . . . . . .|D|. . . . . . .         ",
        
        

    };

    char mapaNivel2[lin][col] = {
        "                                                                           ",
        "                                   * *                                     ",
        "                                  *.&.*                                    ",
        "                                  *...*                                    ",
        "                                  *...*                                    ",
        "                                  *...*                                    ",
        "                                  *...*                                    ",
        "                                  *...*                                    ",
        "                                  *...**********                           ",
        "                         *****    *............*                           ",
        "                         *.@.*    *...*******..*                           ",
        "                         *...*    *...*     *..*                           ",
        "                    ******...*******O********..*                           ",
        "                    *..#.......................*                           ",
        "                    *.#.........................*                          ",
        "                    *##..#*******************....******                    ",
        "                    *..................*    *.........*                    ",
        "                    ****************...*    *......X..*                    ",
        "                                   *...*    *.........*                    ",
        "                                 ...|D|...  ***********                    ",
        "                                                                           ",
        "                                                                           ",
        "                                                                           ",
        "                                                                           ",
        "                                                                           ",

    };
    
    char mapaNivel3[lin][col] = {
        "                                                                                      ",
        "                                                         * * * * * * * * * * * * *    ",
        "                                                         *.................>.....*    ",
        "                                                         *.......................*    ",
        "                                                        *..........* * * * * * * *    ",
        "                                  * * * * * * * * *    *.........*                    ",
        "   * * *                          *...............*   *..........*                    ",
        " *...@....*                   *.................... *...........*                     ",
        " *......#...*                   *..............................*                      ",
        " *.#....#.....* * * * * * * * *....................... * * * *                        ",
        " *.....#...#..........................................*                               ",
        " *.........##..........................................* * * * * * * * * * * */       ",
        " *..#......##..#......#......O............................................&..*/       ",
        " *......#...* * * * * * * * * * * * * * *..............* * * * * * * * * * * */       ",
        " *>.#.....*                              *.............*                              ",
        " *.V..#..*                             *...............*                              ",
        " *.....*                             *................*                               ",
        " *..#*                             *... * * *.......*                                 ",
        " *...*                           *... *      *...*                                    ",
        " *...*                         *... *        *...*                                    ",
        " *...*                       *... *          *...*                                    ",
        " *...* * * * * * * * * * * *.....*           *...*                                    ",
        " *..............................*            *...*                                    ",
        " * * * * * * * * * * * * * * * *             *|D|*                                    ",
        "                                                                                      ",

    };

    
   

    while (continuar) {//selecao dos mapas
    

    system("cls");
        switch(mapavez){

    case 0:
    /*                   [ LOBBY ]                   */

    posicaoObjetos(lobby,&p,&chave,&posporta,&npclv, &botao);//posiciona os objetos usando de base a matriz
    

    player.vida = vidaantiga;
            
    pegouChave = false;// false = chaveNpega | true  = chavepega
    porta = false;//false = porta fechada | true = porta aberta
    npcvivo = true;// false = npc morto | true = npc vivo
    botaopress = false;
    espinhos = false;

    gerarmapa(lobby,&p,&npclv,&chave,&posporta,&player,&npcstatuslv,&botao,mapavez);//gerar mapa
        break;

        
    case 1:

    /*                  [ DUNGEON 1 ]                   */

    

    posicaoObjetos(mapaNivel1,&p,&chave,&posporta,&npclv,&botao);//posiciona os objetos usando de base a matriz

    player.vida = vidaantiga;
    
    pegouChave = false;// false = chaveNpega | true  = chavepega
    porta = false;//false = porta fechada | true = porta aberta
    npcvivo = true;// false = npc morto | true = npc vivo
    botaopress = false;
    espinhos = false;


    gerarmapa(mapaNivel1,&p,&npclv,&chave,&posporta,&player,&npcstatuslv,&botao,mapavez);//gerar mapa
         
        break;

    case 2:
    
    /*                  [ DUNGEON 2 ]                   */


    if(dano){ 
    dano = false;
    }

    posicaoObjetos(mapaNivel2,&p,&chave,&posporta,&npclv,&botao);//posiciona os objetos usando de base a matriz
    
    
    player.vida = vidaantiga;

    pegouChave = false;// false = chaveNpega | true  = chavepega
    porta = false;//false = porta fechada | true = porta aberta
    npcvivo = true;// false = npc morto | true = npc vivo
    botaopress = false;
    espinhos = false;

    gerarmapa(mapaNivel2,&p,&npclv,&chave,&posporta,&player,&npcstatuslv,&botao,mapavez);//gerar mapa

        break;


    case 3:
    /*                  [ DUNGEON 3 ]                   */

    if(dano){ 
    dano = false;
    }

    posicaoObjetos(mapaNivel3,&p,&chave,&posporta,&npclv, &botao);//posiciona os objetos usando de base a matriz
    
    
    player.vida = vidaantiga;
            
    pegouChave = false;// false = chaveNpega | true  = chavepega
    porta = false;//false = porta fechada | true = porta aberta
    npcvivo = true;// false = npc morto | true = npc vivo
    botaopress = false;
    espinhos = false;

    if(npc1){
    npcstatuslv.vida = 50;//vida do npc
    npcstatuslv.dano = rand() % 3+10;//dano do npc
    }
    if(npc2){
    npcstatuslv.vida = 150;//vida do npclv2
    npcstatuslv.dano = rand() % 7+13;//dano do npclv1
    }

    gerarmapa(mapaNivel3,&p,&npclv,&chave,&posporta,&player,&npcstatuslv,&botao,mapavez);//gerar mapa
        break;

    case 4:

    vitoria();

    break;


    
    }//switch

    
    }//loop


if(!sair){
continuar = true;
}
opcao = 0;

}while(continuar);

    getch();
    gotoxy(0,20);


}

void creditos(int *opcao){
    HANDLE cor = GetStdHandle(STD_OUTPUT_HANDLE);
   
int creditosx = 46,creditosy = 8; 

char *creditos[] = {
    " _______________________________ ",
    "|          [ CRIADORES ]        |",
    "|                               |",
    "|     Pedro Artur R. Novaes     |",
    "|      Francinetti Pessoa       |",
    "|        Flavia Martins         |",
    "|                               |",
    "|                               |",
    "|                               |",
    "|_______________________________|",
};



for(int i = 0; i < 10;i++){
gotoxy(creditosx,creditosy+i);
printf("%s\n", creditos[i]);
}

int tecla = getch();

*opcao = 0;

gotoxy(0,30);


}

void vitoria(){

char* vitoria[] = {

    "       * Vitoria *       ",
    "                         ",
    "  Parabens, voce ganhou  ",
    "    Obrigado por jogar   ",

};

for(int i = 0;vitoria[i] != NULL;i++){
gotoxy(40,3+i);
printf("%s\n", vitoria[i]);
}

int tecla = getch();
system("cls");
continuar = false; sair = true;

}



void gerarmapa(char mapa[][col], posicao* p,posicao * npc,posicao* chave, posicao* posporta,status* player,status* npcstatus,posicao* botao, int proxmap){ 


char *mapaMostrar[] = {"VILA", "DUNGEON (sala 1)", "DUNGEON (sala 2)", "DUNGEON (sala 3)"};

gotoxy(0,0);
printf("Mapa: %s", mapaMostrar[mapavez]);//mostra no canto superior esquerdo qual mapa 

    
HANDLE cor = GetStdHandle(STD_OUTPUT_HANDLE);
int vidamaxn = npcstatus->vida;




while(continuar){

    posicao novo;
    novo.x = p->x;
    novo.y = p->y;

    if(dano){
        continuar = false;
    }

        if (!pegouChave) {//PORTA | CHAVE
            if(chave->x != 0 && chave->y != 0)mapa[chave->x][chave->y] = '@';//chav
        } else {
            if(chave->x != 0 && chave->y != 0)mapa[chave->x][chave->y] = '.';//chave (dps de coletada)
            }

        if(!porta){
        if(posporta->x != 0 && posporta->y != 0)mapa[posporta->x][posporta->y] = 'D';//porta trancada
        }else{
        if(posporta->x != 0 && posporta->y != 0)mapa[posporta->x][posporta->y] = '=';//porta (dps de usar a chave)
        }
        
        // Atualiza personagens no mapa
        if (player->vida > 0 && p->x != 0 && p->y != 0) mapa[p->x][p->y] = '&';//player 
        if(npc1){
        if (npcstatus->vida > 0 && (npc->x != 0 && npc->y != 0) ) mapa[npc->x][npc->y] = 'X';// monstro
        }
        if(npc2){
        if (npcstatus->vida > 0 && (npc->x != 0 && npc->y != 0) ) mapa[npc->x][npc->y] = 'V';// monstro
        }
    


        // Exibe mapa
        for (int x = 0; x < lin; x++){
            gotoxy(22, x + 3);
            printf("%s\n", mapa[x]);
        }
        
        



        if (proximos(*p, *npc)) {
        dano = true;
        
        }
        if(mapa[p->x][p->y] == 'V' || mapa[p->x][p->y] == 'X'){
        dano = true;
        
        }

        
    

    int npcintx = 53,npcinty = 22;  
    bool conversando;
    if((p->x >= npcInte.x-1 && p->x <= npcInte.x+1) && (p->y >= npcInte.y-1 && p->y <= npcInte.y+1)){
    gotoxy(npcintx,npcinty);
    printf("  [ NPC ]  ");
    gotoxy(npcintx,npcinty+1);
    printf("'E' Falar");  
    
    }

            //funcao do player
            int tecla = getch();
            if (tecla == 'w') novo.x--;//Player anda para cima
            else if (tecla == 's') novo.x++;//Player anda para baixo
            else if (tecla == 'a') novo.y--;//Player anda para esquerda
            else if (tecla == 'd') novo.y++;//Player anda para direita
            
    if((p->x >= npcInte.x-1 && p->x <= npcInte.x+1) && (p->y >= npcInte.y-1 && p->y <= npcInte.y+1)){
    if(tecla == 'e')conversando = true;
    if(conversando){

    gotoxy(npcintx,npcinty);
    printf("           ");
    gotoxy(npcintx,npcinty+1);
    printf("         ");  
    
    char* mensagem = "Ola, vejo que voce e novo aqui ,olha  ja vou te alertando uma coisa ,preste atencao ao seu redor tem gente aqui   que nao e tao amigavel";
    escreverLetraPorLetra(mensagem,10);
    getch();
    conversando = false;
    }
    
    }
    
            if(novo.x == teleport[0].x && novo.y == teleport[0].y){
            gotoxy(0,5);
            mapa[p->x][p->y] = '.';
            p->x = teleport[1].x;
            p->y = teleport[1].y+1;
            }
            if(novo.x == teleport[1].x && novo.y == teleport[1].y){
            gotoxy(0,5);
            mapa[p->x][p->y] = '.';
            p->x = teleport[0].x;
            p->y = teleport[0].y+1;
            }

            

            if((p->x != 0 && p->y != 0)){//(so funciona se player existir na matriz)
                
                mapa[p->x][p->y] = '.';//apaga rastro do player

                
                //BOTAO 
                if(botao->x != 0 && botao->y != 0){

                if(novo.x == botao->x && novo.y == botao->y){//BOTAO PRESSIONADO
                gotoxy(0,0); printf("%s",botaopress ? "* Botao ja foi pressionado *": "* Botao Pressionado *");
                mapa[p->x][p->y] = '.';
                botaopress = true;
                espinhos = true;
                }
                else if(!botaopress ){//botao antes de ser pressionado
                mapa[botao->x][botao->y] = 'O';
                }else{//botao depois de ser pressionado
                gotoxy(0,0); printf("                            ");
                mapa[botao->x][botao->y] = 'o';
                }

                }
                //BOTAO 

                
            } //(so funciona se player existir na matriz)
        
            if(npc->x != 0 && npc->y != 0)mapa[npc->x][npc->y] = '.';//limpa rastro do npc(so funciona se npc existir na matriz)
            // Limpa posições anteriores


            if (mapa[novo.x][novo.y] == '.' || mapa[novo.x][novo.y] == 'O' || mapa[novo.x][novo.y] == 'o' 
                || mapa[novo.x][novo.y] == '#' || mapa[novo.x][novo.y] == '~') {//pode se mover se...
                *p = novo;
            } else {//se nao
                novo = *p;
            }

            //funcao do player



        
            if ((p->x >= chave->x - 1 && p->x <= chave->x + 1) &&
                (p->y >= chave->y - 1 && p->y <= chave->y + 1) && !pegouChave) {//interacao com a chave
                
                gotoxy(52, 1);
                printf("  [ CHAVE ]");
                
                gotoxy(52, 2);
                printf("[E interagir]");
                
            if (tecla == 'e') pegouChave = true;

            } else {
                gotoxy(52, 1); printf("             ");
                gotoxy(48, 2); printf("                       ");
            }


            if ((p->x >= posporta->x - 1 && p->x <= posporta->x + 1) &&
                (p->y >= posporta->y - 1 && p->y <= posporta->y + 1)) {//interacao com a porta
                

                if(pegouChave && !porta){
                gotoxy(52, 1);
                printf("  [ PORTA ]");
                
                gotoxy(48, 2);
                printf("[E interagir (ABRIR)]"); 

                if (tecla == 'e'){ 
                    porta = true;
                    tecla = 0;
                }
                }
                else{
                gotoxy(52, 1);
                printf("  [ PORTA ]");

                gotoxy(53, 2);
                printf(" *TRANCADA*");
                }

                if(porta){
                gotoxy(52, 1);
                printf("  [ PORTA ]");
                
                gotoxy(48, 2);
                printf("[E interagir (ENTRAR)"); 

                if (tecla == 'e'){ 
                    system("cls");
                    mapavez = proxmap+1;
                    vidaantiga = player->vida;
                    break;
                }
            }

            }

               

        if(movimento > 6){//sistema de 'cansaço'
        movimento = 0;
        mover = false;
        }else{
        movimento++;
        mover = true;
        }

        

        
        if(movimento > 3){//sistema de 'cansaço'
        movimento = 0;
        mover = false;
        }else{
        movimento++;
        mover = true;
        }

        
        int nx = npc->x,
            ny = npc->y;

        if(npc->x < p->x)nx++;
        if(npc->x > p->x)nx--;
        if(npc->y < p->y)ny++;
        if(npc->y > p->y)ny--;


            if ((mapa[nx][ny] == '.' || mapa[nx][ny] == '#' || mapa[nx][ny] == 'O') && mapa[nx][ny] != '&' ) {//npc pode andar

                npc->x = nx;
                npc->y = ny;
            }else{
                nx = npc->x;
                ny = npc->y;
            }
        // Movimento do NPC





        if(espinhos && (botao->x != 0 && botao->y != 0) && mapavez != 0){//espinhos ativados

    for(int i = 0 ; i < MAX_espinhos; i++){
    mapa[espinhosxy[i].x][espinhosxy[i].y] = '#';
    }

    if(mapa[p->x][p->y] == '#'){dano = true;}
    

    }

    if(mapavez == 0){

    for(int i = 0 ; i < 50; i++){
    mapa[trilha[i].x][trilha[i].y] = '~';
    }

    }





        

     
    }

    
    

    

    // Verifica fim de jogo



}

void posicaoObjetos(char mapa[][col], posicao* p, posicao* chave,posicao* posporta, posicao* npclv,posicao* botao){
int x = 0, t = 0, tp = 0;

    for(int i = 0; i < lin;i++){
    for(int j = 0; j < col;j++){
    
    

    if(mapa[i][j] == '&')p->x = i,p->y = j;//define a posicao inicial do player
    
    if(mapa[i][j] == '@')chave->x = i,chave->y = j;//define a posicao inicial da chave
    
    if(mapa[i][j] == 'D')posporta->x = i,posporta->y = j;//define a posicao inicial da chave

    if(mapa[i][j] == 'X'){npclv->x = i,npclv->y = j; npc1 = true;}//define a posicao inicial da chave

    if(mapa[i][j] == 'V'){npclv->x = i,npclv->y = j; npc2 = true;}//define a posicao inicial da chave

    if(mapa[i][j] == 'O'){botao->x = i, botao->y = j;}//define a posicao inicial do botao

    if(mapa[i][j] == '#'){espinhosxy[x] = (posicao) {i,j}; x++; mapa[i][j] = '.';}//define a posicao dos espinhos
    
    if(mapavez == 0){
    if(mapa[i][j] == '~'){trilha[t] = (posicao) {i,j}; t++;}//define a posicao da trilha
    }

    
    if(mapa[i][j] == '>'){teleport[tp] = (posicao) {i,j}; tp++;}//define a posicao dos espinhos
    
     if(mapa[i][j] == 'P'){npcInte.x = i, npcInte.y = j;}//define a posicao inicial do botao


    }
    }


}

void escreverLetraPorLetra(const char* texto, int delayMs) {
int letras = 40;
int linha = 0;
    for (int i = 0; i < strlen(texto); i++) {
    letras++;
    if(letras >= 38){
        gotoxy(40,18+linha);
        linha++;
        letras = 0;
    }
        putchar(texto[i]);
        fflush(stdout);       // Garante que a letra será impressa imediatamente
        Sleep(delayMs);       // Pausa em milissegundos
    
    }
}
