NAME = minishell

SRCS = main.c parsing_tokens.c linked_list_functions.c utils.c parsing_commands.c execute_cmd.c init_tools.c print.c

LIBFT = libft/libft.a

OBJS = $(SRCS:.c=.o)

CFLAGS ?= -Wall -Wextra -Werror #-g -fsanitize=address

all : $(NAME)

$(NAME): $(OBJS)
	@cd libft && make
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline
	@printf "Compiled ./minishell executable succesfully.\n"

clean :
	@cd libft && make clean
	@rm -rf $(OBJS)
	@printf "Removed .o files successfully.\n"

fclean : clean
	@cd libft && make fclean
	@rm -rf $(NAME)
	@printf "Removed ./minishell executable successfully.\n"

re : fclean all

.PHONY: all clean fclean re
