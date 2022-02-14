#include "rpg42.h"

//멘탈 관련 추가!!

void    basic_exam_print(t_user *user, t_event_day *day, char *str)
{
    int kb = 0;

    while (1)
    {
        print_header();
        printf("       %s:%d%68s : %d\n", "day", day->day, "남은 행동력(시간)", user->status->activ_point);
        printf("       %s:%-35d s)%s, i)%s, e)%s\n", "EXAM 진행도", 100 - user->sub_list->exam->stat.hp, "status", "items", "equipment");
        printf("       %s:%d\n", "retry 횟수", user->sub_list->exam->stat.try_cnt);
        printf("\n\n\n\n\n\n\n");
        printf("       %s\n", str);
        basic_information_key(kb, user);
        kb = linux_kbhit();
        if (kb == 'n')
            if (ask_exit(user, day))
                return ;
    }    
}

void    do_exam(t_user *user, t_event_day *day){
    int         kb = 0;
    int         ran = 0;
    int         use = 0;
    t_subject      *work;

    work = user->sub_list->exam;
    if (work->stat.done)
    {
        print_screen(user, day, "문제를 다 풀었습니다. 제출 해주세요.", "뒤로 가기");
        return ;
    }
    use = use_action_point(user);
    if (use == 0)
        return ;
    work->stat.done = 1;
    progress_message("문제", "진행");
    ran = rand() % user->status->luck + use;
    if (ran > work->stat.avoid)
        work->stat.avoid += ran;
    else
        work->stat.success += 0;
    print_screen(user, day, "문제의 답을 다 작성했습니다.", "뒤로 가기");
}

void    review_exam(t_user *user, t_event_day *day)   {
    int ran = 0;

    progress_message("문제", "검토");
    ran = rand() % user->status->luck;
    user->sub_list->exam->stat.avoid += ran;
    user->status->activ_point--;
    print_screen(user, day, "문제를 검토했습니다. 제출 시 정답률이 증가합니다!", "뒤로 가기");
}

void    push_exam(t_user *user, t_event_day *day){
    int     ran = 0;
    int     kb = 0;
    int     success = 0;
    t_subject  *work;

    work = user->sub_list->exam;
    ran = rand() % 100;
    if (work->stat.done == 0)
        print_screen(user, day, "문제를 풀고 나서 제출해 주세요.", "뒤로 가기");
    else if (work->stat.avoid >= ran)
    {
        progress_message("문제", "제출");
        user->status->activ_point--;
        work->stat.avoid = 0;
        work->stat.hp -= user->status->intel;
        if (work->stat.hp < 0)
            work->stat.hp = 0;
        work->stat.try_cnt = 0;
        print_screen(user, day, "맞았습니다! 다음 문제에 도전해 주세요,", "다음으로");
        work->stat.done = 0;
    }
    else
    {
        progress_message("문제", "제출");
        work->stat.try_cnt++;
        work->stat.time += work->stat.try_cnt * 2;
        if (work->stat.time <= user->status->activ_point)
        {
            work->stat.time = 0;
            user->status->activ_point -= work->stat.time;
            print_screen(user, day, "틀렸습니다ㅠㅠ try횟수 + 1, 제출 대기시간 두배 증가(행동력 소모 두배)", "뒤로 가기");
        }
        else
            print_screen(user, day, "남은 시간보다 대기 시간이 더 깁니다...ㅠㅠ", "뒤로 가기");
            work->stat.done = 0;
    }
    return ;
}

void    action_exam_day(t_user *user, t_event_day *day)
{
    int         kb = 0;
    t_subject   work;

    work = *user->sub_list->personal;
    while (1)
    {
        print_screen(user, day, "EXAM을 보는 날! 문제를 작성하고 제출해 EXAM을 뿌셔보자!", "문제 작성,문제 검토,문제 제출");
        kb = linux_kbhit();
        basic_information_key(kb, user);
        if (user->status->activ_point <= 0 && (kb == 'z' || kb == 'x' || kb == 'c'))
            print_screen(user, day, "행동력이 부족합니다. 진행할 수 있는 행동이 없습니다", "뒤로 가기");
        else if(kb == 'z')
            do_exam(user, day);
        else if (kb == 'x')
            review_exam(user, day);
        else if (kb == 'c')
            push_exam(user, day);
        if (kb == 'n')
            if (ask_exit(user, day))
                break ;
    }
    result_exam(user, day);
    user->sub_list->cur_exam++;
}