NAME = ircserv

SRC_DIR = srcs/
OBJ_DIR = objs/
INC_DIR = includes/

INCS = -I$(INC_DIR)

SRC =	main.cpp \
		utils.cpp \
		Server.cpp \
		Client.cpp \
		IrcParser.cpp \
		Request.cpp \
		commands/commands.cpp \
		Channel.cpp \
		commands/join.cpp \
		commands/privmsg.cpp\
		commands/part.cpp\
		commands/notice.cpp \
		commands/mode.cpp \
		commands/invite.cpp \
		commands/who.cpp \
		commands/topic.cpp \
		commands/oper.cpp \
		commands/kick.cpp \
		commands/die.cpp \
		commands/whois.cpp \
		commands/list.cpp

OBJ := $(SRC:%.cpp=%.o)

SRCS = $(addprefix $(SRC_DIR), $(SRC))
OBJS = $(addprefix $(OBJ_DIR), $(OBJ))

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -Wfatal-errors -fsanitize=address
# CXXFLAGS = -std=c++98

all: $(NAME)

$(NAME): $(SRCS) $(OBJ_DIR) $(OBJS)
	@c++ $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	@rm -f $(OBJS)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@c++ $(CXXFLAGS) $(INCS) $< -c -o $@

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)
	@mkdir $(OBJ_DIR)/commands
