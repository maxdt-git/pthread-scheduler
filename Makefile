# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mde-nas <maxdenasdetourris@gmail.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/01 13:51:11 by mde-nas           #+#    #+#              #
#    Updated: 2026/07/06 11:47:23 by mde-nas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
IFLAGS = -Iheader
ARGS = 3 2000 300 200 100 2 100 fifo

SRCS = parse.c main.c utils/utils.c utils/utils_routine.c utils/utils_main.c \
stop_simu.c routine.c utils/utils_dongles.c scheduler.c

OBJS = ${SRCS:.c=.o}

all: ${NAME}

%.o: %.c
	${CC} ${CFLAGS} ${IFLAGS} -c -o $@ $<

$(NAME): ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@

valgrind:
	valgrind ./${NAME} ${ARGS}
run:
	./$(NAME) $(ARGS)
clean:
	rm -f ${OBJS}
fclean: clean
	rm -f ${NAME}
re: fclean all

.PHONY: all run clean fclean re valgrind
