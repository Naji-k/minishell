# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:      :+:    :+:    #
#                                                      +:+                     #
#    By: nakanoun <nakanoun@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2023/05/31 14:04:13 by nakanoun      #+#    #+#                  #
#    Updated: 2023/08/29 21:24:42 by ysrondy          ###   ########.fr        #
#                                                                              #1
# **************************************************************************** #


NAME := minishell

CFLAGS := -Wall -Wextra -Werror #-g -fsanitize=address,undefined

#Lib
LIB_LIBFT = ./lib/libft/libft.a

#Readline
YOUSSEF_READLINE_LIB = /opt/homebrew/opt/readline/lib
CODAM_READLINE_LIB = $(HOME)/.brew/opt/readline/lib
CODAM_READLINE_INCLUDE = $(HOME)/.brew/opt/readline/include
YOUSSEF_READLINE_INCLUDE = /opt/homebrew/opt/readline/include
CODAM_READLINE_LIB = $(HOME)/.brew/opt/readline/lib
CODAM_READLINE_INCLUDE = $(HOME)/.brew/opt/readline/include
NAJI_RL_LIB = /usr/local/opt/readline/lib
NAJI_RL_INC = /usr/local/opt/readline/include
DOCKER_LIB = /usr/lib/x86_64-linux-gnu
DOCKER_INCLUDE = /usr/include


#Directories
LIBFT_DIR = ./lib/libft

OBJ_DIR	= obj

HEADERS	:= -I ./include -I $(LIBFT_DIR) -I $(CODAM_READLINE_INCLUDE)

SRCS := src/main.c \
		src/builtins/builtin_utils.c \
		src/builtins/ft_atoll.c \
		src/builtins/mini_cd.c \
		src/builtins/mini_echo.c \
		src/builtins/mini_env.c \
		src/builtins/mini_exit.c \
		src/builtins/mini_export.c \
		src/builtins/mini_export_utils.c \
		src/builtins/mini_pwd.c \
		src/builtins/mini_unset.c \
		src/executor/env_list.c \
		src/executor/env_utils.c \
		src/executor/error_handling.c \
		src/executor/error_handling_extra.c \
		src/executor/executor.c \
		src/executor/heredoc.c \
		src/executor/multi_cmd.c \
		src/executor/one_cmd.c \
		src/executor/redirection.c \
		src/expansion/expander.c \
		src/expansion/expander_utils.c \
		src/linked_list/linked_list_functions.c \
		src/parsing/handle_syntax.c \
		src/parsing/parsing_commands.c \
		src/parsing/parsing_extra.c \
		src/parsing/parsing_quotations.c \
		src/parsing/parsing_redirection.c \
		src/parsing/parsing_tokens.c \
		src/signals/signals.c \
		src/utils/init_tools.c \
		src/utils/print.c \
		src/utils/utils.c \
		src/utils/utils_extra.c \
		src/utils/utils_parsing.c \
		src/utils/utils_syntax.c 

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all :	$(NAME)

$(OBJ_DIR)/%.o: %.c
		@mkdir -p $(@D)
		@$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

$(NAME):	 $(LIB_LIBFT) $(OBJS)
		@$(CC) $(CFLAGS) $(HEADERS) -lreadline $(OBJS) $(LIB_LIBFT) -o $(NAME) -L $(CODAM_READLINE_LIB)
		@printf "Compiled ./minishell executable succesfully.\n"


$(LIB_LIBFT):
	@make -sC $(LIBFT_DIR)
	@printf "Compiled libft succesfully.\n"

clean :
	@rm -rf $(OBJ_DIR);
	@make clean -sC $(LIBFT_DIR)
	@printf "Removed .o files successfully.\n"

fclean : clean
	@make fclean -sC $(LIBFT_DIR)
	@rm -rf $(NAME)
	@printf "Removed ./minishell executable successfully.\n"

re : fclean all

.PHONY: all clean fclean re
