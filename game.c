#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void gameRules(){
    printf("\n<<====================Welcome to Rock, Paper ,Scissor Game=====================>>\n\n");
    printf("Game Rules:-\n");
    printf("1. Rocks beats Scissor,Scissor beats paper, paper beats Rock\n");
    printf("2.There will be total 3 rounds\n");
    printf("3.In every round Winner gets 2 points");
    printf("4.If it is draw then both gets 1 point\n");
    printf("\n<<===================================Start Game================================>>");

}

char generateChar(){
    char list[3]={'r','p','s'};
    srand(time(NULL));
    int random = rand()%3;
    return list[random];
}

int compare(char c1, char c2){
    if(c1==c2){
        return 0;
    }
    else if(c1=='r' && c2=='p'){
        return 1;
    }
    else if(c1=='p' && c2=='s'){
        return 1;
    }
    else if(c1=='s' && c2=='r'){
        return 1;
    }
    else{
        return -1;
    }
}
int main()
{
    gameRules();
    int playerscore=0,compscore=0;
    char playerchar , compchar;
    for(int i=1;i<=3;i++){
    
        printf("<<\n==================================Round %d====================================>>",i);

    // Taking player input

        printf("\nEnter 'r' for rock, 'p' for paper,'s' for Scissor\n\n");
        scanf("%c",&playerchar);
        getchar();
        printf("--------------------------------------\n>>");
        printf("|     You Choose %c|\n",playerchar);
        printf("-------------------------------------\n>>");

    // Generating Computer Character
    compchar = generateChar();

    printf("------------------------\n");
    printf(" |        Computer Chose: %c |\n",playerchar);
    printf("-----------------------\n\n");
    
    int result = compare(compchar,playerchar);
    if(result==0){
        playerscore++;
        compscore++;
        printf("\n Its Draw , Both gets 1 points\n");
    }
    else if(result == 1){
        compscore+=2;
        printf("\nYou Got This Round");
    }
    else{
        compscore+=2;
        printf("\n Computer Got this Round\n");
    }
    printf("\n\n---------------------------\n");
    printf("|       Your Score:%d       |\n",playerscore);
    printf("|       Computer Score:%d   |\n",compscore);
    printf("----------------------------\n\n");
    }

    printf("------------------------\n");
    printf("|       Final Score     | \n");
    printf("-------------------------\n");
    printf("|  You    |  Computer   |  \n");
    printf("-------------------------\n");
    printf("|    %d   |      %d      | \n",playerscore,compscore);
    printf("-------------------------\n");

    printf("----------------------------\n");
    if(playerscore>compscore){
        printf("|   You Win The Match   |\n");
    }
    else if(compscore>playerscore){
        printf("| Computer Win The Match |\n");
    }
    else{
        printf("|   This Match Is Draw   |\n");
    }
    printf("-----------------------------\n");
    return 0;
}