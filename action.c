#include "rpg42.h"

void    rush_action(t_user *user, t_event_day *day);


void    do_project(t_user *user, t_event_day *day){
    int         kb = 0;
    int         ran = 0;
    int         use = 0;
    t_subject   *work;

    work = user->sub_list->personal;
    if (work->stat.percent >= 100)
    {
        print_screen(user, day, "과제를 끝냈습니다. 제출해 주세요.", "뒤로 가기");
        return ;
    }
    use = use_action_point(user);
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
        go_esc("뒤로 가기");
        print_footer();
        kb = linux_kbhit();
        if (kb == 27)
            return ;
    }
}

void    review_project(t_user *user, t_event_day *day)   {
    int ran = 0;

    if (user->sub_list->personal->stat.percent < 100)
    {
        print_screen(user, day, "과제를 다 진행하지 못했습니다. 과제를 다 작성해 주세요.", "뒤로 가기");
        return ;
    }
    ran = rand() % user->status->luck;
    //수정 필요??라
    print_screen(user, day, "과제를 검토했습니다. 제출시 성공률이 증가합니다.", "뒤로 가기");
    user->sub_list->personal->stat.success += ran;
    user->status->activ_point--;
}

void    push_project(t_user *user, t_event_day *day){
    int         ran = 0;
    int         kb = 0;
    t_subject   *work;

    work = user->sub_list->personal;
    if (work->stat.percent < 100)
    {
        print_screen(user, day, "과제를 다 진행하지 못했습니다. 끝내고 제출해 주세요.", "뒤로 가기");
        return ;
    }
    ran = rand() % 100;
    if (work->stat.success + user->status->mental >= ran)
        subject_success(user);
    else
        subject_fail(user);
    user->status->activ_point--;
    return ;
}

void    print_item(t_user *user){
    print_screen(user, NULL, "업데이트 예정입니다", "뒤로 가기");
}

void    print_equip(t_user *user){
    print_screen(user, NULL, "업데이트 예정입니다", "뒤로 가기");
}


void    peer_eval_action(t_user *user, t_event_day *day)
{
    int use = 0;
    int ran = 0;
    int suc = 0;

    ran = rand() % user->status->luck;
    use = use_action_point(user);
    suc = ran * use;
    if (suc >= 10)
        great_eval(user, day, use);
    else if (suc >= 5)
        good_eval(user, day, use);
    else
        bad_eval(user, day, use);
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
        if (user->status->activ_point <= 0 && (kb == 'a' || kb == 'b' || kb == 'c'))
            print_screen(user, day, "행동력이 부족합니다. 진행할 수 있는 행동이 없습니다", "뒤로 가기");
        else if(kb == 'a')
            do_project(user, day);
        else if (kb == 'b')
            review_project(user, day);
        else if (kb == 'c')
            push_project(user, day);
        if (kb == 27)
            break ;
    }
}


void    action_normal_day(t_user *user, t_event_day *day)
{
    int kb = 0;

    while (1)
    {
        print_screen(user, day, "무엇을 할까?", "개인과제,동료평가,러쉬공부");
        kb = linux_kbhit();
        basic_information_key(kb, user);
        if (user->status->activ_point <= 0 && (kb == 'a' || kb == 'b' || kb == 'c'))
            print_screen(user, day, "행동력이 부족합니다. 진행할 수 있는 행동이 없습니다", "뒤로 가기");
        else if (kb == 'a')
            personal_action(user, day);
        else if (kb == 'b')
            peer_eval_action(user, day);
        else if (kb == 'c' && (day->day == 2))
            rush_action(user, day);
        else if (kb == 'c')
            print_screen(user, day, "RUSH 진행 기간이 아닙니다", "뒤로 가기");
        if (kb == 27)
            break ;
    }
}

void    rush_action(t_user *user, t_event_day *day)
{
    int         ran = 0;
    int         use = 0;
    int         user_compre;
    int         sum = 0;
    t_peer      *peer;
    t_subject   *work;

    work = user->sub_list->rush;
    ran = rand() % user->status->intel;
    peer = work->peer;
    user_compre = ran;
    use = use_action_point(user);
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
                print_screen(user, day, "동료가 러쉬를 포기했습니다ㅠㅠ", "뒤로 가기");
                peer[i].role.comprehension = 0;
                break;
        }
        sum += peer[i].role.comprehension;
    }
    user->sub_list->rush->stat.comprehension += use * user_compre;
    sum += user->sub_list->rush->stat.comprehension;
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

// basic_txt_print(user, day, peer[0].role.comment);
// basic_txt_print(user, day, peer[1].role.comment);
