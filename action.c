#include "rpg42.h"

void    do_project(t_user *user, int day){
    int         kb = 0;
    int         ran = 0;
    int         use = 0;
    t_subject   *work;

    work = user->sub_list->personal;
    if (work->stat.percent >= 100)
    {
        basic_txt_print(user, day, "과제를 다 진행했습니다. 제출해 주세요\n");
        return ;
    }
    use = use_action_point(user);
    work->stat.percent += (user->status->intel * use);
    if (work->stat.percent > 100)
        work->stat.percent = 100;
    while (1)
    {
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

void    review_project(t_user *user, int day){
}

void    push_project(t_user *user, int day){
    int         ran = 0;
    int         kb = 0;
    t_subject   *work;

    work = user->sub_list->personal;
    if (work->stat.percent < 100)
    {
        basic_txt_print(user, day, "과제를 다 진행하지 못했습니다. 과제를 다 진행해 주세요\n");
        return ;
    }
    ran = rand() % 100;
    if (work->stat.success + user->status->mental >= ran)
        subject_success(user);
    else
        subject_fail(user);    
    return ;
}

void    print_item(void){

}
void    print_equip(void){

}


void    peer_eval_action(t_user *user)
{
	
}

void    personal_action(t_user *user, int day, char action[3][20])
{
    int         kb = 0;
    t_subject   work;

    work = *user->sub_list->personal;
    while (1)
    {
        distractor_format(user, day, "뒤로 가기", action);
        kb = linux_kbhit();
        if (user->status->activ_point <= 0 && (kb == 'a' || kb == 'b' || kb == 'c'))
            basic_txt_print(user, day, "행동력이 부족합니다. 진행할 수 있는 행동이 없습니다.");
        else if(kb == 'a')
            do_project(user, day);
        else if (kb == 'b')
            review_project(user, day);
        else if (kb == 'c')
            push_project(user, day);
        basic_information_key(kb, user);
        if (kb == 27)
            break ;
    }
}


void    action_normal_day(t_user *user, int day, char actions[4][3][20])
{
    int kb = 0;

    while (1)
    {
        distractor_format(user, day, "집에 가기", actions[0]);
        kb = linux_kbhit();
        if (user->status->activ_point <= 0 && (kb == 'a' || kb == 'b' || kb == 'c'))
            basic_txt_print(user, day, "행동력이 부족합니다.");        
        else if (kb == 'a')
            personal_action(user, day, actions[1]);
        else if (kb == 'b')
            peer_eval_action(user);
        basic_information_key(kb, user);
        if (kb == 27)
            break ;
    }
}

// void    action_exam_day(t_user *user, char **action)
// {
//     int kb = 0;

//     while (1)
//     {
//         print_header();
//         print_day_info("금요일", user);
//         printf("\n\n\n\n\n\n\n\n");
//         go_esc("다음 날(포기)");
//         print_distractor(action[0], action[1], action[2]);
//         print_footer();
//         kb = linux_kbhit();
//         if(kb == 'a')
//             do_project(user, exam);
//         else if (kb == 'b')
//             review_project(user, exam);
//         else if (kb == 'c')
//             push_project(user, exam);
//         if (kb == 27)
//             break ;
//     }
// }

// void    action_rush_day(t_user *user, char **action1, char **action2)
// {
//     int kb = 0;

//     while (1)
//     {
//         print_header();
//         print_day_info("주말", user);
//         printf("\n\n\n\n\n\n\n\n");
//         go_esc("다음 날");
//         print_distractor(action1[0], action1[1], action1[2]);
//         print_footer();
//         kb = linux_kbhit();
//         if (kb == 's')
//             print_user_status(user);
//         else if (kb == 'i')
//             print_item();
//         else if (kb == 'e')
//             print_equip();
//         else if(kb == 'a')
//             personal_action(user, action2);
//         else if (kb == 'b')
//             peer_eval_action(user);
//         if (kb == 27)
//             break ;
//     }
// }