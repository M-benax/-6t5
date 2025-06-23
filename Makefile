# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/01 21:54:06 by aaboudra          #+#    #+#              #
#    Updated: 2025/06/22 23:13:26 by elben-id         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRC = main/minishell.c ./Parsing/tokenization.c ./utils/utils.c \
	./Parsing/flags.c ./Parsing/validation.c memory_management/ft_malloc.c \
	./Parsing/parse.c ./utils/utils_2.c ./segnal/signals.c bilds/bilds.c \
	./utils/ft_split.c Parsing/init_tenv.c bilds/utils_bilds.c bilds/export.c \
	bilds/expand.c bilds/unset.c bilds/utils_bi2.c main/parcing.c \
	pipe/pipeline_executor.c bilds/builtin_cd.c bilds/builtin_echo.c \
	bilds/builtin_exit.c bilds/builtin_pwd.c bilds/builtins_core.c \
	execution/execution_utils.c execution/execution.c \
	redirections/redirection_handler.c path/path_handler.c
CC = cc 
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
OBJ = $(SRC:.c=.o)
HEADER = minishell.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lreadline -o $@
	@echo "\033[32mCompilation of $(NAME) is complete!\033[0m"

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< into $@"

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re