
NAME = minishell

SOURCE = main.c executor/here_doc.c executor/pipes_processes2.c executor/path_finders.c executor/multi_pipe.c executor/pipes_processes.c executor/file_redirections.c executor/file_redirections2.c executor/executor.c executor/executor_utils.c executor/executor_utils2.c executor/executor_utils3.c executor/executor_utils4.c environment.c environment2.c synthax/synthax.c synthax/synthax2.c

SOURCEBUILTINS = builtins/builtins.c builtins/cd_functions.c builtins/builtin_utils.c builtins/builtin_utils2.c builtins/echo.c builtins/export3.c builtins/exit_cmds.c builtins/export2.c builtins/export.c  builtins/unset.c

SOURCETOK = signal.c freefuncs.c ./tokenizer/token.c ./tokenizer/lsthelper.c ./tokenizer/tokenhelper.c ./tokenizer/tokenhelper2.c ./tokenizer/tokenhelper3.c

SOURCEPARSE = ./parser/parse.c ./parser/prep_exec.c ./parser/prep_exec2.c ./parser/prep_exec3.c ./parser/parsehelper.c ./parser/expand.c ./parser/prep_exec_helper.c ./parser/cmd_lst.c ./parser/split_vars.c ./parser/fuse_nodes.c ./parser/heredoc_delimiter.c ./parser/parse_around_var.c ./parser/parse_around_var2.c ./parser/fuse_node_word.c ./parser/copy_exec_args.c ./parser/expand_in_doublequote.c

OBJECTS = $(SOURCE:.c=.o)

OBJECTSBUILTINS = $(SOURCEBUILTINS:.c=.o)

OBJECTSTOK = $(SOURCETOK:.c=.o)

OBJECTSPARSE = $(SOURCEPARSE:.c=.o)

CFLAGS = -Wall -Werror -Wextra -g

COMP = cc -lreadline

RM = rm -f

all: ${NAME}

.c.o:
	-cc ${CFLAGS} -c $< -o ${<:.c=.o} 

${NAME}: ${OBJECTS} ${OBJECTSBUILTINS} ${OBJECTSTOK} $(OBJECTSPARSE)
		${MAKE} -C ./libft --no-print-directory
		${COMP} ${CFLAGS} ${OBJECTS} ${OBJECTSBUILTINS} ${OBJECTSTOK} $(OBJECTSPARSE) ./libft/libft.a -o ${NAME}

clean: 
	${MAKE} -C ./libft --no-print-directory fclean
	${RM} ${OBJECTS} ${OBJECTSBUILTINS} ${OBJECTSTOK} $(OBJECTSPARSE)

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
