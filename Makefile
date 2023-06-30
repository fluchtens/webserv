# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/16 16:17:24 by fluchten          #+#    #+#              #
#    Updated: 2023/06/30 16:07:10 by fluchten         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -rf

INC_DIR = includes
SRCS_DIR = srcs
OBJS_DIR = objs

INC_DIRS = $(shell find $(INC_DIR) -type d)
INC_FLAGS = $(addprefix -I, $(INC_DIRS))

SRCS =	temp/autoindex.cpp \
		temp/Cgi.cpp \
		temp/Client.cpp \
		temp/Connection.cpp \
		temp/HTTPRequest.cpp \
		temp/Response.cpp \
		temp/Server.cpp \
		main.cpp \
		Parser.cpp \
		utils/errors.cpp utils/signals.cpp utils/utils.cpp

OBJS = $(addprefix ${OBJS_DIR}/, ${SRCS:%.cpp=%.o})

all: ${NAME}

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.cpp
	@mkdir -p ${@D}
	${CC} ${CFLAGS} ${INC_FLAGS} -c $< -o $@

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean:
	${RM} ${OBJS_DIR}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
