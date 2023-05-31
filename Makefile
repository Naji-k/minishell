# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: nakanoun <nakanoun@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2023/05/31 14:04:13 by nakanoun      #+#    #+#                  #
#    Updated: 2023/05/31 14:04:13 by nakanoun      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #


NAME := minishell

CFLAGS := -Wall -Wextra -Werror -g -fsanitize=address,undefined
# CFLAGS := -g -fsanitize=address

#Lib
LIB_LIBFT = ./lib/libft/libft.a

#Directories
LIBFT_DIR = ./lib/libft
OBJ_DIR	= obj

HEADERS	:= -I ./include -I $(LIBFT_DIR)
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
