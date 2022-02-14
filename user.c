#include "rpg42.h"
void print_personal_progressbar(int persent);

void    print_user_status(t_user *user)
{
    int         exit;
    int         cur;
    user_type   type;

    cur = user->sub_list->cur_personal;
    type = user->type;
    while (1)
    {
        print_header();
        printf("               유저 정보");
        printf("\n\n\n\n");
        printf("     이름  : %s\n", user->user_name);
        if (type == 0)
            printf("     type  : %s\n", "전공자");
        else if (type == 1)
            printf("     type : %s\n", "비전공자");
        printf("     level : %d\n", user->status->level);
        printf("     exp   : %d\n", user->status->exp);
        printf("     현재 진행 중인 과제 : %s", user->sub_list->personal[cur].event.title);
        print_personal_progressbar(user->sub_list->personal[cur].stat.percent);
        printf("     ------------------------------------------------------------------------\n");
        printf("     %-15s: %d (컴퓨터 지식, 코딩 실력을 나타냅니다)\n", "cs", user->status->intel);
        printf("     %-15s: %d (민첩이 높다면 빠르게 움직입니다. 행동력을 더 많이 얻습니다.)\n", "dex", user->status->dex);
        printf("     %-15s: %d (\"life is unfair!\" 당신의 운을 나타냅니다)\n", "luck", user->status->luck);
        printf("     %-15s: %d (정신력이 낮으면 실수하기 쉽습니다)\n", "mental", user->status->mental);
        printf("     %-15s: %d (하루동안 사용할 수 있는 행동력입니다)\n", "active_point", user->status->activ_point);
        printf("     %-15s: %d (당신이 클러스터에서 얼마나 유명한지를 나타냅니다)\n", "fame", user->status->fame);
        printf("     %-15s: %d (사기가 낮으면 다음 날의 정신력이 감소합니다)\n", "fighting_point", user->status->fighting_point);
        printf("\n\n\n\n");
        go_next();
        print_footer();
        exit = linux_kbhit();
        if (exit == 'n')
            break ;
    }
}

void    level_up(t_user *user)
{
    user->status->level++;
    user->status->intel++;
    user->status->dex++;
    user->status->activ_point++;
    if (user->status->mental < 10)
        user->status->mental = 10;
}

int check_level_up(t_user *user)
{
    int i = 0;

    while (user->status->exp >= 100 * user->status->level)
    {
        user->status->exp -= (100 * (user->status->level));
        level_up(user);
        i++;
    }
    for (int j = 0; j < i; j++)
        printf("       레벨이 올랐습니다!\n");
    printf("\n");
    return (i);
}

void print_personal_progressbar(int percent)
{
    int temp = percent;
    printf(" [");
    for (int i = 0; i < 20; i++)
    {
        if (temp > 0)
            printf("█");
        else
            printf("-");
        temp -= 5;
    }
    printf("] %d%%\n", percent);
}