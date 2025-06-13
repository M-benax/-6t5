# ------------------------------ Project Name ------------------------------- #
NAME = minishell

# ------------------------------ Compiler & Flags --------------------------- #
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I.
LDFLAGS = -lreadline

# ------------------------------ Directories -------------------------------- #
DIR_BUILTINS = ./builtins
DIR_EXE = ./exe
DIR_MEMORY = ./memory_management
DIR_PARSING = ./parsing
DIR_PATH = ./path
DIR_PIPE = ./pipe
DIR_REDIRECTIONS = ./redirections
DIR_SIGNAL = ./segnal
DIR_UTILS = ./Utils

# ------------------------------ Source Files ------------------------------- #
FILES_BUILTINS = bilds.c builtin_cd.c builtin_echo.c builtin_exit.c \
                 builtin_pwd.c builtins_core.c expand.c export.c \
                 unset.c utils_bi2.c utils_bilds.c
FILES_EXE = execution.c execution_utils.c minshell.c
FILES_MEMORY = ft_malloc.c
FILES_PARSING = flags.c init_tenv.c parse.c tokenization.c validation.c
FILES_PATH = path_handler.c
FILES_PIPE = pipeline_executor.c
FILES_REDIRECTIONS = redirection_handler.c
FILES_SIGNAL = signals.c
FILES_UTILS = ft_split.c utils_2.c utils.c

SRCS_BUILTINS = $(addprefix $(DIR_BUILTINS)/, $(FILES_BUILTINS))
SRCS_EXE = $(addprefix $(DIR_EXE)/, $(FILES_EXE))
SRCS_MEMORY = $(addprefix $(DIR_MEMORY)/, $(FILES_MEMORY))
SRCS_PARSING = $(addprefix $(DIR_PARSING)/, $(FILES_PARSING))
SRCS_PATH = $(addprefix $(DIR_PATH)/, $(FILES_PATH))
SRCS_PIPE = $(addprefix $(DIR_PIPE)/, $(FILES_PIPE))
SRCS_REDIRECTIONS = $(addprefix $(DIR_REDIRECTIONS)/, $(FILES_REDIRECTIONS))
SRCS_SIGNAL = $(addprefix $(DIR_SIGNAL)/, $(FILES_SIGNAL))
SRCS_UTILS = $(addprefix $(DIR_UTILS)/, $(FILES_UTILS))

SRCS = $(SRCS_BUILTINS) $(SRCS_EXE) $(SRCS_MEMORY) $(SRCS_PARSING) \
       $(SRCS_PATH) $(SRCS_PIPE) $(SRCS_REDIRECTIONS) $(SRCS_SIGNAL) \
       $(SRCS_UTILS)

# ------------------------------ Object Files ------------------------------- #
OBJS = $(SRCS:.c=.o)

# ------------------------------ Header Files ------------------------------- #
HEADER = minishell.h

# ------------------------------ Rules -------------------------------------- #
all: $(NAME)

$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "\033[32m$(NAME) compiled successfully!\033[0m"

%.o: %.c $(HEADER)
	@echo "Compiling $< into $@"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@echo "OBJS are: [$(OBJS)]" # Added brackets for clarity
	@rm -f $(OBJS)
	@echo "Object files cleaned."

fclean: clean
	@echo "Cleaning $(NAME)..."
	@rm -f $(NAME)
	@echo "$(NAME) cleaned."

re: fclean all

.PHONY: all clean fclean re