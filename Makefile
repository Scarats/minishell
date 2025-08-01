SRCS	= minishell.c

FLAGS	= -Wall -Wextra -Werror 
CC		= cc
RM		= rm -f
OBJ_DIR = dot_o
OBJS	= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

NAME	= minishell

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	${CC} ${FLAGS} -c $< -o $@

${NAME}: 	${OBJS}
	@make -C ./libft
	${CC} ${OBJS} -Llibft -lft -o ${NAME} 

all: 		${NAME}

clean:	
	@make fclean -C libft
	${RM} -r ${OBJ_DIR}

fclean:		clean
	${RM} ${NAME} 

re:			fclean all

bonus: re
    
.PHONY:		all bonus clean fclean re