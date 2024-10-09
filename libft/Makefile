
NAME = libft.a

SOURCE = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_strchr.c ft_strdup.c ft_strlcpy.c  ft_strlcat.c ft_strlen.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_tolower.c ft_toupper.c ft_itoa.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_striteri.c ft_strjoin.c ft_strmapi.c ft_strtrim.c ft_substr.c ft_printf.c ft_putchar_print.c ft_putnbr_count.c ft_putnbr_hex.c ft_putnbr_pointhex.c ft_putnbr_unsigned.c ft_putstr_print.c

OBJECTS = $(SOURCE:.c=.o)

CFLAGS = -Wall -Werror -Wextra

COMP = cc -g

AR = ar -rc

RM = rm -f

all: ${NAME}

.c.o:
	${COMP} ${CFLAGS} -c $< -o ${<:.c=.o} 

${NAME}: ${OBJECTS} 
	 ${AR} ${NAME} ${OBJECTS}
	 ranlib ${NAME}

clean: 
	${RM} ${OBJECTS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
