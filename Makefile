# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: nakanoun <nakanoun@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2023/05/31 14:04:13 by nakanoun      #+#    #+#                  #
#    Updated: 2023/08/05 00:43:47 by ysrondy       ########   odam.nl          #
#                                                                              #
# **************************************************************************** #


NAME := minishell

CFLAGS := -Wall -Wextra -Werror -g #-fsanitize=address,undefined

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

#Directories
LIBFT_DIR = ./lib/libft

OBJ_DIR	= obj

HEADERS	:= -I ./include -I $(LIBFT_DIR) -I $(CODAM_READLINE_INCLUDE)
SRCS	:= $(shell find ./src -iname "*.c") # you're not allowed to do this (shell) explicitly state source files.
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all :	$(NAME)

$(OBJ_DIR)/%.o: %.c
		@mkdir -p $(@D)
		@$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

$(NAME):	 $(LIB_LIBFT) $(OBJS)
		@$(CC) $(CFLAGS) $(HEADERS) -lreadline $(OBJS) $(LIB_LIBFT) -o $(NAME) -L$(CODAM_READLINE_LIB)
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
