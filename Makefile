# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/01 21:54:06 by aaboudra          #+#    #+#              #
#    Updated: 2025/05/31 16:51:06 by elben-id         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRC = ./exe/minishell.c ./Parsing/tokenization.c ./utils/utils.c  \
	./Parsing/flags.c ./Parsing/validation.c ./memory_management/ft_malloc.c \
	./Parsing/parse.c ./utils/utils_2.c ./segnal/signals.c bilds/bilds.c  \
	 ./utils/ft_split.c Parsing/init_tenv.c bilds/utils_bilds.c bilds/export.c \
	 bilds/expand.c bilds/unset.c bilds/utils_bi2.c ./bilds/builtin_echo.c ./bilds/builtin_pwd.c \
	 ./bilds/builtin_cd.c ./bilds/builtin_exit.c ./bilds/builtins_core.c \
	 ./exe/execution_utils.c ./exe/execution.c ./path/path_handler.c \
	 ./pipe/pipeline_executor.c ./redirection/redirection_handler.c

	 
CC = cc 
CFLAGS = -Wall -Wextra -Werror #-g -fsanitize=address
OBJ = $(SRC:.c=.o)
HEADER = minishell.h

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -lreadline -o $@
	@echo "Compilation of $(NAME) is complete!"

%.o: %.c $(HEADER)
	@$(CC) $(CFLAGS)  -c $< -o $@
	@echo "Compiled $< into $@"

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all