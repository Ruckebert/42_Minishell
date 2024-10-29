
NAME = minishell

SOURCE = main.c executor/here_doc.c executor/multi_pipe.c builtins/builtins.c  builtins/builtin_utils.c  builtins/export.c  builtins/unset.c executor/executor.c executor/executor_utils.c environment.c

SOURCETOK = ./tokenizer/testing.c ./tokenizer/token.c ./tokenizer/lsthelper.c ./tokenizer/tokenhelper.c

SOURCEPARSE = ./parser/parse.c ./parser/prep_exec.c

OBJECTS = $(SOURCE:.c=.o)

OBJECTSTOK = $(SOURCETOK:.c=.o)

OBJECTSPARSE = $(SOURCEPARSE:.c=.o)

CFLAGS = -Wall -Werror -Wextra -g

COMP = cc -lreadline

RM = rm -f

all: ${NAME}

.c.o:
	-cc ${CFLAGS} -c $< -o ${<:.c=.o} 

${NAME}: ${OBJECTS} ${OBJECTSTOK} $(OBJECTSPARSE)
		${MAKE} -C ./libft --no-print-directory
		${COMP} ${CFLAGS} ${OBJECTS} ${OBJECTSTOK} $(OBJECTSPARSE) ./libft/libft.a -o ${NAME}

clean: 
	${MAKE} -C ./libft --no-print-directory fclean
	${RM} ${OBJECTS} ${OBJECTSTOK} $(OBJECTSPARSE)

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
