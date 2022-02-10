#include <stdlib.h>
#include <stdio.h>
# include <termios.h>
# include <unistd.h>
#include "rpg42.h"

int linux_kbhit(void);

void    print_header(void){
    system("clear");
    printf("########################################################################################\n");
    printf("\n\n");
}

void    print_footer(void){
    printf("\n\n");
    printf("########################################################################################\n");
}

void    go_esc(char *str){
    printf("        %s : (esc)\n", str);
}

void    go_next(char *str){
    printf("        %s : (n)\n", str);
}

void    input_error(void){
    printf("\n\n------------------------------------------------------------------\n");
    printf("\n\n\n");
    printf("       error(입력 오류) : 올바른 값을 입력해 주세요.\n\n\n");
    printf("\n------------------------------------------------------------------\n");
    printf("\n\n");
}

void    print_title(void){
    int kb = 0;

    while (!kb){
    print_header();
    printf("\n\n");
    printf("             _______   _______    ______         __    __   ______  \n");
    printf("             |       \\ |       \\  /      \\       |  \\  |  \\ /      \\ \n ");
    printf("             | $$$$$$$\\| $$$$$$$\\|  $$$$$$\\      | $$  | $$|  $$$$$$\n");
    printf("             | $$__/ $$| $$__| $$| $$ __\\$$      | $$__| $$ \\$$__| $$\n");
    printf("             | $$    $$| $$    $$| $$|    \\      | $$    $$ /      $$\n");
    printf("             | $$$$$$$ | $$$$$$$\\| $$ \\$$$$       \\$$$$$$$$|  $$$$$$ \n");
    printf("             | $$  | $$| $$|        $$__| $$            | $$| $$_____ \n");
    printf("             | $$  | $$| $$        \\$$    $$            | $$| $$     \\\n");
    printf("              \\$$ \\$$ \\$$         \\$$$$$$              \\$$ \\$$$$$$$$\n");
    printf("\n\n");
    print_footer();
    printf("press any key to start!");
    kb = linux_kbhit();
    }
}


char    select_charictor(void){
    char    chrtype;
    int     kb = 0;

    while (1)
    {
        print_header();
        printf("       캐릭터를 선택해 주세요.\n");
        printf("\n");
        printf("       ------------------------------------------------------------------------\n\n\n");
        printf("       a) 전공자 : 컴퓨터공학과를 다녀 컴퓨터 관련 지식을 상당히 가지고 있음\n\n");
        printf("       b) 비전공자 : 비전공자로 컴퓨터 공부를 처음 한다\n\n\n");
        printf("       ------------------------------------------------------------------------\n");
        print_footer();
        printf("\n\n\n");
        printf("       입력(a or b) :");
        kb = linux_kbhit();
        if (kb == 'a' || kb == 'b')
        {
            if (kb == 'a')
            {
                printf("'전공자'캐릭터를 선택하셨습니다.\n");
                chrtype = 'a';
            }
            else if (kb == 'b')
            {
                printf("'비전공자'캐릭터를 선택하셨습니다.\n");
                chrtype = 'b';
            }
            return (chrtype);
        }
        else
            input_error();
    }
}

int linux_kbhit(void)
{
    struct termios oldt, newt;
    int ch;

    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

    // switch(day){
    //             case monday:
    //                 break ;
    //             case thesday:
    //                 // 선택지 함수
    //                 break ;
    //             case weekend:
    // 				// 함수
    //                 break ;
    // 			default :
    // 				break;
    //         }
        
void	print_day_info(int day, t_user *user){
    printf("       %10s:%d%48s : %d\n", "day", day, "남은 행동력", user->status->activ_point);
    printf("                                                 s) %s, i) %s, e) %s\n", "status", "items", "equipment");
}

void    print_distractor(char act[3][20]){
    
    printf("              행동 목록\n\n");
    printf("       a ) %s\n", act[0]);
    printf("       b ) %s\n", act[1]);
    printf("       c ) %s\n\n", act[2]);
}

int    use_action_point(t_user *user)
{
    int use = 0;

    while (1)
    {
        print_header();
        printf("\n\n       남은 행동력 : %d\n\n", user->status->activ_point);
        printf("       사용할 행동력 : ");
        if (use > user->status->activ_point)
            input_error();
        scanf("%d", &use);
        if (use <= user->status->activ_point)
        {
            user->status->activ_point -= use;
            return use;
        }
        printf("\n\n\n\n");
        print_footer();
    }
    return (0);
}

void    basic_txt_print(t_user *user, int day, char *str)
{
    int kb = 0;

    while (1)
    {
        print_header();
        print_day_info(day, user);
        printf("\n\n\n\n\n\n\n\n");
        printf("       %s\n", str);
        go_esc("뒤로 가기");
        print_footer();
        kb = linux_kbhit();
        if (kb == 27)
            return ;
    }    
}

void    distractor_format(t_user *user, int day, char *go_to, char act[3][20])
{
    print_header();
    print_day_info(day, user);
    printf("\n\n\n\n\n\n\n\n");
    print_distractor(act);
    go_esc(go_to);
    print_footer();
}

void    basic_information_key(int kb, t_user *user)
{
    if (kb == 's')
        print_user_status(user);
    // else if (kb == 'i')
    //     print_item();
    // else if (kb == 'e')
    //     print_equip();
}