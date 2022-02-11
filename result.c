#include "rpg42.h"

void    operate_status(t_user *user, t_subject operate){
    user->status->exp += operate.reward.stat.exp;
    user->status->activ_point += operate.reward.stat.activ_point;
    user->status->dex += operate.reward.stat.dex;
    user->status->fame += operate.reward.stat.fame;
    user->status->intel += operate.reward.stat.intel;
    user->status->luck += operate.reward.stat.luck;
    user->status->mental += operate.reward.stat.mental;
    user->status->fighting_point += operate.reward.stat.fighting_point;
    user->status->level += operate.reward.stat.level;
}

void    subject_success(t_user *user)
{
    int kb = 0;
    int lev_up = 0;
    t_subject   *work;

    work = user->sub_list->personal;
    operate_status(user, *work);
    lev_up = check_level_up(user);
    user->sub_list->cur_personal++;
    while (1)
    {
        print_header();
        printf("       %s를 통과했습니다!!!\n\n", work->event.title);
        for (int i = 0; i < lev_up; i++)
            printf("       레벨이 올랐습니다!\n");
        printf("\n");
        go_esc("뒤로 가기");
        print_footer();
        kb = linux_kbhit();
        if (kb == 27)
        {
            user->sub_list->personal++;
            return ;
        }
    }
}

void    print_fail_massage(int index)
{
    int     ran = 5 - index;
    char    norm[5][100] = {"연산자 이후 띄어쓰기를 하지 않았습니다...", "탭이 들어갈 자리에 스페이스가 들어갔습니다...", "헤더를 까먹고 안넣었습니다...", "힌 즐에 여러 번 대입을 사용했습니다...", "들여쓰기가 제대로 되지 않았습니다..."};
    char    compile[5][100] = {"-Wall : 사용되지 않은 변수로 컴파일 실패...", "함수에 return이 없어 컴파일에 실패했습니다...", "매개변수를 제대로 넣지 않아 컴파일에 실패했습니다...", "자료형을 잘못 사용해 컴파일에 실패했습니다...", "dose not compile! 이유를 알 수 없습니다..."};
    char    ect[5][100] = {"segmentation fault!! 프로그램이 제대로 실행되지 않습니다...", "BUS ERROR!! 버스타고 집에 가고싶습니다...", "", "", ""};
    
    if (index < 2)
    {
        printf("       norm error!!\n");
        printf("       %s\n", norm[index]);
    }
    else
    {
        printf("       compile error!!\n");
        printf("       %s\n", compile[ran]);
    }
}

void    subject_fail(t_user *user)
{
    int kb = 0;
    int ran = 0;
    int lev_up = 0;
    t_status operate;
    t_subject   *work;

    work = user->sub_list->personal;
    ran = rand() % 5;
    user->status->mental--;
    user->status->activ_point--;
    work->stat.success += user->status->mental;
    work->stat.percent -= 10;
    while (1)
    {
        print_header();
        print_fail_massage(ran);
        printf("\n\n\n\n");
        go_esc("뒤로 가기");
        print_footer();
        kb = linux_kbhit();
        if (kb == 27)
            return ;
    }
}

void    print_success_massage_peer_eval(point)
{
    
}

void    great_eval(t_user *user, t_event_day *day, int point)
{
    user->status->intel += point;
    print_screen(user, day, "코딩의 신과 동료평가를 진행했습니다. cs지식이 상승합니다!", "뒤로 가기");
}

void    good_eval(t_user *user, t_event_day *day, int point)
{
    user->status->intel += point / 2;
    print_screen(user, day, "좋은 동료와 동료평가를 진행했습니다. cs지식이 소폭 상승합니다!", "뒤로 가기");
}

void    bad_eval(t_user *user, t_event_day *day, int point)
{
    print_screen(user, day, "건성건성 하는 동료와 평가를 진행했습니다. 어떻게든 평가를 하긴 했습니다.", "뒤로 가기");
}