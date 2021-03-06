#include "rpg42.h"

void    operate_status(t_user *user, t_status operate, int score){
    user->status->exp += (operate.exp * score / 100);
    user->status->activ_point += (operate.activ_point * score / 100);
    user->status->dex += (operate.dex * score / 100);
    user->status->fame += (operate.fame * score / 100);
    user->status->intel += (operate.intel * score / 100);
    if (user->status->intel <= 0)
        user->status->intel = 1;
    user->status->luck += (operate.luck * score / 100);
    user->status->mental += (operate.mental * score / 100);
    user->status->fighting_point += (operate.fighting_point * score / 100);
    user->status->level += (operate.level * score / 100);
}

void    subject_success(t_user *user)
{
    int         kb = 0;
    int         lev_up = 0;
    int         cur;
    t_subject   *work;

    cur = user->sub_list->cur_personal;
    work = &user->sub_list->personal[cur];
    operate_status(user, work->reward.stat, work->stat.percent);
    print_result_screen(user, NULL, work->reward.stat, work->event.title, work->stat.percent);
    user->sub_list->cur_personal++;
}

void    print_fail_massage(void)
{
    int     i = rand() % 3;
    int     j = rand() % 5;
    char    norm[5][100] = {"연산자 이후 띄어쓰기를 하지 않았습니다...", "탭이 들어갈 자리에 스페이스가 들어갔습니다...", "헤더를 까먹고 안넣었습니다...", "힌 즐에 여러 번 대입을 사용했습니다...", "들여쓰기가 제대로 되지 않았습니다..."};
    char    compile[5][100] = {"-Wall : 사용되지 않은 변수로 컴파일 실패...", "함수에 return이 없어 컴파일에 실패했습니다...", "매개변수를 제대로 넣지 않아 컴파일에 실패했습니다...", "자료형을 잘못 사용해 컴파일에 실패했습니다...", "dose not compile! 이유를 알 수 없습니다..."};
    char    ect[5][100] = {"segmentation fault!! 프로그램이 제대로 실행되지 않습니다...", "BUS ERROR!! 버스타고 집에 가고싶습니다...", "알 수 없는 오류가 발생했습니다! 코딩의 신에게 저주받은 것 같습니다...", "알 수 없는 힘에 의해 파일이 사라졌습니다...", "뷁뷇뷁뷇뷁!!!!!"};
    
    if (i == 0)
    {
        printf("       norm error!!\n");
        printf("       %s\n", norm[j]);
    }
    else if (i == 1)
    {
        printf("       compile error!!\n");
        printf("       %s\n", compile[j]);
    }
    else
    {
        printf("       error!!\n");
        printf("       %s\n", ect[j]);
    }
}

void    subject_fail(t_user *user)
{
    int         kb = 0;
    int         cur;
    t_subject   *work;

    cur = user->sub_list->cur_personal;
    work = &user->sub_list->personal[cur];
    user->status->mental--;
    user->status->activ_point--;
    work->stat.success += user->status->mental;
    work->stat.percent -= 10;
    while (1)
    {
        print_header();
        print_fail_massage();
        printf("\n\n\n\n");
        go_next();
        print_footer();
        kb = linux_kbhit();
        basic_information_key(kb, user);
        if (kb == 'n')
            return ;
    }
}

void    print_result_screen(t_user *user, t_event_day *day, t_status exam, char *subject, int score)
{
    int kb = 0;

    while (1)
    {
        print_header();
            if (day != NULL)
                print_day_info(day, user);
        printf("\n\n");
        printf("       %s를 완료했습니다. 획득 점수 : %d\n\n", subject, score);
        if (exam.exp)
            printf("       %-6s %d 증가\n", "경험치", exam.exp * score / 100);
        if (exam.intel)
            printf("       %-6s %d 증가\n", "cs지식", exam.intel * score / 100);
        if (exam.dex)
            printf("       %-6s %d 증가\n", "dex", exam.dex * score / 100);
        if (exam.fame)
            printf("       %-6s %d 증가\n", "fame", exam.fame * score / 100);
        if (exam.luck)
            printf("       %-6s %d 증가\n", "luck",exam.luck * score / 100);
        if (exam.mental)
            printf("       %-6s %d 증가\n", "mental", exam.mental * score / 100);
        if (exam.fighting_point)
            printf("       %-6s %d 증가\n", "사기", exam.fighting_point * score / 100);
        if (exam.activ_point)
            printf("       %-6s %d 증가\n", "행동력", exam.activ_point * score / 100);
        if (exam.level)
            printf("       %-6s %d 증가\n", "레벨", exam.level * score / 100);
        check_level_up(user);
        printf("\n\n\n\n");
        go_next();
        print_footer();
        kb = linux_kbhit();
        basic_information_key(kb, user);
        if (kb == 'n')
            break;
    }
}

void    result_exam(t_user *user, t_event_day *day)
{
    int cur_exam = user->sub_list->cur_exam;
    int score = user->sub_list->exam[cur_exam].stat.hp;
    t_subject exam = user->sub_list->exam[cur_exam];

    operate_status(user, exam.reward.stat, 100 - exam.stat.hp);
    print_result_screen(user, day, exam.reward.stat, exam.event.title, 100 - score);
}

void    print_eval(t_user *user, t_event_day *day, int point)
{
    int     ran, suc, i, j;
    char    name[10][10] = {"jibae", "kipark", "gyepark", "silee", "cgim", "nheo", "donghuck", "jayoon", "jrim", "nakkim"};
    char    type[6][30] = {"코딩의 신", "좋은 동료", "귀염둥이 동료", "열정적인 동료", "클러스터의 무법자", "전설의 포켓몬"};

    i = rand() % 10;
    j = rand() % 6;
    ran = rand() % user->status->luck;
    suc = ran * point;
    if (suc > 20 && j == 0)
    {
        user->status->intel += point;
        user->status->fame++;
        print_peer_type(user, day, name[i], type[j], point);
    }
    else if (j == 0 || (suc >= 10 && j != 4) || (suc >= 10 && j != 3))
    {
        user->status->intel += point / 2;
        user->status->fame++;
        print_peer_type(user, day, name[i], type[j], point / 2);
    }
    else
    {
        user->status->intel -= point;
        user->status->fame--;
        if (user->status->intel < 1)
            user->status->intel = 1;
        print_peer_type(user, day, name[i], type[j], -point);
    }
}

void    print_peer_type(t_user *user, t_event_day *day, char *name, char *type, int point)
{
    int     kb = 0;
    char    last_word = name[strlen(name) - 1];
    while (1)
    {
        print_header();
        if (day != NULL)
            print_day_info(day, user);
        printf("\n\n\n\n");
        if (last_word =='a' || last_word == 'e' || last_word == 'o' || last_word == 'u' || last_word == 'i')
        {
            printf("       %s %s와 동료평가를 진행합니다. cs지식이 %d 상승합니다.\n", type, name, point);
            printf("       fame이 상승합니다.\n\n");
        }
        else
        {
            printf("       %s %s과 동료평가를 진행합니다. cs지식이 %d 상승합니다.\n", type, name, point);
            printf("       fame이 상승합니다.\n\n");
        }
        go_next();
        print_footer();
        kb = linux_kbhit();
        basic_information_key(kb, user);
        if (kb == 'n')
            return ;
    }
}

void    result_rush(t_user *user, t_event_day *day)
{
    int         kb;
    int         ran = rand() % 100;
    int         i = rand() % 5;
    int         cur = user->sub_list->cur_rush;
    int         score = user->sub_list->rush[cur].stat.comprehension / 3;
    t_status    reward = user->sub_list->rush[cur].reward.stat;

    if (ran >= 80 && score)
        print_result_screen(user, day, reward, user->sub_list->rush[cur].event.title, score);
    else
        while (1)
        {
            print_header();
            if (day != NULL)
                print_day_info(day, user);
            print_fail_massage();
            go_next();
            print_footer();
            kb = linux_kbhit();
            basic_information_key(kb, user);
            if (kb == 'n')
                return ;
        }
}