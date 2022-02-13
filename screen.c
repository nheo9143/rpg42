#include "rpg42.h"

#define LEFT_SPACE 7
void gotoxy(int x, int y);
/*
##########################

	Day-??       남은 행동력(시간)
	현재 과제      기본 행동

	일반 텍스트

	선택지1
	선택지2
	선택지3

##########################

##########################

	Day-??                  남은 행동력(시간)
	현재 과제(시험날은 시험)      기본 행동

	일반 텍스트

	뒤로 가기

##########################
print_screen("다음으로")


##########################

	Day-??                  남은 행동력(시간)
	현재 과제(시험날은 시험)      기본 행동

	결과 텍스트(유저정보)

	뒤로 가기

##########################

USER, EVENT_DAY
기본문자를 출력할 함수
1(TEXT)
##########################

	Day-??                  남은 행동력(시간)
	현재 과제(시험날은 시험)      기본 행동

	TEXT


선택지를 처리할 함수
2(char *, char ch): "선택지1,선택지2,선택지3"
2(char *, char ch)  "다음으로"
					"이전으로"
					"null"





##########################

	텍스트 : 입력값
*/


//  기본 파라미터(t_user *user, t_event day *day)

//  화면 기본값 : 위 아래
//  print_screen(user, day, *str1(일반 텍스트), *str2(선택지))



//  type = 일반 텍스트/ 선택지



//  1. 선택창이 있는 화면
//      -> 추가 파라미터 : 선택지들 (보기 중에 고르는거)



//  2. 일반 텍스트가 있는 화면(content, event 관련, 스토리 관련 진행) -> 선택지 : 뒤로 가기 / 다음으로
//      -> 추가 파라미터 : 텍스트



//  3. 결과창(스테이터스 변화 출력)
//      -> 추가 파라미터 : ? 



//  4. 스테이터스, 아이템, 장비창



//  5. 시작페이지



//  6. 입력값 받는 창(행동력 소모(scanf), 이름 정하기 등등)



//  7. 엔딩페이지

static size_t	ft_count_strs(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (*s)
	{
		if (*s != c && *s)
		{
			while (*s != c && *s)
				s++;
			i++;
		}
		else
			s++;
	}
	return (i);
}


int		get_count_strs(char **str){
	int	cnt;

	cnt = 0;
	while (str != NULL && *str)
	{
		cnt++;
		str++;
	}
	return cnt;
}

void    print_header(void){
    system("clear");
    printf("########################################################################################\n");
    printf("\n\n");
}

void    print_footer(void){
    printf("\n\n");
    printf("########################################################################################\n");
}

void    print_text(char * text)
{
	printf("\n\n\n       %s\n\n", text);
}

int    print_distractor(char *distractor)
{
	char    **select;
	int     pos_y;
	int		i;
	int		cnt;

	i = 0;
	select = ft_split(distractor, ',');
	
	cnt = get_count_strs(select);
	if (cnt == 1)
		return cnt;
	while (select != NULL && *select)
	{
		printf("       %c)%s\n", 'a' + i, *select);
		select++;
		i++;
	}
    printf("\n\n\n       (esc)뒤로 가기");
	return 0;
}

void	print_day_info(t_event_day *day, t_user *user){
	int	cur_p = user->sub_list->cur_personal;
	int	cur_e = user->sub_list->cur_exam;

	printf("       %s:%d%68s : %d\n", "day", day->day, "남은 행동력(시간)", user->status->activ_point);
    if (day->day % 3 == 2)
    {
    	printf("       %s:%d%-25s w)%s, s)%s, i)%s, e)%s\n", "EXAM 진행도", 100 - user->sub_list->exam->stat.hp, "/100", "work", "status", "items", "equipment");
        printf("       %s:%d\n", "retry 횟수", user->sub_list->exam->stat.try_cnt);
    }
    else
		printf("       %s:%-29s w)%s, s)%s, i)%s, e)%s\n", "현재 과제", user->sub_list->personal[cur_p].event.title, "work", "status", "items", "equipment");
	
}

void    print_screen(t_user *user, t_event_day *day, char *text, char *distractor)
{
    int kb = 0;

	// 헤더 푸터 출력
    if (ft_count_strs(distractor, ',') == 1)
	{
		while (1)
		{
			//수정 필요??
            print_header();
            if (day != NULL)
                print_day_info(day, user);
            print_text(text);
			printf("\n\n\n\n\n\n\n\n");
			printf("       (esc)뒤로 가기");
            print_footer();
            kb = linux_kbhit();
            basic_information_key(kb, user);
			if (kb == 27)
				return ;
		}
	}
    else
    {
        print_header();

        // day 및 남은 행동력, 과제, 도움 옵션 출력
        if (day != NULL)
            print_day_info(day, user);

        // 기본 텍트스를 처리할 함수
        print_text(text);

        print_distractor(distractor);

        // 선택지를 처리할 함수

        print_footer();
    }
}

void	print_peer(t_user *user, t_event_day *day)
{
	int	kb = 0;

	while (1)
	{
		print_header();
		printf("	   %s 진행 기간입니다!\n\n", user->sub_list->rush->event.title);
		printf("       동료 %s와 같은 팀이 되었습니다!\n", user->sub_list->rush->peer[0].name);
		printf("       동료 %s와 같은 팀이 되었습니다!\n", user->sub_list->rush->peer[1].name);
		printf("\n\n\n\n");
		printf("       (esc)뒤로 가기");
		print_footer();
		kb = linux_kbhit();
		if (kb == 27)
			return ;
	}
}

void	print_personal_work(t_user *user)
{
	int	kb = 0;
	int	cur_p = user->sub_list->cur_personal;

	while (1)
	{
		print_header();
		printf("       -------개인 과제-------\n\n");
		for (int i = 0; i <= cur_p; i++)
		{
				if (i == cur_p)
					printf("       >%s(진행 중), %d / 100\n", user->sub_list->personal[i].event.title, user->sub_list->personal[i].stat.percent);
				else
					printf("       █%s(진행 완료), %d / 100\n", user->sub_list->personal[i].event.title, user->sub_list->personal[i].stat.percent);
			printf("\n");
		}
		for (int i = cur_p + 1; i < 10; i++)
		{
			printf("       -%s(아직 열리지 않음)\n", user->sub_list->personal[i].event.title);
			printf("\n");
		}
		printf("\n       (esc)뒤로 가기");
		print_footer();
		kb = linux_kbhit();
		if (kb == 27)
			return ;
	}
}

void	print_exam_work(t_user *user)
{
	int	kb = 0;
	int	cur_e = user->sub_list->cur_exam;

	while (1)
	{
		print_header();
		printf("\n       -------EXAM-------\n\n");
		for (int i = 0; i <= cur_e; i++)
		{
			if (i == cur_e)
				printf("       >%s(진행 중), %d / 100\n", user->sub_list->exam[i].event.title, 100 - user->sub_list->exam[i].stat.hp);
			else
				printf("       █%s(진행 완료), %d / 100\n", user->sub_list->exam[i].event.title, 100 - user->sub_list->exam[i].stat.hp);
			printf("\n");
		}
		for (int i = cur_e + 1; i < 3; i++)
		{
			printf("       -%s(아직 열리지 않음)\n", user->sub_list->exam[i].event.title);
			printf("\n");
		}
		printf("\n\n       (esc)뒤로 가기");
		print_footer();
		kb = linux_kbhit();
		if (kb == 27)
			return ;
	}
}

void	print_rush_work(t_user *user)
{
	int	kb = 0;
	int	cur_r = user->sub_list->cur_rush;

	while (1)
	{
		print_header();
		printf("\n       -------RUSH-------\n\n");
		for (int i = 0; i <= cur_r; i++)
		{
			if (i == cur_r)
				printf("       >%s(진행 중), %d / 100\n", user->sub_list->rush[i].event.title, user->sub_list->rush[i].stat.percent);
			else
				printf("       █%s(진행 완료), %d / 100\n", user->sub_list->rush[i].event.title, user->sub_list->rush[i].stat.percent);
			printf("\n");
		}
		for (int i = cur_r + 1; i < 3; i++)
		{
			printf("       -%s(아직 열리지 않음)\n", user->sub_list->rush[i].event.title);
			printf("\n");
		}
		printf("\n\n       (esc)뒤로 가기");
		print_footer();
		kb = linux_kbhit();
		if (kb == 27)
			return ;
	}
}

void	print_work(t_user * user)
{
	int	kb = 0;

	while (1)
	{
		print_header();
		printf("\n\n\n\n");
		printf("       과제 목록\n\n");
		print_distractor("개인 과제,EXAM,RUSH");
		print_footer();
		kb = linux_kbhit();
		if (kb == 'a')
			print_personal_work(user);
		else if (kb == 'b')
			print_exam_work(user);
		else if (kb == 'c')
			print_rush_work(user);
		if (kb == 27)
			return ;
	}
}

void    print_item(t_user *user){
    print_screen(user, NULL, "업데이트 예정입니다", "뒤로 가기");
}

void    print_equip(t_user *user){
    print_screen(user, NULL, "업데이트 예정입니다", "뒤로 가기");
}

void    progress_message(char *name, char *act)
{
    print_header();
    printf("       %s를 %s합니다.\n", name, act);
    print_footer();
    usleep(600000);
    print_header();
    printf("       %s를 %s합니다..\n", name, act);
    print_footer();
    usleep(600000);
    print_header();
    printf("       %s를 %s합니다...\n", name, act);
    print_footer();
    usleep(600000);
}

void	print_works(t_user *user, t_event_day *day)
{

}