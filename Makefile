# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/12 11:25:31 by dgarizad          #+#    #+#              #
#    Updated: 2024/05/16 18:30:35 by dgarizad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


################################################################################
#                                     CONFIG                                   #
################################################################################

NAME = webserv

HEADERS = includes/webserv.hpp  includes/ConfParse.hpp includes/Master.hpp  #includes/VHost.hpp
OBJDIR = ./obj


################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

# SRCS =  $(wildcard srcs/*.cpp)
SRCS = main.cpp ConfParse.cpp Master.cpp utils.cpp VHost.cpp 
SRCS := $(addprefix srcs/, $(SRCS))
# SRCS = srcs/main.cpp srcs/ConfParse.cpp srcs/Master.cpp srcs/utils.cpp
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
# OBJS = $(SRCS:srcs/%.cpp=$(OBJDIR)/%.o)

$(OBJDIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(@D)
	g++ -Wall -Wextra -Werror -std=c++98 -c $< -o $@





################################################################################
#                                  Makefile  objs                              #
################################################################################

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
		g++ -Wall -Wextra -Werror -std=c++98 -o $(NAME) $(OBJS)