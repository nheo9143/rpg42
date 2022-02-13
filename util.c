#include <stdlib.h>
#include <stdio.h>
# include <termios.h>
# include <unistd.h>
#include "rpg42.h"

int linux_kbhit(void);

void    go_esc(char *str){
    printf("        %s : (esc)\n", str);
}

void    go_next(char *str){
    printf("        %s : (n)\n", str);
}

void    input_error(void){
    int kb = 0;

    while (1)
    {
        print_header();
        printf("       error(입력 오류) : 올바른 값을 입력해 주세요.\n\n\n\n");
        printf("       (esc)뒤로 가기\n");
        print_footer();
        kb = linux_kbhit();
        if (kb == 27)
            break ;
    }
}

void    print_title(void){
    int kb = 0;

    while (!kb){
    print_header();
    printf("\n\n");
    printf("             _______   _______    ______         __    __   ______  \n");
    printf("             |       \\ |       \\  /      \\       |  \\  |  \\ /      \\ \n ");
    printf("             | $$$$$$$\\| $$$$$$$\\|  $$$$$$\\      | $$  | $$|  $$$$$$\n");
    printf("             | $$__/ $$| $$__| $$| $$ __\\$$      | $$__| $$ \\$$__| $$\n");
    printf("             | $$    $$| $$    $$| $$|    \\      | $$    $$ /      $$\n");
    printf("             | $$$$$$$ | $$$$$$$\\| $$ \\$$$$       \\$$$$$$$$|  $$$$$$ \n");
    printf("             | $$  | $$| $$|       $$__| $$            | $$| $$_____ \n");
    printf("             | $$  | $$| $$       \\$$    $$            | $$| $$     \\\n");
    printf("              \\$$   \\$$ \\$$        \\$$$$$$              \\$$ \\$$$$$$$$\n");
    printf("\n\n");
    print_footer();
    printf("press any key to start!");
    kb = linux_kbhit();
    }
}


char    select_charictor(void){
    char    chrtype;
    int     kb = 0;

    while (1)
    {
        print_header();
        printf("       캐릭터를 선택해 주세요.\n");
        printf("\n");
        printf("       ------------------------------------------------------------------------\n\n\n");
        printf("       a) 전공자 : 컴퓨터공학과를 다녀 컴퓨터 관련 지식을 상당히 가지고 있음\n\n");
        printf("       b) 비전공자 : 비전공자로 컴퓨터 공부를 처음 한다\n\n\n");
        printf("       ------------------------------------------------------------------------\n");
        print_footer();
        printf("\n\n\n");
        printf("       입력(a or b) :");
        kb = linux_kbhit();
        if (kb == 'a' || kb == 'b')
        {
            print_header();
            if (kb == 'a')
            {
                printf("\n       '전공자'캐릭터를 선택하셨습니다.\n");
                chrtype = 'a';
            }
            else if (kb == 'b')
            {
                printf("\n       '비전공자'캐릭터를 선택하셨습니다.\n");
                chrtype = 'b';
            }
            print_footer();
            sleep(1);
            return (chrtype);
        }
        else
            input_error();
    }
}

int linux_kbhit(void)
{
    struct termios oldt, newt;
    int ch;

    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

    // switch(day){
    //             case monday:
    //                 break ;
    //             case thesday:
    //                 // 선택지 함수
    //                 break ;
    //             case weekend:
    // 				// 함수
    //                 break ;
    // 			default :
    // 				break;
    //         }

int    use_action_point(t_user *user)
{
    int use = 0;

    while (1)
    {
        print_header();
        printf("\n\n       남은 행동력 : %d\n\n", user->status->activ_point);
        printf("       사용할 행동력 : ");
        scanf("%d", &use);
        if (use > user->status->activ_point || use < 0)
            input_error();
        else
        {
            user->status->activ_point -= use;
            return use;
        }
        printf("\n\n\n\n");
        print_footer();
    }
    return (0);
}

int    basic_information_key(int kb, t_user *user)
{
    if (kb == 's')
        print_user_status(user);
    else if (kb == 'i')
        print_item(user);
    else if (kb == 'e')
        print_equip(user);
    else if (kb == 'w')
        print_work(user);
    return kb;
}

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

static size_t	ft_count_chars(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] != c && s[i])
		i++;
	return (i);
}

static char const	*ft_next_strs(char const *s, char c, int i)
{
	if (i == 0)
		while (*s && *s == c)
			s++;
	else
		while (*s && *s != c)
			s++;
	return (s);
}

static char	**ft_free_strs(char **strs)
{
	size_t	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		strs[i++] = 0;
	}
	free(strs);
	return (0);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	num_strs;
	char	**ptrs;

	if (!s)
		return (0);
	i = 0;
	num_strs = ft_count_strs(s, c) + 1;
	ptrs = (char **)calloc(num_strs, sizeof(char *));
	if (!ptrs)
		return (0);
	while (i + 1 < num_strs)
	{
		s = ft_next_strs(s, c, 0);
		ptrs[i] = (char *)calloc(ft_count_chars(s, c) + 1, sizeof(char));
		if (!ptrs[i])
			return (ft_free_strs(ptrs));
		strlcpy(ptrs[i++], s, ft_count_chars(s, c) + 1);
		s = ft_next_strs(s, c, 1);
	}
	return (ptrs);
}

int ask_exit(t_user *user, t_event_day *day)
{
    int kb = 0;

    while (1)
    {
        print_screen(user, day, "집에 가시겠습니까?", "집에 간다,집에 안간다");
        kb = linux_kbhit();
        basic_information_key(kb, user);
        if (kb == 'a')
            return (1);
        else if (kb == 'b')
            return (0);
        else if (kb == 27)
            return (0);
    }
}
