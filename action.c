#include "rpg42.h"

void    rush_action(t_user *user, t_event_day *day);


void    do_project(t_user *user, t_event_day *day){
    int         kb = 0;
    int         ran = 0;
    int         use = 0;
    int         cur = user->sub_list->cur_personal;
    t_subject   *work;

    work = &user->sub_list->personal[cur];
    if (work->stat.percent >= 100)
    {
        print_screen(user, day, "과제를 끝냈습니다. 제출해 주세요.", "뒤로 가기");
        return ;
    }
    use = use_action_point(user);
    if (use == 0)
        return ;
    progress_message(work->event.title, "진행");
    work->stat.percent += (user->status->intel * use);
    if (work->stat.percent > 100)
        work->stat.percent = 100;
    while (1)
    {
        //수정 필요??
        print_header();
        print_day_info(day, user);
        printf("\n\n\n\n\n\n\n\n");
        printf("       과제 진행도 : %d\n", work->stat.percent);
        go_next();
        print_footer();
        kb = linux_kbhit();
        basic_information_key(kb, user);
        if (kb == 'n')
            return ;
    }
}

void    review_project(t_user *user, t_event_day *day)   {
    int ran = 0;
    int cur = user->sub_list->cur_personal;

    if (user->sub_list->personal[cur].stat.percent < 100)
    {
        print_screen(user, day, "과제를 다 진행하지 못했습니다. 과제를 다 작성해 주세요.", "뒤로 가기");
        return ;
    }
    progress_message(user->sub_list->personal[cur].event.title, "검토");
    ran = rand() % user->status->luck;
    print_screen(user, day, "과제를 검토했습니다. 제출시 성공률이 증가합니다.", "뒤로 가기");
    user->sub_list->personal[cur].stat.success += ran;
    user->status->activ_point--;
}

void    push_project(t_user *user, t_event_day *day){
    int         ran = 0;
    int         kb = 0;
    int         cur = user->sub_list->cur_personal;
    t_subject   *work;

    work = &user->sub_list->personal[cur];
    if (work->stat.percent < 100)
    {
        print_screen(user, day, "과제를 다 진행하지 못했습니다. 끝내고 제출해 주세요.", "뒤로 가기");
        return ;
    }
    progress_message(work->event.title, "제출");
    ran = rand() % 100;
    if (work->stat.success + (user->status->mental - 10) >= ran)
        subject_success(user);
    else
        subject_fail(user);
    user->status->activ_point--;
    return ;
}


void    peer_eval_action(t_user *user, t_event_day *day)
{
    int use;

    use = use_action_point(user);
    if (use == 0)
        return ;
    progress_message("동료 평가", "진행");
    print_eval(user, day, use);
}

void    personal_action(t_user *user, t_event_day *day)
{
    int         kb = 0;
    t_subject   work;

    work = *user->sub_list->personal;
    while (1)
    {
        print_screen(user, day, "개인 과제를 하자. 뭘 할까?", "과제 작성,과제 검토,과제 제출");
        kb = linux_kbhit();
        basic_information_key(kb, user);
        if (user->status->activ_point <= 0 && (kb == 'z' || kb == 'x' || kb == 'c'))
            print_screen(user, day, "행동력이 부족합니다. 진행할 수 있는 행동이 없습니다", "뒤로 가기");
        else if(kb == 'z')
            do_project(user, day);
        else if (kb == 'x')
            review_project(user, day);
        else if (kb == 'c')
            push_project(user, day);
        if (kb == 'n')
            break ;
    }
}


void    action_normal_day(t_user *user, t_event_day *day)
{
    int kb = 0;

    if (day->day == 3)
        print_peer(user, day);
    while (1)
    {
        print_screen(user, day, "무엇을 할까?", "개인과제,동료평가,러쉬공부");
        kb = linux_kbhit();
        basic_information_key(kb, user);
        if (user->status->activ_point <= 0 && (kb == 'z' || kb == 'x' || kb == 'c'))
            print_screen(user, day, "행동력이 부족합니다. 진행할 수 있는 행동이 없습니다", "뒤로 가기");
        else if (kb == 'z')
            personal_action(user, day);
        else if (kb == 'x')
            peer_eval_action(user, day);
        else if (kb == 'c' && (day->day == 3))
            rush_action(user, day);
        else if (kb == 'c')
            print_screen(user, day, "RUSH 진행 기간이 아닙니다", "뒤로 가기");
        if (kb == 'n')
            if (ask_exit(user, day))
                break ;
    }
}

void    rush_action(t_user *user, t_event_day *day)
{
    int         ran = 0;
    int         use = 0;
    int         cur = user->sub_list->cur_rush;
    int         user_compre;
    int         sum = 0;
    t_peer      *peer;
    t_subject   *work;

    work = &user->sub_list->rush[cur];    
    user_compre = rand() % user->status->intel;
    peer = work->peer;
    
    use = use_action_point(user);
    if (use == 0)
        return ;
    progress_message(work->event.title, "진행");
    for (int i = 0; i < 2; i++){
        switch (peer[i].status.type)
        {
            case rich:
                peer[i].role.comprehension += rand() % (use * ran);
                break;
            case passional:
                peer[i].role.comprehension += (rand() % (use * ran)) * 2;
                break;
            case s_normal:
                peer[i].role.comprehension += rand() % (use * ran);
                break;
            case wordy:
                user->sub_list->rush->stat.comprehension = peer[i].role.comprehension - ((peer[i].role.comprehension - user_compre) / use);
                break;
            case busy:
                peer[i].role.comprehension += (rand() % (use * ran)) / 2;
                break;
            case forgiven:
                print_screen(user, day, "동료 cgim이 러쉬를 포기했습니다ㅠㅠ", "뒤로 가기");
                peer[i].role.comprehension = 0;
                break;
        }
        sum += peer[i].role.comprehension;
    }
    work->stat.comprehension += use * user_compre;
    sum += user->sub_list->rush->stat.comprehension;
    work->stat.percent = sum / 3;
    if (sum <= 50)
        print_screen(user, day, "이번 RUSH는 망한 것 같습니다ㅠㅠ", "뒤로 가기");
    else if (sum <= 100)
        print_screen(user, day, "RUSH가 어떻게든 진행은 되고 있습니다.", "뒤로 가기");
    else if (sum <= 150)
        print_screen(user, day, "RUSH를 열심히 진행중입니다.", "뒤로 가기");
    else if (sum <= 200)
        print_screen(user, day, "RUSH가 상당히 잘 되고 있는 것 같습니다.", "뒤로 가기");
    else if (sum <= 250)
        print_screen(user, day, "RUSH를 꽤 잘 한 기분입니다. 이대로라면 나쁘지 않을지도...?", "뒤로 가기");
    else if (sum > 250)
        print_screen(user, day, "RUSH의 신이 된 기분입니다. 완벽하게 디펜스를 할 수 있을 것 같습니다.", "뒤로 가기");
}
