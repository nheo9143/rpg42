#include "rpg42.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void	start_42rpg(){
	static	t_user	*user;
    int             day = 0;
    t_event_day     **event_day;
	char            chrtype;
    print_title();
	// 캐릭터 설정
    chrtype = select_charictor();
    // printf("chtype : %c\n", chrtype);
	// 초기화
	user = init_user(chrtype);
	event_day = init_event_day();
    // 피신 시작 이벤트
    ft_event_day(event_day[0], user);
    
    for (int i = 0; i < EVENT_DAY_MAX; i++)
    {
        // ft_event_day(event_day[i], user);
        // 0:개인과제 하는 날, 1:시험, 2:러쉬 활성화
        day_work(i % 3, user, day);
        day++;
    }
    //유저 정보 조회 함수
	// print_user_status(user);
    // subject_success(user, 100);
    // print_user_status(user);
	
}

int main(void){
	system("mode con cols=80 lines=25");
	start_42rpg();
}
