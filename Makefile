DEBUG = false
NAME = webserv

SRC = ./config/ConfigParse/srcs/lexer.cpp \
	  ./config/ConfigParse/srcs/token.cpp \
	  ./config/ConfigParse/srcs/server.cpp \
	  ./config/ConfigParse/srcs/location.cpp \
	  ./config/ConfigParse/srcs/http.cpp \
	  ./config/ConfigParse/srcs/parse.cpp \
	  ./config/ConfigParse/srcs/defaultConfig.cpp \
	  ./Server/ServerDrive.cpp \
	  ./Server/Client.cpp \
	  ./Server/HttpRequest.cpp \
	  ./Server/Utils.cpp \
	  ./Server/ErrorHandler.cpp \
	  ./Server/Colors.cpp \
	  ./Response/Response.cpp \
	  ./Handler/Handler.cpp \
	  ./Handler/CGI.cpp \
	  ./Handler/HandlerUtils.cpp \
	  ./Response/ResponseUtils.cpp \
	  main.cpp

OBJS=${SRC:.cpp=.o} 

FLAGS = -Wall -Werror -Wextra -fsanitize=address

COMPILER = c++

STANDARD = -std=c++98

all: $(NAME)

%.o: %.cpp
	$(COMPILER) $(FLAGS) -c -D DEBUG=$(DEBUG) $< -o ${<:.cpp=.o}

$(NAME): $(OBJS)
	$(COMPILER) $(FLAGS) $(STANDARD) $(OBJS) -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

