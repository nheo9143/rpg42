#ifndef RPG42_H
# define RPG42_H
# include <stdio.h>     // 파일 처리 함수가 선언된 헤더 파일
# include <stdlib.h>    // malloc, free 함수가 선언된 헤더 파일
# include <stdbool.h>   // bool, true, false가 정의된 헤더 파일
# include <string.h>    // strchr, memset, memcpy 함수가 선언된 헤더 파일
# include "parson.h"

# define SUBJECT_PERSONAL_MAX 17
# define SUBJECT_EXAM_MAX 3
# define SUBJECT_RUSH_MAX 3
# define EVENT_DAY_MAX 1
# define EVENT_DAY_PATH "./event_day/"
# define JSON_PATH "./json/"

typedef struct s_status {
	// 기본 능력치
	int	level;
	int	exp;
	int	intel;
	int	dex;
	int	luck;
	int	mental;
	int	activ_point;
	int	fame;
	int	fighting_point;
	
} t_status;

typedef struct  sub_status 
{
	int	percent;
	int comprehension;
	int success;
	int hp;
	int avoid;
	int try_cnt;
	int time;
}	t_sub_status;

typedef enum _equip_type{
	helmet = 0,
	weapon
}	equip_type;


typedef struct s_equip_status{
	int	intel;
	int	dex;
	int	luck;
	int	mental;
	int	activ_point;
	int	fame;
	int	fighting_point;
}	t_equip_status;

typedef struct s_equip {
	equip_type		type;
	t_equip_status	status;
}	t_equip;

typedef struct s_item {
	char		*name;
	t_status	reward_status;
	int			num; // 소지 개수
}	t_item;

typedef enum _user_type	{
	전공자 = 0,
	비전공자
}	user_type;

typedef struct s_event_day
{
	int			day;
	char		*event_title;
	char		*event_content;
	t_status	reward_status;	
} t_event_day;

typedef enum _personal_quest
{
	Shell00 = 0,
	Shell01,
	C00,
	C01,
	C02,
	C03,
	C04,
	C05,
	C06,
	C07,
	C08,
	C09,
	C10,
	C11,
	C12,
	C13,
	C14,
	C15
} personal_quest;

typedef struct s_event {
	char	*title;
	char	*content;
}	t_event;

typedef struct s_reward {
	t_status stat;
	t_equip equip;
}	t_reward;

typedef struct s_personal_stat {
	int			percent;
	int			success;
	int			grade;
}	t_personal_stat;

typedef struct s_act_personal
{
	t_event			event;
	t_personal_stat	stat;
	t_reward		reward;
} t_act_personal;

typedef struct s_rush_peer_type {
	int		type;
	char	staus[20];
} t_rush_peer_type;

typedef struct s_rush_peer_status {
	int		type;
	char	status[20];
	int		comprehension;
} t_rush_peer_status;

typedef enum _rush_type {
	rush00 = 0,
	rush01,
	rush02
} rush_type;

typedef	struct s_peer {
	char				*name;
	t_rush_peer_type	type;
	t_rush_peer_status	status;
} t_peer;


typedef struct s_work {
	int			type;
	t_event		event;
	t_status	stat;
	t_reward	reward;
	t_peer		peer[2];
}	t_work;

typedef struct s_rush {
	rush_type	num;
	int			percent;
	int			success;
	t_peer		peer1;
	t_peer		peer2;
	t_reward	reward;
} t_rush;

typedef enum _week {
	monday = 0,
	thesday,
	weekend
}	week;

typedef enum _exam_name {
	exam00 = 0,
	exam01,
	exam02,
	final_exam
} exam_name;

typedef struct s_exam {
	exam_name name;
	int hp;
	int avoid;
	int try_cnt;
	int time;
	t_status reward_status;
	t_equip reward_equip;
} t_exam;

typedef struct s_peer_eval {
	char		*event_peer_eval;
	t_status	reward_status;
	t_equip		reward_equip;
} t_peer_eval;

typedef enum _action_type{
	personal = 0,
	rush,
	exam
}	action_type;

typedef struct s_action {
	int			type;
	t_event		event;
	t_status	stat;
	t_reward	reward;
	t_peer		peer[2];
}	t_action;

typedef struct s_subject{
	int				type;
	t_event			event;
	t_sub_status	stat;
	t_reward		reward;
	t_peer			peer[2];
}	t_subject;

typedef struct s_subject_list{
	int			cur_personal;
	int			cur_exam;
	int			cur_rush;	
	t_subject	*personal;
	t_subject	*exam;
	t_subject	*rush;
}	t_subject_list;

typedef struct s_user
{
	// 유저 타입(전공자 / 비전공자)
	int			type;
	// 유저 이름
	char		user_name[20];
	// 유저 능력치
	t_status	*status;
	// 유저 장비
	t_equip		equip[10];
	// 아이템
	t_item		item[100];
	t_subject_list *sub_list;
	// 현재 수행중인 과제
} t_user;

void    input_error(void);
void	print_title(void);
char    select_charictor(void);
t_user		*init_user(char type);
t_event_day	**init_event_day(void);
//t_scheudule **init_scheudule(void);
t_action	*init_action(void);
t_subject_list *init_subjet_list();
void    subject_success(t_user *user);
void    subject_fail(t_user *user);
void    print_header(void);
void    print_footer(void);
void    print_user_status(t_user *user);
void    print_distractor(char act[3][20]);
void    go_esc(char *str);
void	go_next(char *str);
int		linux_kbhit(void);
void    operate_status(t_user *user, t_status operate);
void    ft_event_day(t_event_day *event_day, t_user *user);
void    action_normal_day(t_user *user, int day, char actions[4][3][20]);
void    action_exam_day(t_user *user, char **action);
int		use_action_point(t_user *user);
void    day_work(int date, t_user *user, int day);
void	print_day_info(int day, t_user *user);
int		check_level_up(t_user *user);
void    basic_txt_print(t_user *user, int day, char *str);
void    distractor_format(t_user *user, int day, char *go_to, char act[3][20]);
void    basic_information_key(int kb, t_user *user);


//void    action_normal_day(t_user *user, char **action1, char **action2);

#endif