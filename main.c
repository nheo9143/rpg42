#include "rpg42.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
# include <termios.h>

void	start_42rpg(){
	static	t_user	*user;
    t_event_day     *event_day;
	char            chrtype;

    print_title();
	// 캐릭터 설정
    chrtype = select_charictor();
    // printf("chtype : %c\n", chrtype);
	// 초기화
	user = init_user(chrtype);
	event_day = init_event_day();
    // 피신 시작 이벤트
    
    for (int i = 0; i < 3; i++)
    {
        user->status->activ_point = 100 + user->status->dex;
        ft_event_day(event_day, user);
        // ft_event_day(event_day[i], user);
        // 0:개인과제 하는 날, 1:시험, 2:러쉬 활성화
        day_work(i, user, event_day);
        print_header();
        printf("       집에 가는 중...");
        print_footer();
        usleep(1500000);
        event_day++;
    }
    
    //유저 정보 조회 함수
	// print_user_status(user);
    // subject_success(user, 100);
    // print_user_status(user);
	
}

int main(void){
	start_42rpg();
}
