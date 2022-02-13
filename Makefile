NAME = rpg42
SRCS = main.c\
		  action.c\
		  event.c\
		  exam.c\
		  init.c\
		  parson.c\
		  result.c\
		  screen.c\
		  user.c\
		  util.c

OBJS = $(SRCS:.c=.o)

%.o : %.c
	gcc -c -o $@ $<

all : $(NAME)

$(NAME) : $(OBJS)
	gcc $(OBJS) -o $(NAME)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

