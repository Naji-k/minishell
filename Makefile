# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:      :+:    :+:    #
#                                                      +:+                     #
#    By: nakanoun <nakanoun@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2023/04/25 13:56:08 by nakanoun      #+#    #+#                  #
#    Updated: 2023/05/17 10:13:53 by ysrondy          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

# CFLAGS := -Wall -Wextra -Werror #-g -fsanitize=address
# CFLAGS := -g -fsanitize=address

#Lib
LIB_LIBFT = ./lib/libft/libft.a

#Directories
LIBFT_DIR = ./lib/libft
OBJ_DIR	= obj

HEADERS	:= -I ./include -I $(LIBFT_DIR)
# FILES	:= main.c parsing_tokens.c linked_list_functions.c utils.c parsing_commands.c execute_cmd.c init_tools.c print.c expander.c
SRCS	:= $(shell find ./src -iname "*.c")
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all :	$(NAME)

$(OBJ_DIR)/%.o: %.c
		@mkdir -p $(@D)
		@$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

$(NAME):	 $(LIB_LIBFT) $(OBJS)
		@$(CC) $(CFLAGS) $(HEADERS) $(OBJS) $(LIB_LIBFT) -o $(NAME) -lreadline
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
