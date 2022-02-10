# include "rpg42.h"

void    ft_event_day(t_event_day *event_day, t_user *user)  {
    int kb = 0;

    while (1)
    {
        print_header();
        printf("      DAY %d - %s\n\n", event_day->day, event_day->event_title);
        printf("      %s\n", event_day->event_content);
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        go_next("다음");
        print_footer();
        kb = linux_kbhit();
        if (kb == 'n')
            break ;
    }
}

void    day_work(int date, t_user *user, int day) {
    int kb = 0;
    int i = date % 3;
    char    actions[4][3][20] = {{"개인 과제", "동료 평가", "없음"}, 
                                {"소스 작성", "소스 검토", "과제 제출"}, 
                                {"소스 작성", "소스 검토", "소스 제출"}, 
                                {"개인 과제", "동료 평가", "러쉬 공부"}};

    while (1)
    {
		if (i == 0)
		    action_normal_day(user, day, actions);
        // else if (i == 1)
        //     action_exam_day(user, action3);
        // else if (i == 2)
        //     action_rush_day(user, action4, action2);
	   // 요일 출력 - 
        // 행동력 출력
        
        // 선택지에 따른 행동력 감소
        // 선택지에 따른 능력치 변동
        break ;
    }
}
