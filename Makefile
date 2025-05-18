CC = cc
FLAGS = -Wall -Wextra -Werror
HEADER = minishell.h
NAME = minishell

FILE = builtins.c envp_.c execute.c utiles.c builtins_utils.c envp_cmpl.c main.c redirections.c
OBJECT_FILE = $(FILE:.c=.o)

all : $(NAME)

%.o : %.c $(HEADER)
	$(CC) $(FLAGS) -c $< -o $@

$(NAME) : $(OBJECT_FILE)
	$(CC) $(FLAGS) -o $(NAME) $(OBJECT_FILE) -lreadline

clean :
	rm -f $(OBJECT_FILE)

fclean : clean
	rm -f $(NAME)

re : fclean all