# include "rpg42.h"

void    ft_event_day(t_event_day *event_day, t_user *user)  {
    int kb = 0;
    t_status stat = event_day->reward_status;

    while (1)
    {
        operate_status(user, stat);
        print_header();
        printf("      DAY %d - %s\n\n", event_day->day, event_day->event_title);
        printf("      %s\n", event_day->event_content);
         if (stat.exp)
            printf("       %-6s %d\n", "경험치", stat.exp);
        else
            printf("\n");
        if (stat.intel)
            printf("       %-6s %d\n", "cs지식", stat.intel);
        else
            printf("\n");
        if (stat.dex)
            printf("       %-6s %d\n", "dex", stat.dex);
        else
            printf("\n");
        if (stat.fame)
            printf("       %-6s %d\n", "fame", stat.fame);
        else
            printf("\n");
        if (stat.luck)
            printf("       %-6s %d\n", "luck",stat.luck);
        else
            printf("\n");
        if (stat.mental)
            printf("       %-6s %d\n", "mental", stat.mental);
        else
            printf("\n");
        if (stat.fighting_point)
            printf("       %-6s %d\n", "사기", stat.fighting_point);
        else
            printf("\n");
        if (stat.activ_point)
            printf("       오늘 %-6s %d\n", "행동력", stat.activ_point);
        else
            printf("\n");
        if (stat.level)
            printf("       %-6s %d\n", "레벨", stat.level);
        else
            printf("\n");
        go_next("다음");
        print_footer();
        kb = linux_kbhit();
        if (kb == 'n')
            break ;
    }
}

void    day_work(int date, t_user *user, t_event_day *day) {
    int kb = 0;
    int i = date % 3;

    while (1)
    {
		if (i != 1)
		    action_normal_day(user, day);
        else if (i == 1)
            action_exam_day(user, day);
        
	   // 요일 출력 - 
        // 행동력 출력
        
        // 선택지에 따른 행동력 감소
        // 선택지에 따른 능력치 변동
        break ;
    }
}
