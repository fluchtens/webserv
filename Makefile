# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/16 16:17:24 by fluchten          #+#    #+#              #
#    Updated: 2023/08/01 10:09:29 by fluchten         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g
RM = rm -rf

INCS_DIR = includes
SRCS_DIR = srcs
OBJS_DIR = objs

INCS_DIRS = $(shell find $(INCS_DIR) -type d)
INCS = $(addprefix -I, $(INCS_DIRS))

SRCS =	Cgi.cpp \
		Client.cpp \
		Connection.cpp \
		HTTPRequest.cpp \
		HTTPResponse.cpp \
		Location.cpp \
		main.cpp \
		MimeTypes.cpp \
		Parser.cpp \
		Server.cpp \
		Utils.cpp

OBJS = $(addprefix ${OBJS_DIR}/, ${SRCS:%.cpp=%.o})

all: ${NAME}

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.cpp
	@mkdir -p ${@D}
	${CC} ${CFLAGS} ${INCS} -I srcs -c $< -o $@

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean:
	${RM} ${OBJS_DIR}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re

# .SILENT:
