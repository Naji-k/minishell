# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:      :+:    :+:    #
#                                                      +:+                     #
#    By: nakanoun <nakanoun@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2023/05/31 14:04:13 by nakanoun      #+#    #+#                  #
#    Updated: 2023/08/05 00:43:47 by ysrondy          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME := minishell

CFLAGS := -Wall -Wextra -Werror #-fsanitize=address,undefined

#Lib
LIB_LIBFT = ./lib/libft/libft.a
# LIB_READLINE = /opt/homebrew/Cellar/readline/8.2.1/lib/libreadline.a

#Directories
LIBFT_DIR = ./lib/libft

# READLINE_DIR= ./lib/readline
OBJ_DIR	= obj

HEADERS	:= -I ./include -I $(LIBFT_DIR) -I $(HOME)/.brew/opt/readline/include
SRCS	:= $(shell find ./src -iname "*.c") # you're not allowed to do this (shell) explicitly state source files.
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all :	$(NAME)

$(OBJ_DIR)/%.o: %.c
		@mkdir -p $(@D)
		@$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

$(NAME):	 $(LIB_LIBFT) $(OBJS)
		@$(CC) $(CFLAGS) $(HEADERS) -lreadline $(OBJS) $(LIB_LIBFT) -o $(NAME) -L$(HOME)/.brew/opt/readline/lib
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
