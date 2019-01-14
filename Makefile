#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msakovyc <msakovyc@student.unit.ua>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/05 18:13:54 by msakovyc          #+#    #+#              #
#    Updated: 2018/11/05 18:13:59 by msakovyc         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME	:= avm

CC		:= clang++
FLAGS	:= -Wall -Werror -Wextra

DEPS	:= IOperand.h Operand.hpp VM.hpp Creator.hpp Exception.hpp Parser.hpp Lexer.hpp
SRCS	:= main.cpp Creator.cpp VM.cpp Exception.cpp Parser.cpp Lexer.cpp
OBJS	:= $(SRCS:%.cpp=%.o)

all: $(NAME)

$(NAME): $(OBJS)
	@ $(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	@ $(CC) $(CFLAGS) -c $< -o $@

clean:
	@ rm -f $(OBJS)

fclean: clean
	@ rm -f $(NAME)

re: fclean all
